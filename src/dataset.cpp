
#include "edusvm/dataset.hpp"

#include <cmath>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cctype>

namespace edusvm {

namespace {

// Remove spaces from the beginning and end of a value.
std::string trim(const std::string& value) {
    const std::size_t first = value.find_first_not_of(" \t\r\n");

    if (first == std::string::npos) {
        return "";
    }

    const std::size_t last = value.find_last_not_of(" \t\r\n");

    return value.substr(first, last - first + 1);
}

// Split a simple comma-separated CSV row.
std::vector<std::string> splitCSVRow(const std::string& line) {
    std::vector<std::string> values;
    std::stringstream stream(line);
    std::string value;

    while (std::getline(stream, value, ',')) {
        values.push_back(trim(value));
    }

    // Preserve an empty final column if the row ends with a comma.
    if (!line.empty() && line.back() == ',') {
        values.push_back("");
    }

    return values;
}

// Convert a string into a finite double.
bool parseNumber(const std::string& text, double& result) {
    std::istringstream stream(text);

    stream >> result;

    if (!stream || !std::isfinite(result)) {
        return false;
    }

    stream >> std::ws;

    return stream.eof();
}

} // anonymous namespace


bool DatasetLoader::loadFromCSV(
    const std::string& filePath,
    Dataset& dataset,
    std::string& errorMessage
) {
    errorMessage.clear();

    std::ifstream file(filePath);

    if (!file.is_open()) {
        errorMessage = "Could not open CSV file: " + filePath;
        return false;
    }

    std::string line;

    // Read the header row.
    if (!std::getline(file, line)) {
        errorMessage = "CSV file is empty.";
        return false;
    }

    const std::vector<std::string> headings = splitCSVRow(line);

    // At least one feature column and one label column are required.
    if (headings.size() < 2) {
        errorMessage = "CSV must contain at least one feature column and one label column.";
        return false;
    }

    Dataset loadedDataset;
    std::size_t lineNumber = 1;

    // Read the student records.
    while (std::getline(file, line)) {
        ++lineNumber;

        if (trim(line).empty()) {
            continue;
        }

        const std::vector<std::string> values = splitCSVRow(line);

        if (values.size() != headings.size()) {
            errorMessage =
                "Incorrect number of columns on CSV line " +
                std::to_string(lineNumber) + ".";
            return false;
        }

        std::vector<double> features;

        // Every column except the final one is a numerical feature.
        for (std::size_t i = 0; i + 1 < values.size(); ++i) {
            double featureValue = 0.0;

            if (!parseNumber(values[i], featureValue)) {
                errorMessage =
                    "Invalid feature value on CSV line " +
                    std::to_string(lineNumber) + ".";
                return false;
            }

            features.push_back(featureValue);
        }

        // The final column is the binary class label.
        double labelValue = 0.0;

        if (!parseNumber(values.back(), labelValue)) {
            errorMessage =
                "Invalid label on CSV line " +
                std::to_string(lineNumber) + ".";
            return false;
        }

        if (labelValue != -1.0 && labelValue != 1.0) {
            errorMessage =
                "Labels must be -1 or +1 on CSV line " +
                std::to_string(lineNumber) + ".";
            return false;
        }

        loadedDataset.features.push_back(features);
        loadedDataset.labels.push_back(
            static_cast<int>(labelValue)
        );
    }

    if (!file.eof()) {
        errorMessage = "An error occurred while reading the CSV file.";
        return false;
    }

    // Validate the completed dataset.
    if (!validateDataset(loadedDataset, errorMessage)) {
        return false;
    }

    // Update the output only after successful loading and validation.
    dataset = loadedDataset;

    return true;
}


bool validateDataset(
    const Dataset& dataset,
    std::string& errorMessage
) {
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

} // namespace edusvm
