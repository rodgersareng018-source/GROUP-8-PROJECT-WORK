#ifndef EDUSVM_PREPROCESSOR_HPP
#define EDUSVM_PREPROCESSOR_HPP

#include "edusvm/dataset.hpp"
#include "edusvm/preprocessor_base.hpp"
#include <string>

namespace edusvm {

class DatasetPreprocessor final : public Preprocessor {
public:
    DatasetPreprocessor();

    bool process(
        const Dataset& input,
        Dataset& output,
        std::string& errorMessage
    ) const override;
};

bool preprocessDataset(
    const Dataset& input,
    Dataset& output,
    std::string& errorMessage
);

}

#endif
