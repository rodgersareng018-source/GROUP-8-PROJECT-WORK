
#include "edusvm/preprocessor_strategies.hpp"
#include "edusvm/preprocessor.hpp"

namespace edusvm {

bool ValidationPreprocessor::process(
    const Dataset& input,
    Dataset& output,
    std::string& errorMessage
) const {
    return preprocessDataset(input, output, errorMessage);
}

bool ScalingPreprocessor::process(
    const Dataset& input,
    Dataset& output,
    std::string& errorMessage
) const {
    try {
        Scaler scaler;
        output = scaler.fitTransform(input);
        errorMessage.clear();
        return true;
    }
    catch (const std::exception& error) {
        errorMessage = error.what();
        return false;
    }
}

} // namespace edusvm
