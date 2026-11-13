
#ifndef EDUSVM_DATASET_HPP
#define EDUSVM_DATASET_HPP

#include <string>
#include <vector>

namespace edusvm {

// Features and labels at the same index belong to the same row.
struct Dataset {
    std::vector<std::vector<double>> features;
    std::vector<int> labels;

    bool empty() const {
        return features.empty();
    }
};

class DatasetLoader {
public:
    // Load a CSV file into a dataset and return an error message on failure.
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

bool loadDatasetCsv(
    const std::string& filePath,
    Dataset& dataset,
    std::string& errorMessage
);

} // namespace edusvm

#endif // EDUSVM_DATASET_HPP
