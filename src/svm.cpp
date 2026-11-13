#include "edusvm/svm.hpp"

#include "edusvm/scaler.hpp"

#include <algorithm>
#include <cmath>
#include <random>
#include <utility>

namespace edusvm {

namespace {

void finalizeMetrics(EvaluationMetrics& metrics) {
    const std::size_t total = metrics.truePositives + metrics.falsePositives +
        metrics.trueNegatives + metrics.falseNegatives;
    metrics.accuracy = static_cast<double>(
        metrics.truePositives + metrics.trueNegatives
    ) / static_cast<double>(total);

    const std::size_t predictedPositive =
        metrics.truePositives + metrics.falsePositives;
    if (predictedPositive != 0) {
        metrics.precision = static_cast<double>(metrics.truePositives) /
            static_cast<double>(predictedPositive);
    }

    const std::size_t actualPositive =
        metrics.truePositives + metrics.falseNegatives;
    if (actualPositive != 0) {
        metrics.recall = static_cast<double>(metrics.truePositives) /
            static_cast<double>(actualPositive);
    }

    const double precisionRecallSum = metrics.precision + metrics.recall;
    if (precisionRecallSum > 0.0) {
        metrics.f1Score = 2.0 * metrics.precision * metrics.recall /
            precisionRecallSum;
    }
}

void addMetrics(EvaluationMetrics& destination, const EvaluationMetrics& source) {
    destination.truePositives += source.truePositives;
    destination.falsePositives += source.falsePositives;
    destination.trueNegatives += source.trueNegatives;
    destination.falseNegatives += source.falseNegatives;
}

class SvmTrainer {
public:
    bool train(
        const Dataset& trainingData,
        const TrainingOptions& options,
        Hyperplane& model,
        std::string& errorMessage
    ) const {
        if (!validateDataset(trainingData, errorMessage)) {
            errorMessage = "Invalid training dataset: " + errorMessage;
            return false;
        }
        if (!std::isfinite(options.lambda) || options.lambda < 0.0) {
            errorMessage = "Lambda must be finite and non-negative.";
            return false;
        }
        if (!std::isfinite(options.learningRate) ||
            options.learningRate <= 0.0) {
            errorMessage = "Learning rate must be finite and positive.";
            return false;
        }
        if (options.epochs == 0) {
            errorMessage = "Training epochs must be greater than zero.";
            return false;
        }

        bool hasNegative = false;
        bool hasPositive = false;
        for (int label : trainingData.labels) {
            hasNegative = hasNegative || label == -1;
            hasPositive = hasPositive || label == 1;
        }
        if (!hasNegative || !hasPositive) {
            errorMessage = "Training data must contain both labels (-1 and +1).";
            return false;
        }

        Hyperplane trained;
        trained.weights.assign(trainingData.features.front().size(), 0.0);
        const double sampleCount =
            static_cast<double>(trainingData.features.size());

        // Update the weights and bias using the hinge-loss gradient each epoch.
        for (std::size_t epoch = 0; epoch < options.epochs; ++epoch) {
            const double stepSize =
                options.learningRate / std::sqrt(static_cast<double>(epoch + 1));
            std::vector<double> weightGradient = trained.weights;
            for (double& gradient : weightGradient) {
                gradient *= options.lambda;
            }
            double biasGradient = 0.0;

            for (std::size_t row = 0; row < trainingData.features.size(); ++row) {
                double score = trained.bias;
                for (std::size_t column = 0;
                     column < trained.weights.size(); ++column) {
                    score += trained.weights[column] *
                        trainingData.features[row][column];
                }
                if (!std::isfinite(score)) {
                    errorMessage = "Training produced a non-finite decision score.";
                    return false;
                }

                const int label = trainingData.labels[row];
                if (label * score < 1.0) {
                    for (std::size_t column = 0;
                         column < weightGradient.size(); ++column) {
                        weightGradient[column] -=
                            label * trainingData.features[row][column] /
                            sampleCount;
                    }
                    biasGradient -= label / sampleCount;
                }
            }

            for (std::size_t column = 0;
                 column < trained.weights.size(); ++column) {
                trained.weights[column] -=
                    stepSize * weightGradient[column];
                if (!std::isfinite(trained.weights[column])) {
                    errorMessage = "Training produced a non-finite model weight.";
                    return false;
                }
            }
            trained.bias -= stepSize * biasGradient;
            if (!std::isfinite(trained.bias)) {
                errorMessage = "Training produced a non-finite model bias.";
                return false;
            }
        }

        for (std::size_t row = 0; row < trainingData.features.size(); ++row) {
            double score = trained.bias;
            for (std::size_t column = 0;
                 column < trained.weights.size(); ++column) {
                score += trained.weights[column] *
                    trainingData.features[row][column];
            }
            if (trainingData.labels[row] * score <= 1.01) {
                trained.supportVectorIndices.push_back(row);
            }
        }

        model = std::move(trained);
        errorMessage.clear();
        return true;
    }
};

class DecisionEvaluator {
public:
    bool evaluate(
        const Hyperplane& model,
        const std::vector<double>& features,
        double& result,
        std::string& errorMessage
    ) const {
        if (model.weights.empty() || model.weights.size() != features.size()) {
            errorMessage = "Feature count does not match model weights.";
            return false;
        }

        if (!std::isfinite(model.bias)) {
            errorMessage = "Model bias must be finite.";
            return false;
        }

        double sum = model.bias;

        for (std::size_t i = 0; i < features.size(); ++i) {
            if (!std::isfinite(features[i]) ||
                !std::isfinite(model.weights[i])) {
                errorMessage = "Features and weights must be finite.";
                return false;
            }

            sum += model.weights[i] * features[i];

            if (!std::isfinite(sum)) {
                errorMessage = "Decision result is not finite.";
                return false;
            }
        }

        result = sum;
        errorMessage.clear();
        return true;
    }
};

class MarginCalculator {
public:
    bool calculate(
        const Hyperplane& model,
        const std::vector<double>& features,
        int label,
        double& margin,
        std::string& errorMessage
    ) const {
        if (label != 1 && label != -1) {
            errorMessage = "Label must be 1 or -1.";
            return false;
        }

        double result = 0.0;
        DecisionEvaluator evaluator;
        if (!evaluator.evaluate(model, features, result, errorMessage)) {
            return false;
        }

        margin = label * result;

        if (!std::isfinite(margin)) {
            errorMessage = "Functional margin is not finite.";
            return false;
        }

        errorMessage.clear();
        return true;
    }
};

class LabelPredictor {
public:
    bool predict(
        const Hyperplane& model,
        const std::vector<double>& features,
        int& prediction,
        std::string& errorMessage
    ) const {
        double score = 0.0;
        DecisionEvaluator evaluator;
        if (!evaluator.evaluate(model, features, score, errorMessage)) {
            return false;
        }
        prediction = score >= 0.0 ? 1 : -1;
        errorMessage.clear();
        return true;
    }
};

class CrossValidator {
public:
    bool validate(
        const Dataset& data,
        std::size_t foldCount,
        const TrainingOptions& options,
        EvaluationMetrics& metrics,
        std::string& errorMessage
    ) const {
        if (!validateDataset(data, errorMessage)) {
            errorMessage = "Invalid cross-validation dataset: " + errorMessage;
            return false;
        }
        if (foldCount < 2) {
            errorMessage = "Cross-validation requires at least two folds.";
            return false;
        }

        std::vector<std::size_t> negativeRows;
        std::vector<std::size_t> positiveRows;
        for (std::size_t row = 0; row < data.labels.size(); ++row) {
            if (data.labels[row] == 1) {
                positiveRows.push_back(row);
            } else {
                negativeRows.push_back(row);
            }
        }
        if (negativeRows.size() < foldCount ||
            positiveRows.size() < foldCount) {
            errorMessage =
                "Each class must have at least one row per cross-validation fold.";
            return false;
        }

        std::mt19937 generator(42);
        std::shuffle(negativeRows.begin(), negativeRows.end(), generator);
        std::shuffle(positiveRows.begin(), positiveRows.end(), generator);

        // Spread each class across folds so every fold keeps both labels.
        std::vector<std::size_t> foldAssignments(data.labels.size(), 0);
        for (std::size_t i = 0; i < negativeRows.size(); ++i) {
            foldAssignments[negativeRows[i]] = i % foldCount;
        }
        for (std::size_t i = 0; i < positiveRows.size(); ++i) {
            foldAssignments[positiveRows[i]] = i % foldCount;
        }

        EvaluationMetrics combined;
        for (std::size_t fold = 0; fold < foldCount; ++fold) {
            Dataset trainingData;
            Dataset validationData;
            for (std::size_t row = 0; row < data.features.size(); ++row) {
                Dataset& destination = foldAssignments[row] == fold
                    ? validationData
                    : trainingData;
                destination.features.push_back(data.features[row]);
                destination.labels.push_back(data.labels[row]);
            }

            Scaler scaler;
            Dataset scaledTraining;
            Dataset scaledValidation;
            try {
                scaler.fit(trainingData);
                scaledTraining = scaler.transform(trainingData);
                scaledValidation = scaler.transform(validationData);
            } catch (const std::invalid_argument& error) {
                errorMessage = "Cross-validation scaling failed: ";
                errorMessage += error.what();
                return false;
            } catch (const std::logic_error& error) {
                errorMessage = "Cross-validation scaling failed: ";
                errorMessage += error.what();
                return false;
            }

            Hyperplane model;
            SvmTrainer trainer;
            if (!trainer.train(scaledTraining, options, model, errorMessage)) {
                errorMessage = "Cross-validation training failed: " +
                    errorMessage;
                return false;
            }

            EvaluationMetrics foldMetrics;
            if (!evaluateModel(model, scaledValidation, foldMetrics, errorMessage)) {
                errorMessage = "Cross-validation evaluation failed: " +
                    errorMessage;
                return false;
            }
            addMetrics(combined, foldMetrics);
        }

        finalizeMetrics(combined);
        metrics = combined;
        errorMessage.clear();
        return true;
    }
};

} // namespace

bool trainSvm(
    const Dataset& trainingData,
    const TrainingOptions& options,
    Hyperplane& model,
    std::string& errorMessage
) {
    SvmTrainer trainer;
    return trainer.train(trainingData, options, model, errorMessage);
}

bool decisionFunction(
    const Hyperplane& model,
    const std::vector<double>& features,
    double& result,
    std::string& errorMessage
) {
    DecisionEvaluator evaluator;
    return evaluator.evaluate(model, features, result, errorMessage);
}

bool calculateFunctionalMargin(
    const Hyperplane& model,
    const std::vector<double>& features,
    int label,
    double& margin,
    std::string& errorMessage
) {
    MarginCalculator calculator;
    return calculator.calculate(model, features, label, margin, errorMessage);
}

bool predictLabel(
    const Hyperplane& model,
    const std::vector<double>& features,
    int& prediction,
    std::string& errorMessage
) {
    LabelPredictor predictor;
    return predictor.predict(model, features, prediction, errorMessage);
}

bool crossValidateSvm(
    const Dataset& data,
    std::size_t foldCount,
    const TrainingOptions& options,
    EvaluationMetrics& metrics,
    std::string& errorMessage
) {
    CrossValidator validator;
    return validator.validate(data, foldCount, options, metrics, errorMessage);
}

}
