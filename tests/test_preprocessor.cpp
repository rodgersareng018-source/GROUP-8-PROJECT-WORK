// Check successful preprocessing and rejection of malformed datasets.
#include "edusvm/dataset.hpp"
#include "edusvm/preprocessor.hpp"
#include <iostream>
#include <cassert>
#include <cmath> // For NAN and INFINITY constants

// A valid dataset should be copied to the output with no error.
void testValidDataset() {
    edusvm::Dataset input;
    input.features = {
        {55.0, 40.0, 12.0},
        {60.0, 45.0, 13.0}
    };
    input.labels = {-1, 1};

    edusvm::Dataset output;
    std::string error;

    bool result = edusvm::preprocessDataset(input, output, error);
    
    assert(result == true);
    assert(error == "");
    std::cout << "[PASS] testValidDataset" << std::endl;
}

// Every row must have the same number of feature values.
void testInconsistentFeatureCount() {
    edusvm::Dataset input;
    input.features = {
        {55.0, 40.0, 12.0}, // Row 0 has 3 features
        {60.0, 45.0}        // Row 1 only has 2 features (Inconsistent!)
    };
    input.labels = {-1, 1};

    edusvm::Dataset output;
    std::string error;

    bool result = edusvm::preprocessDataset(input, output, error);

    assert(result == false);
    // Check the error describes the row-shape problem.
    assert(error.find("inconsistent feature count") != std::string::npos);
    std::cout << "[PASS] testInconsistentFeatureCount" << std::endl;
}

// NaN and infinity are not usable numeric measurements.
void testNonFiniteValues() {
    edusvm::Dataset input;
    input.features = {
        {55.0, NAN, 12.0},   // Contains a NaN value
        {60.0, 45.0, 13.0}
    };
    input.labels = {-1, 1};

    edusvm::Dataset output;
    std::string error;

    bool result = edusvm::preprocessDataset(input, output, error);

    assert(result == false);
    assert(error.find("non-finite value") != std::string::npos);
    std::cout << "[PASS] testNonFiniteValues" << std::endl;
}

// The classifier accepts only -1 and +1 labels.
void testInvalidLabels() {
    edusvm::Dataset input;
    input.features = {
        {55.0, 40.0, 12.0},
        {60.0, 45.0, 13.0}
    };
    input.labels = {-1, 5}; // 5 is an invalid label (must be 1 or -1)

    edusvm::Dataset output;
    std::string error;

    bool result = edusvm::preprocessDataset(input, output, error);

    assert(result == false);
    assert(error.find("invalid label") != std::string::npos);
    std::cout << "[PASS] testInvalidLabels" << std::endl;
}

void testMismatchedLabels() {
    // The preprocessor must catch row/label count mismatches before indexing.
    edusvm::Dataset input;
    input.features = {{55.0, 40.0}, {60.0, 45.0}};
    input.labels = {-1};

    edusvm::Dataset output;
    std::string error;
    assert(!edusvm::preprocessDataset(input, output, error));
    assert(error.find("labels") != std::string::npos);
    std::cout << "[PASS] testMismatchedLabels" << std::endl;
}

int main() {
    // Run each validation case and report the overall result.

    testValidDataset();
    testInconsistentFeatureCount();
    testNonFiniteValues();
    testInvalidLabels();
    testMismatchedLabels();


    std::cout << "ALL PREPROCESSOR TESTS PASSED SUCCESSFULLY!" << std::endl;
    
    
    return 0;

}
