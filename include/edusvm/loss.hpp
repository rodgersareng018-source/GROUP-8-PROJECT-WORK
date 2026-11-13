#ifndef EDUSVM_LOSS_HPP
#define EDUSVM_LOSS_HPP

#include <string>
#include <vector>

namespace edusvm {

// Calculate max(0, 1 - label * score) for one row.
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
