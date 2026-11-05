#ifndef EDUSVM_EVALUATION_HPP
#define EDUSVM_EVALUATION_HPP

#include <cstddef>
#include <string>

#include "edusvm/dataset.hpp"
#include "edusvm/svm.hpp"

namespace edusvm {

struct ConfusionMatrix {
    std::size_t truePositive = 0;
    std::size_t trueNegative = 0;
    std::size_t falsePositive = 0;
    std::size_t falseNegative = 0;
};

struct EvaluationMetrics {
    ConfusionMatrix confusionMatrix;
    double accuracy = 0.0;
    double precision = 0.0;
    double recall = 0.0;
    double f1Score = 0.0;
};

bool evaluateModel(
    const Hyperplane& model,
    const Dataset& testData,
    EvaluationMetrics& metrics,
    std::string& errorMessage
);

} // namespace edusvm

#endif // EDUSVM_EVALUATION_HPP
