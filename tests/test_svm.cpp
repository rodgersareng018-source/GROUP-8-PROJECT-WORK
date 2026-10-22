#include "edusvm/svm.hpp"
#include <iostream>
#include <cassert>
#include <cmath>
#include <string>

int main() {
    std::string error;

    // Test 1: Correct positive score
    edusvm::Hyperplane model{{2.0, 1.0}, -3.0};
    double result = 0.0;
    assert(edusvm::decisionFunction(model, {2.0, 1.0}, result, error));
    assert(std::abs(result - 2.0) < 1e-9);

    // Test 2: Negative score
    assert(edusvm::decisionFunction(model, {0.0, 0.0}, result, error));
    assert(std::abs(result - (-3.0)) < 1e-9);

    // Test 3: Zero score
    edusvm::Hyperplane zeroModel{{1.0, 1.0}, -2.0};
    assert(edusvm::decisionFunction(zeroModel, {1.0, 1.0}, result, error));
    assert(std::abs(result - 0.0) < 1e-9);

    // Test 4: Mismatched feature count must be rejected
    edusvm::Hyperplane badModel{{2.0}, 0.0};
    assert(!edusvm::decisionFunction(badModel, {2.0, 1.0}, result, error));
    assert(!error.empty());

    // Test 5: Invalid label must be rejected
    double margin = 0.0;
    assert(!edusvm::calculateFunctionalMargin(model, {2.0, 1.0}, 0, margin, error));
    assert(!error.empty());

    // Test 6: Functional margin with valid label
    assert(edusvm::calculateFunctionalMargin(model, {2.0, 1.0}, 1, margin, error));
    assert(std::abs(margin - 2.0) < 1e-9);

    return 0;
}
