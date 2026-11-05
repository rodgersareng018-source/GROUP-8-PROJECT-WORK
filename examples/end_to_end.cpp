#include "edusvm/dataset.hpp"
#include "edusvm/preprocessor_strategies.hpp"
#include "edusvm/scaler.hpp"
#include "edusvm/svm.hpp"
#include "edusvm/evaluation.hpp"

#include <iomanip>
#include <iostream>
#include <string>

int main() {
    std::string errorMessage;

    // 1. Load student data from CSV.
    edusvm::Dataset rawData;

    if (!edusvm::DatasetLoader::loadFromCSV(
            "data/input/students.csv",
            rawData,
            errorMessage)) {
        std::cerr << "CSV loading failed: " << errorMessage << '\n';
        return 1;
    }

    std::cout << "EDUSVM - STUDENT PERFORMANCE CLASSIFICATION\n";
    std::cout << "===========================================\n";
    std::cout << "Records loaded: " << rawData.features.size() << '\n';

    // 2. Validate and preprocess the dataset.
    edusvm::ValidationPreprocessor validator;
    edusvm::Dataset validatedData;

    if (!validator.process(rawData, validatedData, errorMessage)) {
        std::cerr << "Preprocessing failed: " << errorMessage << '\n';
        return 1;
    }

    // 3. Split the data into training and testing sets.
    // First four records: training.
    // Last two records: testing.
    edusvm::Dataset trainingData;
    edusvm::Dataset testData;

    for (std::size_t i = 0; i < validatedData.features.size(); ++i) {
        if (i < 4) {
            trainingData.features.push_back(validatedData.features[i]);
            trainingData.labels.push_back(validatedData.labels[i]);
        } else {
            testData.features.push_back(validatedData.features[i]);
            testData.labels.push_back(validatedData.labels[i]);
        }
    }

    std::cout << "Training records: " << trainingData.features.size() << '\n';
    std::cout << "Testing records: " << testData.features.size() << '\n';

    // 4. Fit the scaler on training data only.
    edusvm::Scaler scaler;

    edusvm::Dataset scaledTrainingData;
    edusvm::Dataset scaledTestData;

    try {
        scaler.fit(trainingData);
        scaledTrainingData = scaler.transform(trainingData);
        scaledTestData = scaler.transform(testData);
    } catch (const std::exception& error) {
        std::cerr << "Scaling failed: " << error.what() << '\n';
        return 1;
    }

    std::cout << "Feature scaling completed.\n";

    // 5. Train the SVM model.
    edusvm::SVMTrainingOptions options;
    options.learningRate = 0.01;
    options.C = 1.0;
    options.maxIterations = 1000;

    edusvm::Hyperplane model;

    if (!edusvm::trainSVM(
            scaledTrainingData,
            options,
            model,
            errorMessage)) {
        std::cerr << "SVM training failed: " << errorMessage << '\n';
        return 1;
    }

    std::cout << "SVM training completed.\n";

    // 6. Predict the outcomes of the test records.
    std::cout << "\nPREDICTIONS\n";
    std::cout << "-----------\n";

    for (std::size_t i = 0; i < scaledTestData.features.size(); ++i) {
        int predictedLabel = 0;

        if (!edusvm::predict(
                model,
                scaledTestData.features[i],
                predictedLabel,
                errorMessage)) {
            std::cerr << "Prediction failed: " << errorMessage << '\n';
            return 1;
        }

        const int actualLabel = scaledTestData.labels[i];

        std::cout << "Student " << (i + 1)
                  << " | Actual: "
                  << (actualLabel == 1 ? "PASS" : "FAIL")
                  << " | Predicted: "
                  << (predictedLabel == 1 ? "PASS" : "FAIL")
                  << '\n';
    }

    // 7. Evaluate the model.
    edusvm::EvaluationMetrics metrics;

    if (!edusvm::evaluateModel(
            model,
            scaledTestData,
            metrics,
            errorMessage)) {
        std::cerr << "Model evaluation failed: " << errorMessage << '\n';
        return 1;
    }

    std::cout << "\nEVALUATION RESULTS\n";
    std::cout << "------------------\n";
    std::cout << std::fixed << std::setprecision(2);

    std::cout << "True Positives:  "
              << metrics.confusionMatrix.truePositive << '\n';

    std::cout << "True Negatives:  "
              << metrics.confusionMatrix.trueNegative << '\n';

    std::cout << "False Positives: "
              << metrics.confusionMatrix.falsePositive << '\n';

    std::cout << "False Negatives: "
              << metrics.confusionMatrix.falseNegative << '\n';

    std::cout << "Accuracy:  " << metrics.accuracy * 100.0 << "%\n";
    std::cout << "Precision: " << metrics.precision * 100.0 << "%\n";
    std::cout << "Recall:    " << metrics.recall * 100.0 << "%\n";
    std::cout << "F1 Score:  " << metrics.f1Score * 100.0 << "%\n";

    std::cout << "\nEnd-to-end classification completed.\n";

    return 0;
}
