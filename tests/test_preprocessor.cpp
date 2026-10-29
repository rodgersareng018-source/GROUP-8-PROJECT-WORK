//Test that valid data is processed correctly and invalid data is handled appropriately.
#include "edusvm/dataset.hpp"
#include "edusvm/preprocessor.hpp"
#include <iostream>
#include <cassert>
#include <cmath> // For NAN and INFINITY constants

// Test 1: A completely valid dataset should succeed
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

// Test 2: Inconsistent feature counts should trigger the row size check
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
    // Verifies it catches the exact error message string layout she wrote
    assert(error.find("inconsistent feature count") != std::string::npos);
    std::cout << "[PASS] testInconsistentFeatureCount" << std::endl;
}

// Test 3: Non-finite values (like NaN or Inf) should be caught
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

// Test 4: Labels other than 1 and -1 should fail
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

int main() {
    //Running all tests

    testValidDataset();
    testInconsistentFeatureCount();
    testNonFiniteValues();
    testInvalidLabels();


    std::cout << "ALL PREPROCESSOR TESTS PASSED SUCCESSFULLY!" << std::endl;
    
    
    return 0;

}