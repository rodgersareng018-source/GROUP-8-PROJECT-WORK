#include "edusvm/svm.hpp"

#include <cassert>
#include <cmath>
#include <string>

namespace {

class TestCase {
public:
    virtual ~TestCase() = default;
    virtual void run() const = 0;
};

class DecisionFunctionTest final : public TestCase {
public:
    void run() const override {
        std::string error;
        double result = 0.0;
        const edusvm::Hyperplane model{{2.0, 1.0}, -3.0};

        assert(edusvm::decisionFunction(model, {2.0, 1.0}, result, error));
        assert(std::abs(result - 2.0) < 1e-9);

        assert(edusvm::decisionFunction(model, {0.0, 0.0}, result, error));
        assert(std::abs(result + 3.0) < 1e-9);

        const edusvm::Hyperplane zeroModel{{1.0, 1.0}, -2.0};
        assert(edusvm::decisionFunction(zeroModel, {1.0, 1.0}, result, error));
        assert(std::abs(result) < 1e-9);
    }
};

class InvalidDecisionAndMarginInputsTest final : public TestCase {
public:
    void run() const override {
        std::string error;
        double result = 0.0;
        const edusvm::Hyperplane model{{2.0, 1.0}, -3.0};
        const edusvm::Hyperplane wrongSizeModel{{2.0}, 0.0};

        assert(!edusvm::decisionFunction(
            wrongSizeModel, {2.0, 1.0}, result, error
        ));
        assert(!error.empty());

        double margin = 0.0;
        assert(!edusvm::calculateFunctionalMargin(
            model, {2.0, 1.0}, 0, margin, error
        ));
        assert(!error.empty());

        assert(edusvm::calculateFunctionalMargin(
            model, {2.0, 1.0}, 1, margin, error
        ));
        assert(std::abs(margin - 2.0) < 1e-9);
    }
};

class TrainingPredictionAndEvaluationTest final : public TestCase {
public:
    void run() const override {
        std::string error;
        edusvm::Dataset training;
        training.features = {{-2.0}, {-1.0}, {1.0}, {2.0}};
        training.labels = {-1, -1, 1, 1};

        edusvm::TrainingOptions options;
        options.epochs = 500;
        edusvm::Hyperplane model;
        assert(edusvm::trainSvm(training, options, model, error));
        assert(!model.supportVectorIndices.empty());

        int prediction = 0;
        assert(edusvm::predictLabel(model, {-1.5}, prediction, error));
        assert(prediction == -1);
        assert(edusvm::predictLabel(model, {1.5}, prediction, error));
        assert(prediction == 1);

        edusvm::EvaluationMetrics metrics;
        assert(edusvm::evaluateModel(model, training, metrics, error));
        assert(metrics.truePositives == 2);
        assert(metrics.trueNegatives == 2);
        assert(metrics.falsePositives == 0);
        assert(metrics.falseNegatives == 0);
        assert(std::abs(metrics.accuracy - 1.0) < 1e-9);
        assert(std::abs(metrics.precision - 1.0) < 1e-9);
        assert(std::abs(metrics.recall - 1.0) < 1e-9);
        assert(std::abs(metrics.f1Score - 1.0) < 1e-9);

        edusvm::Dataset oneClass;
        oneClass.features = {{1.0}, {2.0}};
        oneClass.labels = {1, 1};
        assert(!edusvm::trainSvm(oneClass, options, model, error));
        assert(!error.empty());
    }
};

class CrossValidationTest final : public TestCase {
public:
    void run() const override {
        std::string error;
        edusvm::Dataset data;
        data.features = {
            {1.0, 40.0}, {2.0, 50.0}, {3.0, 55.0},
            {6.0, 75.0}, {7.0, 85.0}, {8.0, 90.0}
        };
        data.labels = {-1, -1, -1, 1, 1, 1};

        edusvm::EvaluationMetrics metrics;
        assert(edusvm::crossValidateSvm(
            data, 3, edusvm::TrainingOptions{}, metrics, error
        ));
        assert(metrics.truePositives + metrics.falsePositives +
            metrics.trueNegatives + metrics.falseNegatives == data.labels.size());
        assert(metrics.accuracy >= 0.0);
        assert(metrics.accuracy <= 1.0);

        assert(!edusvm::crossValidateSvm(
            data, 4, edusvm::TrainingOptions{}, metrics, error
        ));
        assert(!error.empty());
    }
};

} // namespace

int main() {
    DecisionFunctionTest decisionTest;
    InvalidDecisionAndMarginInputsTest invalidInputsTest;
    TrainingPredictionAndEvaluationTest trainingTest;
    CrossValidationTest crossValidationTest;
    TestCase* tests[] = {
        &decisionTest,
        &invalidInputsTest,
        &trainingTest,
        &crossValidationTest
    };

    for (const auto* test : tests) {
        test->run();
    }

    return 0;
}
