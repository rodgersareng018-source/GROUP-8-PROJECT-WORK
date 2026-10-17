#include "edusvm/svm.hpp"
#include <cassert>
#include <cmath>
#include <string>

int main() {
    std::string error;

    // Test 1: Correct score
    edusvm::Hyperplane model{{2.0, 1.0}, -3.0};
    double result = 0.0;
    assert(edusvm::decisionFunction(model, {2.0, 1.0}, result, error));
    assert(std::abs(result - 2.0) < 1e-9);

    // Test 2: Mismatched feature count must be rejected
    edusvm::Hyperplane badModel{{2.0}, 0.0};
    assert(!edusvm::decisionFunction(badModel, {2.0, 1.0}, result, error));
    assert(!error.empty());

    // Test 3: Invalid label must be rejected
    double margin = 0.0;
    assert(!edusvm::calculateFunctionalMargin(model, {2.0, 1.0}, 0, margin, error));
    assert(!error.empty());

    // Test 4: Functional margin with valid label
    assert(edusvm::calculateFunctionalMargin(model, {2.0, 1.0}, 1, margin, error));
    assert(std::abs(margin - 2.0) < 1e-9);

    return 0;
}