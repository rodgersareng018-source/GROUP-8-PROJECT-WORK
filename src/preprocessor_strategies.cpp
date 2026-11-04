
#include "edusvm/preprocessor_strategies.hpp"
#include "edusvm/preprocessor.hpp"

#include <stdexcept>

namespace edusvm {

// Adapt the standalone validation function to the common strategy interface.
bool ValidationPreprocessor::process(
    const Dataset& input,
    Dataset& output,
    std::string& errorMessage
) const {
    return preprocessDataset(input, output, errorMessage);
}

// Fit a scaler on this dataset and return a scaled copy; convert scaler
// exceptions into the error-message convention used by Preprocessor.
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
    catch (const std::invalid_argument& error) {
        errorMessage = error.what();
        return false;
    }
    catch (const std::logic_error& error) {
        errorMessage = error.what();
        return false;
    }
}

} // namespace edusvm
