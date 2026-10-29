#ifndef EDUSVM_PREPROCESSOR_HPP
#define EDUSVM_PREPROCESSOR_HPP

#include "edusvm/dataset.hpp"
#include <string>

namespace edusvm {

bool preprocessDataset(
    const Dataset& input,
    Dataset& output,
    std::string& errorMessage
);


}

#endif
