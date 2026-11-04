
#include <cassert>
#include <cmath>
#include <string>
#include <vector>

#include "edusvm/loss.hpp"

int main() {
    // Use a simple two-feature model so the decision score is easy to check.
    std::vector<double> weights = {1.0, 1.0};
    std::vector<double> features = {1.0, 2.0};
    double bias = 0.0;
    double loss = 0.0;
    std::string error;

    // A positive score with a positive label clears the margin, so loss is 0.
    bool ok = edusvm::calculateHingeLoss(
        weights, bias, features, 1, loss, error
    );
    assert(ok);
    assert(std::abs(loss - 0.0) < 1e-9);

    // The same score is wrong for a negative label: max(0, 1 - (-1 * 3)) = 4.
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
