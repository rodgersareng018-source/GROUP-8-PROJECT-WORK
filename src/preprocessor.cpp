
#include "edusvm/preprocessor.hpp"
#include "edusvm/preprocessor_strategies.hpp"
#include <cmath>
#include <limits>
#include <string>

namespace edusvm {

DatasetPreprocessor::DatasetPreprocessor()
    : Preprocessor("Dataset Validation") {}

bool DatasetPreprocessor::process(
    const Dataset& input,
    Dataset& output,
    std::string& errorMessage
) const {
    if (input.features.empty()) {
        errorMessage = "Preprocessing failed: dataset is empty.";
        return false;
    }

    if (input.features.size() != input.labels.size()) {
        errorMessage =
            "Preprocessing failed: number of labels does not match number of rows.";
        return false;
    }

    std::size_t expectedFeatures = input.features[0].size();
    if (expectedFeatures == 0) {
        errorMessage =
            "Preprocessing failed: rows contain zero features.";
        return false;
    }

    for (std::size_t i = 0; i < input.features.size(); ++i) {
        const auto& sample = input.features[i];

        if (sample.size() != expectedFeatures) {
            errorMessage =
                "Preprocessing failed: inconsistent feature count at row "
                + std::to_string(i) + ".";
            return false;
        }

        for (double val : sample) {
            if (!std::isfinite(val)) {
                errorMessage =
                    "Preprocessing failed: non-finite value at row "
                    + std::to_string(i) + ".";
                return false;
            }
        }

        if (input.labels[i] != 1 && input.labels[i] != -1) {
            errorMessage =
                "Preprocessing failed: invalid label at row "
                + std::to_string(i) + " (must be 1 or -1).";
            return false;
        }
    }

    output = input;
    errorMessage.clear();

    return true;
}

bool preprocessDataset(
    const Dataset& input,
    Dataset& output,
    std::string& errorMessage
) {
    DatasetPreprocessor validator;
    return validator.process(input, output, errorMessage);
}

} // namespace edusvm
