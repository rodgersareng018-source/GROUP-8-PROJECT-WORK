
#ifndef EDUSVM_PREPROCESSOR_BASE_HPP
#define EDUSVM_PREPROCESSOR_BASE_HPP

#include "edusvm/dataset.hpp"
#include <string>

namespace edusvm {

// Shared interface for preprocessors that can be called through a base pointer.
class Preprocessor {
private:
    std::string name_;

public:
    explicit Preprocessor(const std::string& name)
        : name_(name) {}

    virtual ~Preprocessor() = default;

    std::string getName() const {
        return name_;
    }

    virtual bool process(
        const Dataset& input,
        Dataset& output,
        std::string& errorMessage
    ) const = 0;
};

} // namespace edusvm

#endif
