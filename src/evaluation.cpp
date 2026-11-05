#include "edusvm/evaluation.hpp"
#include "edusvm/svm.hpp"

#include <string>

namespace edusvm {

bool evaluateModel(
    const Hyperplane& model,
    const Dataset& testData,
    EvaluationMetrics& metrics,
    std::string& errorMessage
) {
    // Validate the dataset and model dimensions before evaluating any rows.
    if (!validateDataset(testData, errorMessage)) {
        errorMessage = "Invalid evaluation dataset: " + errorMessage;
        return false;
    }
    if (model.weights.empty() ||
        model.weights.size() != testData.features.front().size()) {
        errorMessage = "Feature count does not match model weights.";
        return false;
    }

    // Build results locally so the caller's metrics are only updated on success.
    EvaluationMetrics result;

    for (std::size_t i = 0; i < testData.features.size(); ++i) {
        const int actual = testData.labels[i];

        int predicted = 0;

        // Include the row number to make prediction errors easier to locate.
        if (!predictLabel(model, testData.features[i], predicted, errorMessage)) {
            errorMessage = "Prediction failed for test row " +
                           std::to_string(i) + ": " + errorMessage;
            return false;
        }

        // Compare the actual and predicted labels to fill the confusion matrix.
        if (actual == 1 && predicted == 1) {
            ++result.truePositives;
        }
        else if (actual == -1 && predicted == -1) {
            ++result.trueNegatives;
        }
        else if (actual == -1 && predicted == 1) {
            ++result.falsePositives;
        }
        else {
            ++result.falseNegatives;
        }
    }

    // Convert counts to doubles so the metric calculations use division.
    const double total = static_cast<double>(testData.labels.size());

    const double tp = static_cast<double>(
        result.truePositives
    );

    const double fp = static_cast<double>(
        result.falsePositives
    );

    const double fn = static_cast<double>(
        result.falseNegatives
    );

    // Accuracy is the share of all predictions that were correct.
    const double correct = tp + static_cast<double>(
        result.trueNegatives
    );

    result.accuracy = correct / total;

    // Return zero when there are no positive predictions or actual positives.
    result.precision = (tp + fp == 0.0) ? 0.0 : tp / (tp + fp);
    result.recall = (tp + fn == 0.0) ? 0.0 : tp / (tp + fn);

    // F1 is the harmonic mean of precision and recall.
    const double precisionRecallSum =
        result.precision + result.recall;

    result.f1Score = (precisionRecallSum == 0.0)
        ? 0.0
        : 2.0 * result.precision * result.recall /
          precisionRecallSum;

    // Publish the completed metrics and clear any previous error message.
    metrics = result;
    errorMessage.clear();

    return true;
}

} // namespace edusvm
