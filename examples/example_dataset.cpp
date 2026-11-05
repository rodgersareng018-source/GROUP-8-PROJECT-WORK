// examples/example_dataset.cpp

// Small example showing how to build an edusvm::Dataset by hand and
// validate it with edusvm::validateDataset.

// NOTE: CSV loading is still unfinished in EduSVM. This example does NOT
// call any CSV-loading function; the sample data is written directly in code.
// Once a loader exists, replace buildSampleDataset() with a call to it.

#include "edusvm/dataset.hpp"

#include <iostream>
#include <string>

namespace {

// Each row is one student: {study hours per week, attendance percentage}.
// Label +1 = passed, -1 = failed.
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
    // 1. A well-formed dataset.
    edusvm::Dataset good = buildSampleDataset();
    report("good dataset", good);

    // 2. Break it on purpose: 0 is not a valid SVM label.
    edusvm::Dataset badLabel = buildSampleDataset();
    badLabel.labels[2] = 0;
    report("bad label dataset", badLabel);

    // 3. Break it differently: one row has a missing feature.
    edusvm::Dataset jagged = buildSampleDataset();
    jagged.features[4].pop_back();
    report("jagged dataset", jagged);

    return 0;
}
