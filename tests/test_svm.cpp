#include "edusvm/svm.hpp"

#include <cassert>
#include <cmath>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

void testDecisionFunction() {
    std::string error;
    edusvm::Hyperplane model{{2.0, 1.0}, -3.0};
    double result = 0.0;

    assert(edusvm::decisionFunction(model, {2.0, 1.0}, result, error));
    assert(std::abs(result - 2.0) < 1e-9);

    assert(edusvm::decisionFunction(model, {0.0, 0.0}, result, error));
    assert(std::abs(result + 3.0) < 1e-9);

    edusvm::Hyperplane zeroModel{{1.0, 1.0}, -2.0};
    assert(edusvm::decisionFunction(zeroModel, {1.0, 1.0}, result, error));
    assert(std::abs(result) < 1e-9);

    std::cout << "[PASS] testDecisionFunction\n";
}

void testInvalidDecisionFunctionInput() {
    std::string error;
    double result = 0.0;

    edusvm::Hyperplane badModel{{2.0}, 0.0};

    assert(!edusvm::decisionFunction(
        badModel, {2.0, 1.0}, result, error
    ));

    assert(!error.empty());

    std::cout << "[PASS] testInvalidDecisionFunctionInput\n";
}

void testFunctionalMargin() {
    std::string error;
    double margin = 0.0;

    edusvm::Hyperplane model{{2.0, 1.0}, -3.0};

    assert(!edusvm::calculateFunctionalMargin(
        model, {2.0, 1.0}, 0, margin, error
    ));

    assert(edusvm::calculateFunctionalMargin(
        model, {2.0, 1.0}, 1, margin, error
    ));

    assert(std::abs(margin - 2.0) < 1e-9);

    std::cout << "[PASS] testFunctionalMargin\n";
}

void testPrediction() {
    std::string error;
    int prediction = 0;

    edusvm::Hyperplane model{{2.0, 1.0}, -3.0};

    assert(edusvm::predict(model, {2.0, 1.0}, prediction, error));
    assert(prediction == 1);

    assert(edusvm::predict(model, {0.0, 0.0}, prediction, error));
    assert(prediction == -1);

    std::cout << "[PASS] testPrediction\n";
}

void testPredictionAtZero() {
    std::string error;
    int prediction = 0;

    edusvm::Hyperplane model{{1.0, 1.0}, -2.0};

    assert(edusvm::predict(model, {1.0, 1.0}, prediction, error));
    assert(prediction == 1);

    std::cout << "[PASS] testPredictionAtZero\n";
}

void testSupportVectorIdentification() {
    std::string error;
    std::vector<std::size_t> indices;

    edusvm::Hyperplane model{{1.0}, 0.0};

    edusvm::Dataset data;
    data.features = {{2.0}, {1.0}, {0.5}, {-2.0}};
    data.labels = {1, 1, 1, -1};

    assert(edusvm::identifySupportVectors(
        model, data, indices, 1e-9, error
    ));

    assert(indices.size() == 2);
    assert(indices[0] == 1);
    assert(indices[1] == 2);

    std::cout << "[PASS] testSupportVectorIdentification\n";
}

void testInvalidSupportVectorInput() {
    std::string error;
    std::vector<std::size_t> indices;

    edusvm::Hyperplane model{{1.0}, 0.0};

    edusvm::Dataset data;
    data.features = {{1.0}, {2.0}};
    data.labels = {1};

    assert(!edusvm::identifySupportVectors(
        model, data, indices, 1e-9, error
    ));

    assert(!error.empty());

    std::cout << "[PASS] testInvalidSupportVectorInput\n";
}

void testInvalidTolerance() {
    std::string error;
    std::vector<std::size_t> indices;

    edusvm::Hyperplane model{{1.0}, 0.0};

    edusvm::Dataset data;
    data.features = {{1.0}};
    data.labels = {1};

    assert(!edusvm::identifySupportVectors(
        model, data, indices,
        std::numeric_limits<double>::quiet_NaN(), error
    ));

    assert(!error.empty());

    std::cout << "[PASS] testInvalidTolerance\n";
}

edusvm::Dataset makeTrainingDataset() {
    edusvm::Dataset data;

    data.features = {
        {-2.0},
        {-1.0},
        {1.0},
        {2.0}
    };

    data.labels = {-1, -1, 1, 1};

    return data;
}

void testSVMTraining() {
    std::string error;

    edusvm::Dataset data = makeTrainingDataset();
    edusvm::SVMTrainingOptions options;
    edusvm::Hyperplane model;

    assert(edusvm::trainSVM(data, options, model, error));

    assert(model.weights.size() == 1);
    assert(std::isfinite(model.weights[0]));
    assert(std::isfinite(model.bias));

    int prediction = 0;

    assert(edusvm::predict(model, {-2.0}, prediction, error));
    assert(prediction == -1);

    assert(edusvm::predict(model, {2.0}, prediction, error));
    assert(prediction == 1);

    std::cout << "[PASS] testSVMTraining\n";
}

void testInvalidTrainingOptions() {
    std::string error;

    edusvm::Dataset data = makeTrainingDataset();
    edusvm::SVMTrainingOptions options;
    edusvm::Hyperplane model;

    options.learningRate = -0.01;

    assert(!edusvm::trainSVM(data, options, model, error));
    assert(!error.empty());

    std::cout << "[PASS] testInvalidTrainingOptions\n";
}

void testSingleClassRejected() {
    std::string error;

    edusvm::Dataset data;
    data.features = {{1.0}, {2.0}};
    data.labels = {1, 1};

    edusvm::SVMTrainingOptions options;
    edusvm::Hyperplane model;

    assert(!edusvm::trainSVM(data, options, model, error));
    assert(!error.empty());

    std::cout << "[PASS] testSingleClassRejected\n";
}

void testEmptyTrainingDataset() {
    std::string error;

    edusvm::Dataset data;
    edusvm::SVMTrainingOptions options;
    edusvm::Hyperplane model;

    assert(!edusvm::trainSVM(data, options, model, error));
    assert(!error.empty());

    std::cout << "[PASS] testEmptyTrainingDataset\n";
}

int main() {
    testDecisionFunction();
    testInvalidDecisionFunctionInput();
    testFunctionalMargin();
    testPrediction();
    testPredictionAtZero();
    testSupportVectorIdentification();
    testInvalidSupportVectorInput();
    testInvalidTolerance();
    testSVMTraining();
    testInvalidTrainingOptions();
    testSingleClassRejected();
    testEmptyTrainingDataset();

    std::cout << "ALL SVM TESTS PASSED SUCCESSFULLY!\n";
    return 0;
}
