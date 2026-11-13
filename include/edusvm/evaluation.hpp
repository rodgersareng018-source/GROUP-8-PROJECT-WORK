#ifndef EDUSVM_EVALUATION_HPP
#define EDUSVM_EVALUATION_HPP

#include <cstddef>
#include <string>

#include "edusvm/dataset.hpp"

namespace edusvm {

struct Hyperplane;

// Counts the four prediction outcomes and the scores calculated from them.
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

bool evaluateModel(
    const Hyperplane& model,
    const Dataset& data,
    EvaluationMetrics& metrics,
    std::string& errorMessage
);

} // namespace edusvm

#endif // EDUSVM_EVALUATION_HPP
