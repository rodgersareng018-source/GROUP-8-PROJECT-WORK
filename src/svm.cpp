#include "edusvm/svm.hpp"
#include <cmath>

namespace edusvm {

bool decisionFunction(
    const Hyperplane& model,
    const std::vector<double>& features,
    double& result,
    std::string& errorMessage
) {
    if (model.weights.empty() || model.weights.size() != features.size()) {
        errorMessage = "Feature count does not match model weights.";
        return false;
    }
    if (!std::isfinite(model.bias)) {
        errorMessage = "Model bias must be finite.";
        return false;
    }

    double sum = model.bias;
    for (std::size_t i = 0; i < features.size(); ++i) {
        if (!std::isfinite(features[i]) || !std::isfinite(model.weights[i])) {
            errorMessage = "Features and weights must be finite.";
            return false;
        }
        sum += model.weights[i] * features[i];
    }

    if (!std::isfinite(sum)) {
        errorMessage = "Decision result is not finite.";
        return false;
    }

    result = sum;
    errorMessage.clear();
    return true;
}

bool calculateFunctionalMargin(
    const Hyperplane& model,
    const std::vector<double>& features,
    int label,
    double& margin,
    std::string& errorMessage
) {
    if (label != 1 && label != -1) {
        errorMessage = "Label must be 1 or -1.";
        return false;
    }

    double result = 0.0;
    if (!decisionFunction(model, features, result, errorMessage)) {
        return false;
    }

    margin = label * result;
    errorMessage.clear();
    return true;
}

}