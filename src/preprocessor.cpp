
#include "edusvm/preprocessor.hpp"

#include <cmath>

namespace edusvm {

// Check every row before copying the dataset, preventing invalid indexing and
// ensuring callers either receive a fully validated copy or a clear error.
bool preprocessDataset(
    const Dataset& input,
    Dataset& output,
    std::string& errorMessage
 ) {
    if (input.features.empty()) {
        errorMessage = "Preprocessing failed: dataset is empty.";
        return false;
    }

    // Check this before looking up labels by row index.
    if (input.features.size() != input.labels.size()) {
        errorMessage =
            "Preprocessing failed: number of labels does not match number of rows.";
        return false;
    }

    // Use the first row as the expected feature width for every student.
    std::size_t expectedFeatures = input.features[0].size();
    if (expectedFeatures == 0) {
        errorMessage = "Preprocessing failed: rows contain zero features.";
        return false;
    }

    for (std::size_t i = 0; i < input.features.size(); ++i) {
        const auto& sample = input.features[i];

        if (sample.size() != expectedFeatures) {
            errorMessage = "Preprocessing failed: inconsistent feature count at row " + std::to_string(i) + ".";
            return false;
        }

        for (double val : sample) {
            if (!std::isfinite(val)) {
                errorMessage = "Preprocessing failed: non-finite value at row " + std::to_string(i) + ".";
                return false;
            }
        }

        if (input.labels[i] != 1 && input.labels[i] != -1) {
            errorMessage = "Preprocessing failed: invalid label at row " + std::to_string(i) + " (must be 1 or -1).";
            return false;
        }
    }

    // Preserve the input unchanged; this stage validates rather than modifies.
    output = input;
    errorMessage = "";
    return true;
}

} // namespace edusvm