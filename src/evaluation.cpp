#include "edusvm/evaluation.hpp"

#include <string>

namespace edusvm {

bool evaluateModel(
    const Hyperplane& model,
    const Dataset& testData,
    EvaluationMetrics& metrics,
    std::string& errorMessage
) {
    if (testData.features.empty()) {
        errorMessage = "Test dataset is empty.";
        return false;
    }

    if (testData.features.size() != testData.labels.size()) {
        errorMessage = "Number of feature rows and labels must match.";
        return false;
    }

    EvaluationMetrics result;

    for (std::size_t i = 0; i < testData.features.size(); ++i) {
        const int actual = testData.labels[i];

        if (actual != 1 && actual != -1) {
            errorMessage = "Test labels must be either +1 or -1.";
            return false;
        }

        int predicted = 0;

        if (!predict(model, testData.features[i], predicted, errorMessage)) {
            errorMessage = "Prediction failed for test row " +
                           std::to_string(i) + ": " + errorMessage;
            return false;
        }

        if (actual == 1 && predicted == 1) {
            ++result.confusionMatrix.truePositive;
        }
        else if (actual == -1 && predicted == -1) {
            ++result.confusionMatrix.trueNegative;
        }
        else if (actual == -1 && predicted == 1) {
            ++result.confusionMatrix.falsePositive;
        }
        else {
            ++result.confusionMatrix.falseNegative;
        }
    }

    const double total = static_cast<double>(testData.labels.size());

    const double tp = static_cast<double>(
        result.confusionMatrix.truePositive
    );

    const double fp = static_cast<double>(
        result.confusionMatrix.falsePositive
    );

    const double fn = static_cast<double>(
        result.confusionMatrix.falseNegative
    );

    const double correct = tp + static_cast<double>(
        result.confusionMatrix.trueNegative
    );

    result.accuracy = correct / total;
    result.precision = (tp + fp == 0.0) ? 0.0 : tp / (tp + fp);
    result.recall = (tp + fn == 0.0) ? 0.0 : tp / (tp + fn);

    const double precisionRecallSum =
        result.precision + result.recall;

    result.f1Score = (precisionRecallSum == 0.0)
        ? 0.0
        : 2.0 * result.precision * result.recall /
          precisionRecallSum;

    metrics = result;
    errorMessage.clear();

    return true;
}

} // namespace edusvm
