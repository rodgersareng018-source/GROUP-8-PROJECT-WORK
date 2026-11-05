
#include "edusvm/preprocessor_strategies.hpp"
#include <limits>
#include "edusvm/preprocessor.hpp"

#include <cmath>
#include <string>

namespace edusvm {

bool preprocessDataset(
    const Dataset& input,
    Dataset& output,
    std::string& errorMessage
) {
    // Check whether the dataset is empty.
    if (input.features.empty()) {
        errorMessage = "Preprocessing failed: dataset is empty.";
        return false;
    }

    // Check that the number of labels matches the number of rows.
    if (input.features.size() != input.labels.size()) {
        errorMessage =
            "Preprocessing failed: feature and label counts do not match.";
        return false;
    }

    // Determine the expected number of features.
    const std::size_t expectedFeatures = input.features[0].size();

    if (expectedFeatures == 0) {
        errorMessage =
            "Preprocessing failed: rows contain zero features.";
        return false;
    }

    // Validate every row.
    for (std::size_t i = 0; i < input.features.size(); ++i) {
        const auto& sample = input.features[i];

        if (sample.size() != expectedFeatures) {
            errorMessage =
                "Preprocessing failed: inconsistent feature count at row "
                + std::to_string(i) + ".";
            return false;
        }

        // Validation preprocessing rejects NaN and infinity.
        for (double value : sample) {
            if (!std::isfinite(value)) {
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

    // Copy only after all validation succeeds.
    output = input;
    errorMessage.clear();

    return true;
}

} // namespace edusvm
