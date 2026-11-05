#include "edusvm/evaluation.hpp"
#include "edusvm/svm.hpp"

#include <cassert>
#include <cmath>
#include <iostream>
#include <string>

void testEvaluationMetrics() {
    edusvm::Hyperplane model{{1.0}, 0.0};

    edusvm::Dataset data;
    data.features = {{2.0}, {-2.0}, {1.0}, {-1.0}};
    data.labels = {1, -1, -1, 1};

    edusvm::EvaluationMetrics metrics;
    std::string error;

    assert(edusvm::evaluateModel(model, data, metrics, error));

    assert(metrics.truePositives == 1);
    assert(metrics.trueNegatives == 1);
    assert(metrics.falsePositives == 1);
    assert(metrics.falseNegatives == 1);

    assert(std::abs(metrics.accuracy - 0.5) < 1e-9);
    assert(std::abs(metrics.precision - 0.5) < 1e-9);
    assert(std::abs(metrics.recall - 0.5) < 1e-9);
    assert(std::abs(metrics.f1Score - 0.5) < 1e-9);

    std::cout << "[PASS] testEvaluationMetrics\n";
}

void testPerfectEvaluation() {
    edusvm::Hyperplane model{{1.0}, 0.0};

    edusvm::Dataset data;
    data.features = {{2.0}, {-2.0}};
    data.labels = {1, -1};

    edusvm::EvaluationMetrics metrics;
    std::string error;

    assert(edusvm::evaluateModel(model, data, metrics, error));

    assert(metrics.accuracy == 1.0);
    assert(metrics.precision == 1.0);
    assert(metrics.recall == 1.0);
    assert(metrics.f1Score == 1.0);

    std::cout << "[PASS] testPerfectEvaluation\n";
}

void testEmptyDataset() {
    edusvm::Hyperplane model{{1.0}, 0.0};

    edusvm::Dataset data;
    edusvm::EvaluationMetrics metrics;
    std::string error;

    assert(!edusvm::evaluateModel(model, data, metrics, error));
    assert(!error.empty());

    std::cout << "[PASS] testEmptyDataset\n";
}

void testMismatchedDataset() {
    edusvm::Hyperplane model{{1.0}, 0.0};

    edusvm::Dataset data;
    data.features = {{2.0}, {-2.0}};
    data.labels = {1};

    edusvm::EvaluationMetrics metrics;
    std::string error;

    assert(!edusvm::evaluateModel(model, data, metrics, error));
    assert(!error.empty());

    std::cout << "[PASS] testMismatchedDataset\n";
}

void testInvalidLabel() {
    edusvm::Hyperplane model{{1.0}, 0.0};

    edusvm::Dataset data;
    data.features = {{2.0}};
    data.labels = {0};

    edusvm::EvaluationMetrics metrics;
    std::string error;

    assert(!edusvm::evaluateModel(model, data, metrics, error));
    assert(!error.empty());

    std::cout << "[PASS] testInvalidLabel\n";
}

void testInvalidModelDimensions() {
    edusvm::Hyperplane model{{1.0, 2.0}, 0.0};

    edusvm::Dataset data;
    data.features = {{2.0}};
    data.labels = {1};

    edusvm::EvaluationMetrics metrics;
    std::string error;

    assert(!edusvm::evaluateModel(model, data, metrics, error));
    assert(!error.empty());

    std::cout << "[PASS] testInvalidModelDimensions\n";
}

int main() {
    testEvaluationMetrics();
    testPerfectEvaluation();
    testEmptyDataset();
    testMismatchedDataset();
    testInvalidLabel();
    testInvalidModelDimensions();

    std::cout << "ALL EVALUATION TESTS PASSED!\n";
    return 0;
}
