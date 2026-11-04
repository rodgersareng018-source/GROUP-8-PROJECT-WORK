
#include "edusvm/preprocessor.hpp"
#include <cmath>

namespace edusvm {

bool preprocessDataset(
    const Dataset& input,
    Dataset& output,
    std::string& errorMessage
 ) {
    // 1. Check if empty
    if (input.features.empty()) { // Assuming Dataset is a vector-like container
        errorMessage = "Preprocessing failed: dataset is empty.";
        return false;
    }

    // 2. Check expected feature count
    // Assuming input.samples.front().features is a std::vector<double>
    std::size_t expectedFeatures = input.features[0].size();
    if (expectedFeatures == 0) {
        errorMessage = "Preprocessing failed: rows contain zero features.";
        return false;
    }

    // 3. Validate rows
    for (std::size_t i = 0; i < input.features.size(); ++i) {
        const auto& sample = input.features[i];

        // Check feature count
        if (sample.size() != expectedFeatures) {
            errorMessage = "Preprocessing failed: inconsistent feature count at row " + std::to_string(i) + ".";
            return false;
        }

        // Check for NaN or Infinity
        for (double val : sample) {
            if (!std::isfinite(val)) {
                errorMessage = "Preprocessing failed: non-finite value at row " + std::to_string(i) + ".";
                return false;
            }
        }

        // Check label
        if (input.labels[i] != 1 && input.labels[i] != -1) {
            errorMessage = "Preprocessing failed: invalid label at row " + std::to_string(i) + " (must be 1 or -1).";
            return false;
        }
    }

    // 4. Success! Copy input to output
    output = input;
    errorMessage = "";
    return true;
}

} // namespace edusvm