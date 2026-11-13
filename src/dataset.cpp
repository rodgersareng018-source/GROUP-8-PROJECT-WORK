
#include "edusvm/dataset.hpp"

#include <cctype>
#include <cmath>
#include <fstream>
#include <stdexcept>
#include <utility>

namespace edusvm {

namespace {

std::string trim(const std::string& value);
bool parseCsvRow(const std::string& line, std::vector<std::string>& fields);
bool isBlank(const std::string& line);
bool parseNumber(const std::string& field, double& value);

class DatasetValidator {
public:
    bool validate(const Dataset& dataset, std::string& errorMessage) const {
        errorMessage.clear();

        if (dataset.features.empty()) {
            errorMessage = "Dataset contains no feature rows.";
            return false;
        }

        const std::size_t featureCount = dataset.features[0].size();

        if (featureCount == 0) {
            errorMessage = "Dataset contains no features.";
            return false;
        }

        if (dataset.features.size() != dataset.labels.size()) {
            errorMessage = "Number of labels does not match number of rows.";
            return false;
        }

        for (const auto& row : dataset.features) {
            if (row.size() != featureCount) {
                errorMessage = "Rows have inconsistent numbers of features.";
                return false;
            }

            for (double value : row) {
                if (!std::isfinite(value)) {
                    errorMessage = "Feature contains a non-finite value.";
                    return false;
                }
            }
        }

        for (int label : dataset.labels) {
            if (label != -1 && label != 1) {
                errorMessage = "Labels must be -1 or +1.";
                return false;
            }
        }

        return true;
    }
};

class CsvDatasetLoader {
public:
    bool load(
        const std::string& filePath,
        Dataset& dataset,
        std::string& errorMessage
    ) const {
        std::ifstream input(filePath);
        if (!input) {
            errorMessage = "Could not open CSV file: " + filePath;
            return false;
        }

        std::string line;
        std::size_t lineNumber = 0;
        do {
            if (!std::getline(input, line)) {
                errorMessage = "CSV file is empty or has no header.";
                return false;
            }
            ++lineNumber;
        } while (isBlank(line));

        std::vector<std::string> headers;
        if (!parseCsvRow(line, headers) || headers.size() < 2) {
            errorMessage = "CSV header must contain at least one feature and a label.";
            return false;
        }

        for (auto& header : headers) {
            header = trim(header);
        }
        std::string labelHeader = headers.back();
        for (char& character : labelHeader) {
            character = static_cast<char>(
                std::tolower(static_cast<unsigned char>(character))
            );
        }
        if (labelHeader != "label") {
            errorMessage = "The last CSV column must be named 'label'.";
            return false;
        }
        for (std::size_t i = 0; i < headers.size(); ++i) {
            if (headers[i].empty()) {
                errorMessage = "CSV column names cannot be empty.";
                return false;
            }
            for (std::size_t j = i + 1; j < headers.size(); ++j) {
                if (headers[i] == headers[j]) {
                    errorMessage = "CSV column names must be unique.";
                    return false;
                }
            }
        }

        Dataset loaded;
        while (std::getline(input, line)) {
            ++lineNumber;
            if (isBlank(line)) {
                continue;
            }

            std::vector<std::string> fields;
            if (!parseCsvRow(line, fields) || fields.size() != headers.size()) {
                errorMessage = "Malformed CSV row at line " +
                    std::to_string(lineNumber) + ".";
                return false;
            }

            std::vector<double> features;
            features.reserve(fields.size() - 1);
            for (std::size_t i = 0; i + 1 < fields.size(); ++i) {
                double value = 0.0;
                if (!parseNumber(fields[i], value) || !std::isfinite(value)) {
                    errorMessage = "Invalid feature value at line " +
                        std::to_string(lineNumber) + ".";
                    return false;
                }
                features.push_back(value);
            }

            double labelValue = 0.0;
            if (!parseNumber(fields.back(), labelValue) ||
                (labelValue != -1.0 && labelValue != 1.0)) {
                errorMessage = "Label at line " + std::to_string(lineNumber) +
                    " must be -1 or +1.";
                return false;
            }
            loaded.features.push_back(std::move(features));
            loaded.labels.push_back(static_cast<int>(labelValue));
        }

        if (input.bad()) {
            errorMessage = "Error reading CSV file: " + filePath;
            return false;
        }
        DatasetValidator validator;
        if (!validator.validate(loaded, errorMessage)) {
            errorMessage = "Invalid CSV dataset: " + errorMessage;
            return false;
        }

        dataset = std::move(loaded);
        errorMessage.clear();
        return true;
    }
};

std::string trim(const std::string& value) {
    const auto first = value.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) {
        return {};
    }

    const auto last = value.find_last_not_of(" \t\r\n");
    return value.substr(first, last - first + 1);
}

bool parseCsvRow(
    const std::string& line,
    std::vector<std::string>& fields
) {
    // Keep commas inside quoted fields as part of the field value.
    fields.clear();
    std::string field;
    bool quoted = false;
    bool closedQuote = false;

    for (std::size_t i = 0; i < line.size(); ++i) {
        const char character = line[i];
        if (quoted) {
            if (character == '"') {
                if (i + 1 < line.size() && line[i + 1] == '"') {
                    field += '"';
                    ++i;
                } else {
                    quoted = false;
                    closedQuote = true;
                }
            } else {
                field += character;
            }
        } else if (character == ',') {
            fields.push_back(trim(field));
            field.clear();
            closedQuote = false;
        } else if (character == '"' && field.empty() && !closedQuote) {
            quoted = true;
        } else if (closedQuote && character != ' ' && character != '\t' &&
                   character != '\r') {
            return false;
        } else {
            field += character;
        }
    }

    if (quoted) {
        return false;
    }
    fields.push_back(trim(field));
    return true;
}

bool isBlank(const std::string& line) {
    return trim(line).empty();
}

bool parseNumber(const std::string& field, double& value) {
    try {
        std::size_t parsedLength = 0;
        value = std::stod(field, &parsedLength);
        return parsedLength == field.size();
    } catch (const std::invalid_argument&) {
        return false;
    } catch (const std::out_of_range&) {
        return false;
    }
}

} // namespace

bool validateDataset(
    const Dataset& dataset,
    std::string& errorMessage
) {
    DatasetValidator validator;
    return validator.validate(dataset, errorMessage);
}

bool loadDatasetCsv(
    const std::string& filePath,
    Dataset& dataset,
    std::string& errorMessage
) {
    CsvDatasetLoader loader;
    return loader.load(filePath, dataset, errorMessage);
}

bool DatasetLoader::loadFromCSV(
    const std::string& filePath,
    Dataset& dataset,
    std::string& errorMessage
) {
    return loadDatasetCsv(filePath, dataset, errorMessage);
}

} // namespace edusvm
