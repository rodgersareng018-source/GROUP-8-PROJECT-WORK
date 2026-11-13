#ifndef EDUSVM_REGULARIZATION_HPP
#define EDUSVM_REGULARIZATION_HPP

#include <string>
#include <vector>

namespace edusvm {

// Calculate the L2 penalty: lambda / 2 times the sum of squared weights.
bool calculateL2Regularization(
    const std::vector<double>& weights,
    double lambda,
    double& penalty,
    std::string& errorMessage
);

}

#endif
