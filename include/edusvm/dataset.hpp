#ifndef EDUSVM_DATASET_HPP
#define EDUSVM_DATASET_HPP

#include <string>
#include <vector>

namespace edusvm {

// One feature row and one binary label correspond to each student.
struct Dataset {
    std::vector<std::vector<double>> features;
    std::vector<int> labels;
    bool empty() const {
        return features.empty();
    }
};

// Check the dataset's row counts, feature shapes, finite values, and labels.
// Returns false and describes the first problem in errorMessage.
bool validateDataset(
    const Dataset& dataset,
    std::string& errorMessage
);

// Read a CSV whose final column is named "label"; replace dataset only on success.
bool loadDatasetCsv(
    const std::string& filePath,
    Dataset& dataset,
    std::string& errorMessage
);

} // namespace edusvm

#endif // EDUSVM_DATASET_HPP
