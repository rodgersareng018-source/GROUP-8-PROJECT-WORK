#include "edusvm/dataset.hpp"

#include <cassert>
#include <limits>
#include <string>

int main() {
    std::string error;

    // A student with no features must be rejected.
    edusvm::Dataset noFeatures;
    noFeatures.features = {{}};
    noFeatures.labels = {1};

    assert(!edusvm::validateDataset(noFeatures, error));

    // Rows with different numbers of features must be rejected.
    edusvm::Dataset unequalRows;
    unequalRows.features = {{1.0, 2.0}, {3.0}};
    unequalRows.labels = {-1, 1};

    assert(!edusvm::validateDataset(unequalRows, error));

    // NaN must be rejected.
    edusvm::Dataset nanData;
    nanData.features = {{
        std::numeric_limits<double>::quiet_NaN()
    }};
    nanData.labels = {1};

    assert(!edusvm::validateDataset(nanData, error));

    // Infinite values must be rejected.
    edusvm::Dataset infiniteData;
    infiniteData.features = {{
        std::numeric_limits<double>::infinity()
    }};
    infiniteData.labels = {1};

    assert(!edusvm::validateDataset(infiniteData, error));

    // An invalid label must be rejected.
    edusvm::Dataset invalidLabel;
    invalidLabel.features = {{2.0}};
    invalidLabel.labels = {0};

    assert(!edusvm::validateDataset(invalidLabel, error));

    return 0;
}
