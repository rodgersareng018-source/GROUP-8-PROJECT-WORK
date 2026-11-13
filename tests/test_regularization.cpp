// tests/test_regularization.cpp
// Verify the L2 penalty formula and its input validation.
#include <cassert>
#include <cmath>
#include <iostream>
#include <limits>
#include <string>
#include <vector>
#include "edusvm/regularization.hpp"

namespace {

class TestCase {
public:
    virtual ~TestCase() = default;
    virtual void run() const = 0;
};

class KnownValueTest final : public TestCase {
public:
    void run() const override {
        const std::vector<double> weights{2.0, 1.0};
        double penalty = 0.0;
        std::string error = "stale";

        const bool ok = edusvm::calculateL2Regularization(
            weights, 0.1, penalty, error
        );
        assert(ok);
        assert(std::fabs(penalty - 0.25) < 1e-9);
        assert(error.empty());
    }
};

class NegativeLambdaTest final : public TestCase {
public:
    void run() const override {
        const std::vector<double> weights{2.0, 1.0};
        double penalty = 0.0;
        std::string error;

        const bool ok = edusvm::calculateL2Regularization(
            weights, -0.1, penalty, error
        );
        assert(!ok);
        assert(!error.empty());
    }
};

class NonFiniteWeightTest final : public TestCase {
public:
    void run() const override {
        const std::vector<double> weights{1.0, std::numeric_limits<double>::quiet_NaN()};
        double penalty = 0.0;
        std::string error;

        const bool ok = edusvm::calculateL2Regularization(
            weights, 0.1, penalty, error
        );
        assert(!ok);
        assert(!error.empty());
    }
};

} // namespace

int main() {
    KnownValueTest knownValueTest;
    NegativeLambdaTest negativeLambdaTest;
    NonFiniteWeightTest nonFiniteTest;
    TestCase* tests[] = {
        &knownValueTest,
        &negativeLambdaTest,
        &nonFiniteTest
    };

    for (const auto* test : tests) {
        test->run();
    }

    std::cout << "All regularization tests passed.\n";
    return 0;
}
