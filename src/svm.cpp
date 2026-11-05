#include "edusvm/svm.hpp"

#include <cmath>
#include <string>
#include <vector>

namespace edusvm {

bool decisionFunction(
    const Hyperplane& model,
    const std::vector<double>& features,
    double& result,
    std::string& errorMessage
) {
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

bool calculateFunctionalMargin(
    const Hyperplane& model,
    const std::vector<double>& features,
    int label,
    double& margin,
    std::string& errorMessage
) {
    if (label != 1 && label != -1) {
        errorMessage = "Label must be 1 or -1.";
        return false;
    }

    double result = 0.0;

    if (!decisionFunction(model, features, result, errorMessage)) {
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

bool predict(
    const Hyperplane& model,
    const std::vector<double>& features,
    int& predictedLabel,
    std::string& errorMessage
) {
    double score = 0.0;

    if (!decisionFunction(model, features, score, errorMessage)) {
        return false;
    }

    predictedLabel = (score >= 0.0) ? 1 : -1;

    errorMessage.clear();
    return true;
}

bool identifySupportVectors(
    const Hyperplane& model,
    const Dataset& data,
    std::vector<std::size_t>& indices,
    double tolerance,
    std::string& errorMessage
) {
    if (!std::isfinite(tolerance) || tolerance < 0.0) {
        errorMessage = "Tolerance must be finite and non-negative.";
        return false;
    }

    if (data.features.empty()) {
        errorMessage = "Cannot identify support vectors in an empty dataset.";
        return false;
    }

    if (data.features.size() != data.labels.size()) {
        errorMessage = "Feature and label counts do not match.";
        return false;
    }

    std::vector<std::size_t> result;

    for (std::size_t i = 0; i < data.features.size(); ++i) {
        double margin = 0.0;

        if (!calculateFunctionalMargin(
                model,
                data.features[i],
                data.labels[i],
                margin,
                errorMessage)) {
            return false;
        }

        if (margin <= 1.0 + tolerance) {
            result.push_back(i);
        }
    }

    indices = result;
    errorMessage.clear();
    return true;
}

bool trainSVM(
    const Dataset& trainingData,
    const SVMTrainingOptions& options,
    Hyperplane& model,
    std::string& errorMessage
) {
    const std::size_t sampleCount = trainingData.features.size();

    if (sampleCount == 0) {
        errorMessage = "Training failed: dataset is empty.";
        return false;
    }

    if (sampleCount != trainingData.labels.size()) {
        errorMessage = "Training failed: feature and label counts do not match.";
        return false;
    }

    if (!std::isfinite(options.learningRate) ||
        options.learningRate <= 0.0) {
        errorMessage = "Learning rate must be finite and positive.";
        return false;
    }

    if (!std::isfinite(options.C) || options.C <= 0.0) {
        errorMessage = "C must be finite and positive.";
        return false;
    }

    if (options.maxIterations == 0) {
        errorMessage = "Maximum iterations must be greater than zero.";
        return false;
    }

    const std::size_t featureCount = trainingData.features[0].size();

    if (featureCount == 0) {
        errorMessage = "Training failed: dataset has no features.";
        return false;
    }

    bool hasPositiveLabel = false;
    bool hasNegativeLabel = false;

    for (std::size_t i = 0; i < sampleCount; ++i) {
        if (trainingData.features[i].size() != featureCount) {
            errorMessage = "Training failed: inconsistent feature count.";
            return false;
        }

        if (trainingData.labels[i] != 1 &&
            trainingData.labels[i] != -1) {
            errorMessage = "Training failed: labels must be 1 or -1.";
            return false;
        }

        if (trainingData.labels[i] == 1) {
            hasPositiveLabel = true;
        } else {
            hasNegativeLabel = true;
        }

        for (double value : trainingData.features[i]) {
            if (!std::isfinite(value)) {
                errorMessage = "Training failed: features must be finite.";
                return false;
            }
        }
    }

    if (!hasPositiveLabel || !hasNegativeLabel) {
        errorMessage = "Training requires both positive and negative classes.";
        return false;
    }

    Hyperplane trainedModel;
    trainedModel.weights.assign(featureCount, 0.0);
    trainedModel.bias = 0.0;

    const double sampleCountDouble = static_cast<double>(sampleCount);

    for (std::size_t iteration = 0;
         iteration < options.maxIterations;
         ++iteration) {

        std::vector<double> gradientWeights(featureCount, 0.0);
        double gradientBias = 0.0;

        // Gradient of the L2 regularization term.
        for (std::size_t j = 0; j < featureCount; ++j) {
            gradientWeights[j] = trainedModel.weights[j];
        }

        // Calculate the batch hinge-loss gradients.
        for (std::size_t i = 0; i < sampleCount; ++i) {
            double score = trainedModel.bias;

            for (std::size_t j = 0; j < featureCount; ++j) {
                score += trainedModel.weights[j] *
                         trainingData.features[i][j];
            }

            if (!std::isfinite(score)) {
                errorMessage = "Training failed: decision score is not finite.";
                return false;
            }

            const double margin =
                trainingData.labels[i] * score;

            // At margin >= 1, the hinge-loss subgradient is zero.
            if (margin < 1.0) {
                for (std::size_t j = 0; j < featureCount; ++j) {
                    gradientWeights[j] -=
                        (options.C / sampleCountDouble) *
                        trainingData.labels[i] *
                        trainingData.features[i][j];
                }

                gradientBias -=
                    (options.C / sampleCountDouble) *
                    trainingData.labels[i];
            }
        }

        // Gradually decrease the learning rate.
        const double learningRate =
            options.learningRate /
            std::sqrt(static_cast<double>(iteration) + 1.0);

        // Update weights.
        for (std::size_t j = 0; j < featureCount; ++j) {
            if (!std::isfinite(gradientWeights[j])) {
                errorMessage = "Training failed: weight gradient is not finite.";
                return false;
            }

            trainedModel.weights[j] -=
                learningRate * gradientWeights[j];

            if (!std::isfinite(trainedModel.weights[j])) {
                errorMessage = "Training failed: updated weight is not finite.";
                return false;
            }
        }

        // Update bias.
        if (!std::isfinite(gradientBias)) {
            errorMessage = "Training failed: bias gradient is not finite.";
            return false;
        }

        trainedModel.bias -= learningRate * gradientBias;

        if (!std::isfinite(trainedModel.bias)) {
            errorMessage = "Training failed: updated bias is not finite.";
            return false;
        }
    }

    model = trainedModel;
    errorMessage.clear();
    return true;
}

} // namespace edusvm
