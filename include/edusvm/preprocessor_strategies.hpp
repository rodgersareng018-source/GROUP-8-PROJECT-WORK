
#ifndef EDUSVM_PREPROCESSOR_STRATEGIES_HPP
#define EDUSVM_PREPROCESSOR_STRATEGIES_HPP

#include "edusvm/preprocessor_base.hpp"
#include "edusvm/scaler.hpp"

namespace edusvm {

class ValidationPreprocessor final : public Preprocessor {
public:
    ValidationPreprocessor()
        : Preprocessor("Dataset Validation") {}

    bool process(
        const Dataset& input,
        Dataset& output,
        std::string& errorMessage
    ) const override;
};

class ScalingPreprocessor final : public Preprocessor {
public:
    ScalingPreprocessor()
        : Preprocessor("Feature Scaling") {}

    bool process(
        const Dataset& input,
        Dataset& output,
        std::string& errorMessage
    ) const override;
};

} // namespace edusvm

#endif
