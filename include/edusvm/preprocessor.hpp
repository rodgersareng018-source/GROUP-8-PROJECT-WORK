#ifndef EDUSVM_PREPROCESSOR_HPP
#define EDUSVM_PREPROCESSOR_HPP

#include "edusvm/dataset.hpp"
#include <string>

namespace edusvm {

// Validate a dataset and copy it to output when all rows are usable.
bool preprocessDataset(
    const Dataset& input,
    Dataset& output,
    std::string& errorMessage
);

}

#endif
