#ifndef EDUSVM_SVM_HPP
#define EDUSVM_SVM_HPP

#include <string>
#include <vector>

namespace edusvm {

struct Hyperplane {
    std::vector<double> weights;
    double bias = 0.0;
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

}

#endif