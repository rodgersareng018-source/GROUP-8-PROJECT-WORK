#include "edusvm/dataset.hpp"
#include <cassert>
#include <iostream>
#include <string>

int main() {
    std::string error;
    
    // Test 1: Empty dataset must be rejected.
    edusvm::Dataset empty;
    assert(!edusvm::validateDataset(empty, error));
    assert(!error.empty());

    // Test 2: Valid dataset must be accepted.
    edusvm::Dataset valid;
    valid.features = {{2.0, 80.0}, {5.0, 95.0}};
    valid.labels = {-1, 1};
    assert(edusvm::validateDataset(valid, error));
    assert(error.empty());

    // Test 3: Number of labels does not match number of students.
    edusvm::Dataset mismatch;
    mismatch.features = {{2.0, 80.0}, {5.0, 95.0}};
    mismatch.labels = {1};
    assert(!edusvm::validateDataset(mismatch, error));
    assert(!error.empty());

    std::cout << "Basic dataset tests passed.\n";
    return 0;
}