#include "edusvm/loss.hpp"


#include <algorithm>
#include <cmath>

namespace edusvm {

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

    score = bias;
    for (std::size_t i = 0; i < weights.size(); ++i) {
        score += weights[i] * features[i];
    }

    loss = std::max(0.0, 1.0 - label * score);

    errorMessage.clear();
    return true;
}

}
