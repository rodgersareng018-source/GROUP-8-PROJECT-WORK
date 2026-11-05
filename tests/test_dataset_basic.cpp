#include "edusvm/dataset.hpp"
#include <cassert>
#include <iostream>
#include <string>

int main() {
    std::string error;
    
    // Empty datasets cannot provide a feature count or training examples.
    edusvm::Dataset empty;
    assert(!edusvm::validateDataset(empty, error));
    assert(!error.empty());

    // A rectangular matrix with one valid label per row is accepted.
    edusvm::Dataset valid;
    valid.features = {{2.0, 80.0}, {5.0, 95.0}};
    valid.labels = {-1, 1};
    assert(edusvm::validateDataset(valid, error));
    assert(error.empty());

    // Labels and feature rows must stay aligned one-to-one.
    edusvm::Dataset mismatch;
    mismatch.features = {{2.0, 80.0}, {5.0, 95.0}};
    mismatch.labels = {1};
    assert(!edusvm::validateDataset(mismatch, error));
    assert(!error.empty());

    std::cout << "Basic dataset tests passed.\n";
    return 0;
}