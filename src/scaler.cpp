#include "edusvm/scaler.hpp"

#include <cmath>
#include <stdexcept>

namespace edusvm {

namespace {

void validateData(const Dataset& data, std::size_t expectedFeatures) {
    if (data.features.empty()) {
        throw std::invalid_argument("Scaler: dataset is empty");
    }
    if (data.labels.size() != data.features.size()) {
        throw std::invalid_argument("Scaler: labels and rows count differ");
    }

    const std::size_t width = data.features[0].size();
    if (width == 0) {
        throw std::invalid_argument("Scaler: rows have no features");
    }
    if (expectedFeatures != 0 && width != expectedFeatures) {
        throw std::invalid_argument("Scaler: wrong number of features");
    }

    for (const auto& row : data.features) {
        if (row.size() != width) {
            throw std::invalid_argument("Scaler: rows have different sizes");
        }
        for (double value : row) {
            if (!std::isfinite(value)) {
                throw std::invalid_argument("Scaler: value is NaN or infinite");
            }
        }
    }
}

}  // namespace

void Scaler::fit(const Dataset& data) {
    validateData(data, 0);

    // Start with the first row, then update each feature's bounds.
    const std::size_t width = data.features[0].size();
    min_ = data.features[0];
    max_ = data.features[0];

    for (const auto& row : data.features) {
        for (std::size_t j = 0; j < width; ++j) {
            if (row[j] < min_[j]) min_[j] = row[j];
            if (row[j] > max_[j]) max_[j] = row[j];
        }
    }
    fitted_ = true;
}

Dataset Scaler::transform(const Dataset& data) const {
    if (!fitted_) {
        throw std::logic_error("Scaler: call fit() before transform()");
    }
    validateData(data, min_.size());

    Dataset result = data;
    for (auto& row : result.features) {
        for (std::size_t j = 0; j < row.size(); ++j) {
            const double range = max_[j] - min_[j];
            // A constant feature has no range, so map it to zero.
            row[j] = (range == 0.0) ? 0.0 : (row[j] - min_[j]) / range;
        }
    }
    return result;
}

Dataset Scaler::fitTransform(const Dataset& data) {
    fit(data);
    return transform(data);
}

}  // namespace edusvm