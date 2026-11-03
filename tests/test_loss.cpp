
#include <cassert>
#include <cmath>
#include <string>
#include <vector>

#include "edusvm/loss.hpp"

int main() {
    std::vector<double> weights = {1.0, 1.0};
    std::vector<double> features = {1.0, 2.0};
    double bias = 0.0;
    double loss = 0.0;
    std::string error;

    // Positive example: score = 3, label = +1.
    bool ok = edusvm::calculateHingeLoss(
        weights, bias, features, 1, loss, error
    );
    assert(ok);
    assert(std::abs(loss - 0.0) < 1e-9);

    // Negative example: score = 3, label = -1.
    // Hinge loss = max(0, 1 - (-1 * 3)) = 4.
    ok = edusvm::calculateHingeLoss(
        weights, bias, features, -1, loss, error
    );
    assert(ok);
    assert(std::abs(loss - 4.0) < 1e-9);

    // Invalid label should be rejected.
    ok = edusvm::calculateHingeLoss(
        weights, bias, features, 0, loss, error
    );
    assert(!ok);

    return 0;
}
