#include "edusvm/regularization.hpp"

#include <cmath>

namespace edusvm {

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

    double sum = 0.0;

    for (double weight : weights) {
        if (!std::isfinite(weight)) {
            errorMessage = "Weights must be finite.";
            return false;
        }

        sum += weight * weight;
    }

    penalty = (lambda / 2.0) * sum;

    if (!std::isfinite(penalty)) {
        errorMessage = "Regularization result is not finite.";
        return false;
    }

    errorMessage.clear();
    return true;
}

}
