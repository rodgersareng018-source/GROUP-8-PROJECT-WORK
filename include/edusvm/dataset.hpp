
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

// Loads numerical features and binary labels from a CSV file.
// The first row must contain column headings.
// The final column must contain labels (-1 or +1).
class DatasetLoader {
public:
    static bool loadFromCSV(
        const std::string& filePath,
        Dataset& dataset,
        std::string& errorMessage
    );
};

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
