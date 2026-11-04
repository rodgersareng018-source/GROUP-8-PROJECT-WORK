#include "edusvm/dataset.hpp"
#include "edusvm/preprocessor.hpp"
#include "edusvm/svm.hpp"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <string>

namespace {
// Keep command-line errors on stderr so they are easy to distinguish from results.
void reportFailure(const std::string& message) {
    std::cerr << "Error: " << message << '\n';
}
} // namespace

int main(int argc, char* argv[]) {
    // Accept zero or one optional argument: the CSV file to evaluate.
    if (argc > 2) {
        std::cerr << "Usage: edusvm [path-to-csv]\n";
        return 2;
    }

    // Use the sample file when the user does not provide a path.
    const std::string csvPath =
        argc == 2 ? argv[1] : "data/input/students.csv";
    std::string errorMessage;
    edusvm::Dataset loaded;
    if (!edusvm::loadDatasetCsv(csvPath, loaded, errorMessage)) {
        reportFailure(errorMessage);
        return 1;
    }

    // Validate the loaded rows before any model or scaling work.
    edusvm::Dataset validated;
    if (!edusvm::preprocessDataset(loaded, validated, errorMessage)) {
        reportFailure(errorMessage);
        return 1;
    }

    // Choose as many folds as possible, up to five, while keeping both labels
    // represented in every validation fold.
    std::size_t negativeCount = 0;
    std::size_t positiveCount = 0;
    for (int label : validated.labels) {
        (label == 1 ? positiveCount : negativeCount)++;
    }
    const std::size_t foldCount =
        std::min<std::size_t>(5, std::min(negativeCount, positiveCount));
    if (foldCount < 2) {
        reportFailure(
            "Cross-validation requires at least two rows from each class."
        );
        return 1;
    }

    edusvm::EvaluationMetrics metrics;
    if (!edusvm::crossValidateSvm(
            validated, foldCount, edusvm::TrainingOptions{}, metrics,
            errorMessage)) {
        reportFailure(errorMessage);
        return 1;
    }

    // Cross-validation pools predictions from rows held out during each fit.
    std::cout << "EduSVM student performance classifier\n"
              << "CSV: " << csvPath << '\n'
              << "Rows: " << validated.features.size()
              << ", features: " << validated.features.front().size() << '\n'
              << "Evaluation: stratified " << foldCount
              << "-fold cross-validation (" << validated.features.size()
              << " out-of-fold predictions; preliminary on this small dataset)\n"
              << std::fixed << std::setprecision(3)
              << "Accuracy:  " << metrics.accuracy << '\n'
              << "Precision: " << metrics.precision << '\n'
              << "Recall:    " << metrics.recall << '\n'
              << "F1-score:  " << metrics.f1Score << '\n'
              << "Confusion matrix (positive label = +1):\n"
              << "  TP: " << metrics.truePositives
              << "  FP: " << metrics.falsePositives << '\n'
              << "  TN: " << metrics.trueNegatives
              << "  FN: " << metrics.falseNegatives << '\n';
    return 0;
}
