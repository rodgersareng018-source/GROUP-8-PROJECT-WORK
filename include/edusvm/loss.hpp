#ifndef EDUSVM_LOSS_HPP
#define EDUSVM_LOSS_HPP

#include <string>
#include <vector>

namespace edusvm {

bool calculateHingeLoss(
    const std::vector<double>& weights,
    double bias,
    const std::vector<double>& features,
    int label,
    double& loss,
    std::string& errorMessage
);

}

#endif
