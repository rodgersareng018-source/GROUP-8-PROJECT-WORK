#include "edusvm/dataset.hpp"
#include "edusvm/evaluation.hpp"
#include "edusvm/preprocessor_strategies.hpp"
#include "edusvm/scaler.hpp"
#include "edusvm/svm.hpp"

#include <cstddef>
#include <exception>
#include <iomanip>
#include <iostream>
#include <string>

int main() {
    std::string errorMessage;
    edusvm::Dataset data;

    if (!edusvm::DatasetLoader::loadFromCSV(
            "data/input/students.csv", data, errorMessage)) {
        std::cerr << "CSV loading failed: " << errorMessage << '\n';
        return 1;
    }

    edusvm::ValidationPreprocessor validator;
    edusvm::Dataset validatedData;
    if (!validator.process(data, validatedData, errorMessage)) {
        std::cerr << "Preprocessing failed: " << errorMessage << '\n';
        return 1;
    }

    edusvm::Dataset trainingData;
    edusvm::Dataset testData;
    for (std::size_t i = 0; i < validatedData.features.size(); ++i) {
        edusvm::Dataset& destination = i < 4 ? trainingData : testData;
        destination.features.push_back(validatedData.features[i]);
        destination.labels.push_back(validatedData.labels[i]);
    }

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

    edusvm::TrainingOptions options;
    options.learningRate = 0.01;
    options.epochs = 1000;
    edusvm::Hyperplane model;
    if (!edusvm::trainSvm(
            scaledTrainingData, options, model, errorMessage)) {
        std::cerr << "SVM training failed: " << errorMessage << '\n';
        return 1;
    }

    for (std::size_t i = 0; i < scaledTestData.features.size(); ++i) {
        int predictedLabel = 0;
        if (!edusvm::predictLabel(
                model, scaledTestData.features[i], predictedLabel, errorMessage)) {
            std::cerr << "Prediction failed: " << errorMessage << '\n';
            return 1;
        }

        const int actualLabel = scaledTestData.labels[i];
        std::cout << "Student " << (i + 1) << " | Actual: "
                  << (actualLabel == 1 ? "PASS" : "FAIL")
                  << " | Predicted: "
                  << (predictedLabel == 1 ? "PASS" : "FAIL")
                  << '\n';
    }

    edusvm::EvaluationMetrics metrics;
    if (!edusvm::evaluateModel(
            model, scaledTestData, metrics, errorMessage)) {
        std::cerr << "Model evaluation failed: " << errorMessage << '\n';
        return 1;
    }

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Accuracy:  " << metrics.accuracy * 100.0 << "%\n"
              << "Precision: " << metrics.precision * 100.0 << "%\n"
              << "Recall:    " << metrics.recall * 100.0 << "%\n"
              << "F1 Score:  " << metrics.f1Score * 100.0 << "%\n";

    return 0;
}
