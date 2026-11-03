#ifndef EDUSVM_REGULARIZATION_HPP
#define EDUSVM_REGULARIZATION_HPP

#include <string>
#include <vector>

namespace edusvm {

bool calculateL2Regularization(
    const std::vector<double>& weights,
    double lambda,
    double& penalty,
    std::string& errorMessage
);

}

#endif
