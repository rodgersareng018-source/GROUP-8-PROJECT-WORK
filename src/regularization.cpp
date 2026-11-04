#include "edusvm/regularization.hpp"

#include <cmath>

namespace edusvm {

// Penalize large model weights to discourage an overly complex boundary.
bool calculateL2Regularization(
    const std::vector<double>& weights,
    double lambda,
    double& penalty,
    std::string& errorMessage
) {
    if (!std::isfinite(lambda) || lambda < 0.0) {
        errorMessage = "Lambda must be finite and non-negative.";
        return false;
    }

    // Squaring each weight and summing gives the squared Euclidean norm.
    double sum = 0.0;

    for (double weight : weights) {
        if (!std::isfinite(weight)) {
            errorMessage = "Weights must be finite.";
            return false;
        }

        sum += weight * weight;
    }

    // The one-half factor makes the derivative of this penalty lambda * weight.
    penalty = (lambda / 2.0) * sum;

    if (!std::isfinite(penalty)) {
        errorMessage = "Regularization result is not finite.";
        return false;
    }

    errorMessage.clear();
    return true;
}

}
