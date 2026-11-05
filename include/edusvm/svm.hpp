#ifndef EDUSVM_SVM_HPP
#define EDUSVM_SVM_HPP

#include <cstddef>
#include <string>
#include <vector>

#include "edusvm/dataset.hpp"

namespace edusvm {

struct Hyperplane {
    std::vector<double> weights;
    double bias = 0.0;
};

struct SVMTrainingOptions {
    double learningRate = 0.01;
    double C = 1.0;
    std::size_t maxIterations = 1000;
};

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

bool predict(
    const Hyperplane& model,
    const std::vector<double>& features,
    int& predictedLabel,
    std::string& errorMessage
);

bool identifySupportVectors(
    const Hyperplane& model,
    const Dataset& data,
    std::vector<std::size_t>& indices,
    double tolerance,
    std::string& errorMessage
);

bool trainSVM(
    const Dataset& trainingData,
    const SVMTrainingOptions& options,
    Hyperplane& model,
    std::string& errorMessage
);

} // namespace edusvm

#endif
