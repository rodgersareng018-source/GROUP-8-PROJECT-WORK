#pragma once

#include <vector>

#include "edusvm/dataset.hpp"

namespace edusvm {

// Learns min/max values from training data and reuses them when scaling data.
class Scaler {
public:
    void fit(const Dataset& data);

    Dataset transform(const Dataset& data) const;

    Dataset fitTransform(const Dataset& data);

    bool isFitted() const { return fitted_; }

private:
    // These values come from the training dataset, not each new input.
    std::vector<double> min_;
    std::vector<double> max_;
    bool fitted_ = false;
};

}  // namespace edusvm