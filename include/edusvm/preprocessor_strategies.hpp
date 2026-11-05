#ifndef EDUSVM_PREPROCESSOR_STRATEGIES_HPP
#define EDUSVM_PREPROCESSOR_STRATEGIES_HPP

#include "edusvm/preprocessor_base.hpp"
#include "edusvm/scaler.hpp"

namespace edusvm {

// Choose how a preprocessor handles rows containing missing feature values.
enum class ImputationStrategy {
    MEAN,
    DROP_ROW
};

// A strategy that checks dataset structure and values without changing them.
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

// A strategy that scales feature columns while preserving the labels.
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

class ImputationPreprocessor final : public Preprocessor {
private:
    ImputationStrategy strategy_;

public:
    explicit ImputationPreprocessor(ImputationStrategy strategy)
        : Preprocessor("Missing Value Imputation"),
          strategy_(strategy) {}

    bool process(
        const Dataset& input,
        Dataset& output,
        std::string& errorMessage
    ) const override;
};

} // namespace edusvm

#endif
