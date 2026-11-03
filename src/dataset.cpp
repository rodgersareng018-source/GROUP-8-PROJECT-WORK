
#include "edusvm/dataset.hpp"

#include <cmath>

namespace edusvm {

bool validateDataset(
    const Dataset& dataset,
    std::string& errorMessage
) {
    // Clear any previous error message.
    errorMessage.clear();

    // Check that the dataset contains at least one student.
    if (dataset.features.empty()) {
        errorMessage = "Dataset contains no feature rows.";
        return false;
    }

    // Every student must have at least one feature.
    const std::size_t featureCount = dataset.features[0].size();

    if (featureCount == 0) {
        errorMessage = "Dataset contains no features.";
        return false;
    }

    // The number of labels must equal the number of students.
    if (dataset.features.size() != dataset.labels.size()) {
        errorMessage = "Number of labels does not match number of rows.";
        return false;
    }

    // Each student must have the same number of features.
    for (const auto& row : dataset.features) {
        if (row.size() != featureCount) {
            errorMessage = "Rows have inconsistent numbers of features.";
            return false;
        }

        // Feature values must be finite numbers.
        for (double value : row) {
            if (!std::isfinite(value)) {
                errorMessage = "Feature contains a non-finite value.";
                return false;
            }
        }
    }

    // EduSVM uses -1 and +1 as its binary SVM class labels.
    for (int label : dataset.labels) {
        if (label != -1 && label != 1) {
            errorMessage = "Labels must be -1 or +1.";
            return false;
        }
    }

    return true;
}

} // namespace edusvm
