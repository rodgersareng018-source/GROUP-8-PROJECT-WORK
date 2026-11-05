
#ifndef EDUSVM_PREPROCESSOR_BASE_HPP
#define EDUSVM_PREPROCESSOR_BASE_HPP

#include "edusvm/dataset.hpp"
#include <string>

namespace edusvm {

// Common interface for interchangeable dataset-processing steps.
class Preprocessor {
private:
    std::string name_;

public:
    explicit Preprocessor(const std::string& name)
        : name_(name) {}

    // Allow derived preprocessors to be safely destroyed through this interface.
    virtual ~Preprocessor() = default;

    // Return the human-readable name assigned to this processing step.
    std::string getName() const {
        return name_;
    }

    // Derived classes implement their operation and report errors consistently.
    virtual bool process(
        const Dataset& input,
        Dataset& output,
        std::string& errorMessage
    ) const = 0;
};

} // namespace edusvm

#endif
