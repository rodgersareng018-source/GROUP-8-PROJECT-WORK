#include "edusvm/loss.hpp"

#include <algorithm>
#include <cmath>

namespace edusvm {

// Hinge loss is zero beyond the correct margin and grows with each violation.
bool calculateHingeLoss(
    const std::vector<double>& weights,
    double bias,
    const std::vector<double>& features,
    int label,
    double& loss,
    std::string& errorMessage
) {
    if (label != 1 && label != -1) {
        errorMessage = "Label must be 1 or -1.";
        return false;
    }

    double score = 0.0;

    if (weights.size() != features.size()) {
        errorMessage = "Weights and features must have the same size.";
        return false;
    }

    if (!std::isfinite(bias)) {
        errorMessage = "Bias must be finite.";
        return false;
    }

    // First compute the model's raw score, then measure its signed margin.
    score = bias;
    for (std::size_t i = 0; i < weights.size(); ++i) {
        if (!std::isfinite(weights[i]) || !std::isfinite(features[i])) {
            errorMessage = "Weights and features must be finite.";
            return false;
        }
        score += weights[i] * features[i];
    }
    if (!std::isfinite(score)) {
        errorMessage = "Decision score is not finite.";
        return false;
    }

    loss = std::max(0.0, 1.0 - label * score);
    if (!std::isfinite(loss)) {
        errorMessage = "Hinge loss result is not finite.";
        return false;
    }

    errorMessage.clear();
    return true;
}

}
