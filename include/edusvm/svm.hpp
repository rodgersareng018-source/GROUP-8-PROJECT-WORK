#ifndef EDUSVM_SVM_HPP
#define EDUSVM_SVM_HPP

#include <cstddef>
#include <string>
#include <vector>

#include "edusvm/dataset.hpp"
#include "edusvm/evaluation.hpp"

namespace edusvm {

// The learned decision boundary: weights dot features plus bias.
struct Hyperplane {
    std::vector<double> weights;
    double bias = 0.0;
    std::vector<std::size_t> supportVectorIndices;
};

struct TrainingOptions {
    double lambda = 0.01;
    double learningRate = 0.1;
    std::size_t epochs = 1000;
};

bool trainSvm(
    const Dataset& trainingData,
    const TrainingOptions& options,
    Hyperplane& model,
    std::string& errorMessage
);

bool decisionFunction(
    const Hyperplane& model,
    const std::vector<double>& features,
    double& result,
    std::string& errorMessage
);

bool calculateFunctionalMargin(
    const Hyperplane& model,
    const std::vector<double>& features,
    int label,
    double& margin,
    std::string& errorMessage
);

bool predictLabel(
    const Hyperplane& model,
    const std::vector<double>& features,
    int& prediction,
    std::string& errorMessage
);

bool crossValidateSvm(
    const Dataset& data,
    std::size_t foldCount,
    const TrainingOptions& options,
    EvaluationMetrics& metrics,
    std::string& errorMessage
);

}

#endif
