#include <cassert>
#include <cmath>
#include <string>
#include <vector>

#include "edusvm/loss.hpp"

namespace {

class TestCase {
public:
    virtual ~TestCase() = default;
    virtual void run() const = 0;
};

class PositiveLabelLossTest final : public TestCase {
public:
    void run() const override {
        const std::vector<double> weights = {1.0, 1.0};
        const std::vector<double> features = {1.0, 2.0};
        const double bias = 0.0;
        double loss = 0.0;
        std::string error;

        const bool ok = edusvm::calculateHingeLoss(
            weights, bias, features, 1, loss, error
        );
        assert(ok);
        assert(std::abs(loss - 0.0) < 1e-9);
    }
};

class NegativeLabelLossTest final : public TestCase {
public:
    void run() const override {
        const std::vector<double> weights = {1.0, 1.0};
        const std::vector<double> features = {1.0, 2.0};
        const double bias = 0.0;
        double loss = 0.0;
        std::string error;

        const bool ok = edusvm::calculateHingeLoss(
            weights, bias, features, -1, loss, error
        );
        assert(ok);
        assert(std::abs(loss - 4.0) < 1e-9);
    }
};

class InvalidLabelLossTest final : public TestCase {
public:
    void run() const override {
        const std::vector<double> weights = {1.0, 1.0};
        const std::vector<double> features = {1.0, 2.0};
        const double bias = 0.0;
        double loss = 0.0;
        std::string error;

        const bool ok = edusvm::calculateHingeLoss(
            weights, bias, features, 0, loss, error
        );
        assert(!ok);
    }
};

} // namespace

int main() {
    PositiveLabelLossTest positiveTest;
    NegativeLabelLossTest negativeTest;
    InvalidLabelLossTest invalidTest;
    TestCase* tests[] = {&positiveTest, &negativeTest, &invalidTest};

    for (const auto* test : tests) {
        test->run();
    }

    return 0;
}
