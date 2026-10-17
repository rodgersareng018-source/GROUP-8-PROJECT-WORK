//Test that valid data is processed correctly and invalid data is handled appropriately.

#include "edusvm/preprocessor.hpp"

#include <cassert>
#include <iostream>
#include <string>

int main() {
    std::string error;

    // Test 1: Valid data should be processed successfully.
    edusvm::Dataset input;
    input.features = {{2.0, 80.0}, {5.0, 95.0}};
    input.labels = {-1, 1};

    edusvm::Dataset output;

    assert(edusvm::preprocessDataset(input, output, error));
    assert(error.empty());

    // Check that the current preprocessor preserves the data.
    assert(output.features == input.features);
    assert(output.labels == input.labels);

    // Test 2: Invalid data should be rejected.
    edusvm::Dataset invalid;
    invalid.features = {{2.0, 80.0}, {5.0, 95.0}};
    invalid.labels = {1};

    assert(!edusvm::preprocessDataset(invalid, output, error));
    assert(!error.empty());

    std::cout << "Preprocessor tests passed.\n";
    return 0;
}
