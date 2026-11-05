#include "edusvm/preprocessor_strategies.hpp"
#include "edusvm/preprocessor.hpp"

#include <stdexcept>

namespace edusvm {

// Adapt the standalone validation function to the common strategy interface.
bool ValidationPreprocessor::process(
    const Dataset& input,
    Dataset& output,
    std::string& errorMessage
) const {
    return preprocessDataset(input, output, errorMessage);
}

// Fit a scaler on this dataset and return a scaled copy; convert scaler
// exceptions into the error-message convention used by Preprocessor.
bool ScalingPreprocessor::process(
    const Dataset& input,
    Dataset& output,
    std::string& errorMessage
) const {
    try {
        Scaler scaler;
        output = scaler.fitTransform(input);
        errorMessage.clear();
        return true;
    }
    catch (const std::invalid_argument& error) {
        errorMessage = error.what();
        return false;
    }
    catch (const std::logic_error& error) {
        errorMessage = error.what();
        return false;
    }
}

bool ImputationPreprocessor::process(
    const Dataset& input,
    Dataset& output,
    std::string& errorMessage
) const {

    if (input.features.empty()) {
        errorMessage = "Imputation failed: dataset is empty.";
        return false;
    }

    if (input.features.size() != input.labels.size()) {
        errorMessage = "Imputation failed: feature and label counts do not match.";
        return false;
    }

    const std::size_t featureCount = input.features[0].size();

    if (featureCount == 0) {
        errorMessage = "Imputation failed: dataset has no features.";
        return false;
    }

    for (std::size_t i = 0; i < input.features.size(); ++i) {
        if (input.features[i].size() != featureCount) {
            errorMessage = "Imputation failed: inconsistent feature count at row "
                + std::to_string(i) + ".";
            return false;
        }

        if (input.labels[i] != -1 && input.labels[i] != 1) {
            errorMessage = "Imputation failed: invalid label at row "
                + std::to_string(i) + ".";
            return false;
        }

        for (double value : input.features[i]) {
            if (std::isinf(value)) {
                errorMessage = "Imputation failed: infinite feature value at row "
                    + std::to_string(i) + ".";
                return false;
            }
        }
    }

    Dataset result;

    if (strategy_ == ImputationStrategy::MEAN) {
        std::vector<double> sums(featureCount, 0.0);
        std::vector<std::size_t> counts(featureCount, 0);

        for (const auto& row : input.features) {
            for (std::size_t j = 0; j < featureCount; ++j) {
                if (!std::isnan(row[j])) {
                    sums[j] += row[j];
                    ++counts[j];
                }
            }
        }

        std::vector<double> means(featureCount);

        for (std::size_t j = 0; j < featureCount; ++j) {
            if (counts[j] == 0) {
                errorMessage = "Imputation failed: a feature column contains only missing values.";
                return false;
            }

            means[j] = sums[j] / counts[j];

            if (!std::isfinite(means[j])) {
                errorMessage = "Imputation failed: calculated mean is not finite.";
                return false;
            }
        }

        result = input;

        for (auto& row : result.features) {
            for (std::size_t j = 0; j < featureCount; ++j) {
                if (std::isnan(row[j])) {
                    row[j] = means[j];
                }
            }
        }
    }
    else if (strategy_ == ImputationStrategy::DROP_ROW) {
        for (std::size_t i = 0; i < input.features.size(); ++i) {
            bool hasMissingValue = false;

            for (double value : input.features[i]) {
                if (std::isnan(value)) {
                    hasMissingValue = true;
                    break;
                }
            }

            if (!hasMissingValue) {
                result.features.push_back(input.features[i]);
                result.labels.push_back(input.labels[i]);
            }
        }

        if (result.features.empty()) {
            errorMessage = "Imputation failed: no rows remain after dropping missing values.";
            return false;
        }
    }
    else {
        errorMessage = "Imputation failed: unknown strategy.";
        return false;
    }

    output = result;
    errorMessage.clear();
    return true;
}

} // namespace edusvm
