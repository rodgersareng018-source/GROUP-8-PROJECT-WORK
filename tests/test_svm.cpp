#include "edusvm/svm.hpp"
#include <cassert>
#include <cmath>
#include <iostream>

int main() {
    std::string error;
    edusvm::Hyperplane model;
    model.weights = {2.0, 1.0};
    model.bias = -3.0;
    
    double result = 0.0;
    
    // Test 1: Correct score
    // Score = 2*2 + 1*1 - 3 = 2
    assert(edusvm::decisionFunction(model, {2.0, 1.0}, result, error));
    assert(std::abs(result - 2.0) < 1e-9);
    
    // Test 2: Mismatched feature count
    assert(!edusvm::decisionFunction(model, {2.0}, result, error));
    assert(!error.empty());
    
    // Test 3: Invalid label
    double margin = 0.0;
    assert(!edusvm::calculateFunctionalMargin(model, {2.0, 1.0}, 0, margin, error));
    assert(!error.empty());
    
    // Test 4: Valid functional margin
    // Margin = 1 * 2 = 2
    assert(edusvm::calculateFunctionalMargin(model, {2.0, 1.0}, 1, margin, error));
    assert(std::abs(margin - 2.0) < 1e-9);
    
    std::cout << "SVM tests passed.\n";
    return 0;
}