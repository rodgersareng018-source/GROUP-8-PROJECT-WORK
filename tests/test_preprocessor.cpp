#include "edusvm/dataset.hpp"
#include "edusvm/preprocessor.hpp"
#include "edusvm/preprocessor_strategies.hpp"

#include <iostream>
#include <cassert>
#include <cmath>
#include <limits>

void testValidDataset() {
    edusvm::Dataset input;
    input.features = {{55.0, 40.0, 12.0}, {60.0, 45.0, 13.0}};
    input.labels = {-1, 1};

    edusvm::Dataset output;
    std::string error;

    assert(edusvm::preprocessDataset(input, output, error));
    assert(error.empty());

    std::cout << "[PASS] testValidDataset\n";
}

void testInconsistentFeatureCount() {
    edusvm::Dataset input;
    input.features = {{55.0, 40.0, 12.0}, {60.0, 45.0}};
    input.labels = {-1, 1};

    edusvm::Dataset output;
    std::string error;

    assert(!edusvm::preprocessDataset(input, output, error));
    assert(error.find("inconsistent feature count") != std::string::npos);

    std::cout << "[PASS] testInconsistentFeatureCount\n";
}

void testNonFiniteValues() {
    edusvm::Dataset input;
    input.features = {{55.0, NAN, 12.0}, {60.0, 45.0, 13.0}};
    input.labels = {-1, 1};

    edusvm::Dataset output;
    std::string error;

    assert(!edusvm::preprocessDataset(input, output, error));
    assert(error.find("non-finite value") != std::string::npos);

    std::cout << "[PASS] testNonFiniteValues\n";
}

void testInvalidLabels() {
    edusvm::Dataset input;
    input.features = {{55.0, 40.0}, {60.0, 45.0}};
    input.labels = {-1, 5};

    edusvm::Dataset output;
    std::string error;

    assert(!edusvm::preprocessDataset(input, output, error));
    assert(error.find("invalid label") != std::string::npos);

    std::cout << "[PASS] testInvalidLabels\n";
}

void testMeanImputation() {
    edusvm::Dataset input;
    const double missing = std::numeric_limits<double>::quiet_NaN();

    input.features = {{2.0, 60.0}, {missing, 70.0}, {4.0, 80.0}};
    input.labels = {-1, -1, 1};

    edusvm::Dataset output;
    std::string error;

    edusvm::ImputationPreprocessor imputer(edusvm::ImputationStrategy::MEAN);

    assert(imputer.process(input, output, error));
    assert(output.features.size() == 3);
    assert(output.features[1][0] == 3.0);
    assert(output.labels == input.labels);

    std::cout << "[PASS] testMeanImputation\n";
}

void testDropRowImputation() {
    edusvm::Dataset input;
    const double missing = std::numeric_limits<double>::quiet_NaN();

    input.features = {{2.0, 60.0}, {missing, 70.0}, {4.0, 80.0}};
    input.labels = {-1, -1, 1};

    edusvm::Dataset output;
    std::string error;

    edusvm::ImputationPreprocessor imputer(edusvm::ImputationStrategy::DROP_ROW);

    assert(imputer.process(input, output, error));
    assert(output.features.size() == 2);
    assert(output.labels.size() == 2);
    assert(output.labels[0] == -1);
    assert(output.labels[1] == 1);

    std::cout << "[PASS] testDropRowImputation\n";
}

void testAllMissingColumn() {
    edusvm::Dataset input;
    const double missing = std::numeric_limits<double>::quiet_NaN();

    input.features = {{missing, 60.0}, {missing, 70.0}};
    input.labels = {-1, 1};

    edusvm::Dataset output;
    std::string error;

    edusvm::ImputationPreprocessor imputer(edusvm::ImputationStrategy::MEAN);

    assert(!imputer.process(input, output, error));
    assert(error.find("only missing values") != std::string::npos);

    std::cout << "[PASS] testAllMissingColumn\n";
}

void testMismatchedLabelCount() {
    edusvm::Dataset input;
    input.features = {{2.0, 60.0}, {4.0, 80.0}};
    input.labels = {-1};

    edusvm::Dataset output;
    std::string error;

    assert(!edusvm::preprocessDataset(input, output, error));
    assert(error.find("counts do not match") != std::string::npos);

    std::cout << "[PASS] testMismatchedLabelCount\n";
}

int main() {
    testValidDataset();
    testInconsistentFeatureCount();
    testNonFiniteValues();
    testInvalidLabels();
    testMeanImputation();
    testDropRowImputation();
    testAllMissingColumn();
    testMismatchedLabelCount();

    std::cout << "ALL PREPROCESSOR TESTS PASSED SUCCESSFULLY!\n";
    return 0;
}
