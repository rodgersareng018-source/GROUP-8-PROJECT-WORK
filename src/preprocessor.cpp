#include "edusvm/preprocessor.hpp"

namespace edusvm {

bool preprocessDataset(
    const Dataset& input,
    Dataset& output,
    std::string& errorMessage
) {
    if (!validateDataset(input, errorMessage)) {
        return false;
    }

    // For this first version, preprocessing validates and copies the data. 
    // Feature scaling is handled by the scaler module.
    output = input;

    errorMessage.clear();
    return true;
}

}
