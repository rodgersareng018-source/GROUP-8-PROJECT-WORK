#pragma once

#include <vector>

#include "edusvm/dataset.hpp"

namespace edusvm {

// Min-max feature scaler: x' = (x - min) / (max - min)
// Policy: if a feature is constant (max == min), every value becomes 0.0.
class Scaler {
public:
    // Learn the min and max of every feature from the data.
    void fit(const Dataset& data);

    // Apply the learned min and max to a dataset (new copy returned).
    Dataset transform(const Dataset& data) const;

    // fit() then transform() on the same data.
    Dataset fitTransform(const Dataset& data);

    bool isFitted() const { return fitted_; }

private:
    std::vector<double> min_;
    std::vector<double> max_;
    bool fitted_ = false;
};

}  // namespace edusvm