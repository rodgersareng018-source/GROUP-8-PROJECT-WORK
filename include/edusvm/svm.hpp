#ifndef EDUSVM_SVM_HPP
#define EDUSVM_SVM_HPP

#include <cstddef>
#include <string>
#include <vector>

#include "edusvm/dataset.hpp"

namespace edusvm {

// Linear model: score a row by weights dot features plus bias.
struct Hyperplane {
    std::vector<double> weights;
    double bias = 0.0;
    // Indices of training rows close to the learned decision margin.
    std::vector<std::size_t> supportVectorIndices;
};

// Tunable settings used by gradient-based training.
struct TrainingOptions {
    double lambda = 0.01;
    double learningRate = 0.1;
    std::size_t epochs = 1000;
};

// Confusion-matrix counts and commonly used binary-classification scores.
struct EvaluationMetrics {
    std::size_t truePositives = 0;
    std::size_t falsePositives = 0;
    std::size_t trueNegatives = 0;
    std::size_t falseNegatives = 0;
    double accuracy = 0.0;
    double precision = 0.0;
    double recall = 0.0;
    double f1Score = 0.0;
};

// Learn a linear SVM from labeled rows and return its weights and bias.
bool trainSvm(
    const Dataset& trainingData,
    const TrainingOptions& options,
    Hyperplane& model,
    std::string& errorMessage
);

// Calculate the signed model score; zero lies on the decision boundary.
bool decisionFunction(
    const Hyperplane& model,
    const std::vector<double>& features,
    double& result,
    std::string& errorMessage
);

// Calculate label * score; correctly classified points have positive margin.
bool calculateFunctionalMargin(
    const Hyperplane& model,
    const std::vector<double>& features,
    int label,
    double& margin,
    std::string& errorMessage
);

// Convert a decision score to the project's +1 or -1 class label.
bool predictLabel(
    const Hyperplane& model,
    const std::vector<double>& features,
    int& prediction,
    std::string& errorMessage
);

// Compare predictions with known labels and fill the confusion-matrix metrics.
bool evaluateModel(
    const Hyperplane& model,
    const Dataset& data,
    EvaluationMetrics& metrics,
    std::string& errorMessage
);

// Evaluate every row once using deterministic, label-balanced folds.
// Scaling is fitted only on each fold's training rows.
bool crossValidateSvm(
    const Dataset& data,
    std::size_t foldCount,
    const TrainingOptions& options,
    EvaluationMetrics& metrics,
    std::string& errorMessage
);

}

#endif
