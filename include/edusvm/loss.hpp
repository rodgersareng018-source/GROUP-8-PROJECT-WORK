#ifndef EDUSVM_LOSS_HPP
#define EDUSVM_LOSS_HPP

#include <string>
#include <vector>

namespace edusvm {

// Compute max(0, 1 - label * (weights dot features + bias)).
// Returns false for mismatched dimensions, invalid labels, or non-finite inputs.
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
