#include "edusvm/svm.hpp"
#include <iostream>
#include <cassert>
#include <cmath>
#include <string>

int main() {
    // Shared error string is populated when an API call rejects its inputs.
    std::string error;

    // The dot product is 2*2 + 1*1, then adding bias -3 gives score 2.
    edusvm::Hyperplane model{{2.0, 1.0}, -3.0};
    double result = 0.0;
    assert(edusvm::decisionFunction(model, {2.0, 1.0}, result, error));
    assert(std::abs(result - 2.0) < 1e-9);

    // A zero feature vector leaves only the model bias, which is negative.
    assert(edusvm::decisionFunction(model, {0.0, 0.0}, result, error));
    assert(std::abs(result - (-3.0)) < 1e-9);

    // A score of zero lies exactly on the decision boundary.
    edusvm::Hyperplane zeroModel{{1.0, 1.0}, -2.0};
    assert(edusvm::decisionFunction(zeroModel, {1.0, 1.0}, result, error));
    assert(std::abs(result - 0.0) < 1e-9);

    // A model and prediction row must have equal feature counts.
    edusvm::Hyperplane badModel{{2.0}, 0.0};
    assert(!edusvm::decisionFunction(badModel, {2.0, 1.0}, result, error));
    assert(!error.empty());

    // Margins are defined only for the two supported labels.
    double margin = 0.0;
    assert(!edusvm::calculateFunctionalMargin(model, {2.0, 1.0}, 0, margin, error));
    assert(!error.empty());

    // For a positive label, the functional margin equals the raw score.
    assert(edusvm::calculateFunctionalMargin(model, {2.0, 1.0}, 1, margin, error));
    assert(std::abs(margin - 2.0) < 1e-9);

    // Train on a simple linearly separable dataset with two classes.
    edusvm::Dataset training;
    training.features = {{-2.0}, {-1.0}, {1.0}, {2.0}};
    training.labels = {-1, -1, 1, 1};
    edusvm::TrainingOptions options;
    options.epochs = 500;
    edusvm::Hyperplane trained;
    assert(edusvm::trainSvm(training, options, trained, error));
    assert(!trained.supportVectorIndices.empty());

    // Check that unseen points on each side get the expected class.
    int prediction = 0;
    assert(edusvm::predictLabel(trained, {-1.5}, prediction, error));
    assert(prediction == -1);
    assert(edusvm::predictLabel(trained, {1.5}, prediction, error));
    assert(prediction == 1);

    // The training examples are separable, so all four outcomes are correct.
    edusvm::EvaluationMetrics metrics;
    assert(edusvm::evaluateModel(trained, training, metrics, error));
    assert(metrics.truePositives == 2);
    assert(metrics.trueNegatives == 2);
    assert(metrics.falsePositives == 0);
    assert(metrics.falseNegatives == 0);
    assert(std::abs(metrics.accuracy - 1.0) < 1e-9);
    assert(std::abs(metrics.precision - 1.0) < 1e-9);
    assert(std::abs(metrics.recall - 1.0) < 1e-9);
    assert(std::abs(metrics.f1Score - 1.0) < 1e-9);

    // Training must fail clearly when one of the classes is absent.
    edusvm::Dataset oneClass;
    oneClass.features = {{1.0}, {2.0}};
    oneClass.labels = {1, 1};
    assert(!edusvm::trainSvm(oneClass, options, trained, error));
    assert(!error.empty());

    // Cross-validation should produce exactly one held-out prediction per row.
    edusvm::Dataset crossValidationData;
    crossValidationData.features = {
        {1.0, 40.0}, {2.0, 50.0}, {3.0, 55.0},
        {6.0, 75.0}, {7.0, 85.0}, {8.0, 90.0}
    };
    crossValidationData.labels = {-1, -1, -1, 1, 1, 1};
    edusvm::EvaluationMetrics crossValidationMetrics;
    assert(edusvm::crossValidateSvm(
        crossValidationData, 3, options, crossValidationMetrics, error
    ));
    assert(crossValidationMetrics.truePositives +
        crossValidationMetrics.falsePositives +
        crossValidationMetrics.trueNegatives +
        crossValidationMetrics.falseNegatives == 6);
    assert(crossValidationMetrics.accuracy >= 0.0);
    assert(crossValidationMetrics.accuracy <= 1.0);

    assert(!edusvm::crossValidateSvm(
        crossValidationData, 4, options, crossValidationMetrics, error
    ));
    assert(!error.empty());

    return 0;
}