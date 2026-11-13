#include "edusvm/evaluation.hpp"
#include "edusvm/svm.hpp"

#include <string>

namespace edusvm {

namespace {

class EvaluationEngine {
public:
    bool evaluate(
        const Hyperplane& model,
        const Dataset& testData,
        EvaluationMetrics& metrics,
        std::string& errorMessage
    ) const {
        if (!validateDataset(testData, errorMessage)) {
            errorMessage = "Invalid evaluation dataset: " + errorMessage;
            return false;
        }
        if (model.weights.empty() ||
            model.weights.size() != testData.features.front().size()) {
            errorMessage = "Feature count does not match model weights.";
            return false;
        }

        EvaluationMetrics result;

        for (std::size_t i = 0; i < testData.features.size(); ++i) {
            const int actual = testData.labels[i];

            int predicted = 0;

            if (!predictLabel(
                    model, testData.features[i], predicted, errorMessage)) {
                errorMessage = "Prediction failed for test row " +
                    std::to_string(i) + ": " + errorMessage;
                return false;
            }

            if (actual == 1 && predicted == 1) {
                ++result.truePositives;
            } else if (actual == -1 && predicted == -1) {
                ++result.trueNegatives;
            } else if (actual == -1 && predicted == 1) {
                ++result.falsePositives;
            } else {
                ++result.falseNegatives;
            }
        }

        const double total = static_cast<double>(testData.labels.size());

        const double truePositive = static_cast<double>(result.truePositives);
        const double falsePositive = static_cast<double>(result.falsePositives);
        const double falseNegative = static_cast<double>(result.falseNegatives);
        const double correct = truePositive +
            static_cast<double>(result.trueNegatives);

        result.accuracy = correct / total;
        result.precision = (truePositive + falsePositive == 0.0)
            ? 0.0
            : truePositive / (truePositive + falsePositive);
        result.recall = (truePositive + falseNegative == 0.0)
            ? 0.0
            : truePositive / (truePositive + falseNegative);

        const double precisionRecallSum = result.precision + result.recall;

        result.f1Score = (precisionRecallSum == 0.0)
            ? 0.0
            : 2.0 * result.precision * result.recall / precisionRecallSum;

        metrics = result;
        errorMessage.clear();

        return true;
    }
};

} // namespace

bool evaluateModel(
    const Hyperplane& model,
    const Dataset& testData,
    EvaluationMetrics& metrics,
    std::string& errorMessage
) {
    EvaluationEngine engine;
    return engine.evaluate(model, testData, metrics, errorMessage);
}

} // namespace edusvm
