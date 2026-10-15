#ifndef EDUSVM_DATASET_HPP
#define EDUSVM_DATASET_HPP

#include <string>
#include <vector>

namespace edusvm {

struct Dataset {
    std::vector<std::vector<double>> features;
    std::vector<int> labels;
};

bool validateDataset(
    const Dataset& dataset,
    std::string& errorMessage
);

} // namespace edusvm

#endif // EDUSVM_DATASET_HPP

