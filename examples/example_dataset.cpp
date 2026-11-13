#include "edusvm/dataset.hpp"

#include <iostream>
#include <string>

namespace {

edusvm::Dataset buildSampleDataset() {
    edusvm::Dataset dataset;

    dataset.features = {
        {12.0, 95.0},
        {10.5, 90.0},
        {14.0, 98.0},
        {2.0, 55.0},
        {3.5, 60.0},
        {1.0, 40.0},
    };

    dataset.labels = {1, 1, 1, -1, -1, -1};

    return dataset;
}

void report(const std::string& name, const edusvm::Dataset& dataset) {
    std::string error;

    if (edusvm::validateDataset(dataset, error)) {
        std::cout << name << ": valid ("
                  << dataset.features.size() << " students, "
                  << dataset.features[0].size() << " features each)\n";
    } else {
        std::cout << name << ": INVALID - " << error << '\n';
    }
}

} // namespace

int main() {
    edusvm::Dataset validData = buildSampleDataset();
    report("good dataset", validData);

    edusvm::Dataset invalidLabelData = buildSampleDataset();
    invalidLabelData.labels[2] = 0;
    report("bad label dataset", invalidLabelData);

    edusvm::Dataset unevenRowsData = buildSampleDataset();
    unevenRowsData.features[4].pop_back();
    report("jagged dataset", unevenRowsData);

    return 0;
}
