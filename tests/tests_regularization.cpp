// tests/test_regularization.cpp
// Penalty only; combining with hinge loss is a later integration step.
#include <cmath>
#include <iostream>
#include <limits>
#include <string>
#include <vector>
#include "edusvm/regularization.hpp"

class RegularizationTest {
public:
    int run() {
        testKnownValue();
        testNegativeLambdaRejected();
        testNonFiniteWeightRejected();
        std::cout << (failures_ ? "FAILED\n" : "All regularization tests passed.\n");
        return failures_ ? 1 : 0;
    }

private:
    std::vector<double> weights_{2.0, 1.0};
    double penalty_ = 0.0;
    std::string error_;
    int failures_ = 0;

    bool call(double lambda, const std::vector<double>& w) {
        return edusvm::calculateL2Regularization(w, lambda, penalty_, error_);
    }
    void check(bool ok, const char* msg) {
        if (!ok) { std::cerr << "FAIL: " << msg << "\n"; ++failures_; }
    }

    void testKnownValue() {
        error_ = "stale";
        check(call(0.1, weights_), "valid input succeeds");
        check(std::fabs(penalty_ - 0.25) < 1e-9, "penalty is 0.05*(4+1) = 0.25");
        check(error_.empty(), "error cleared on success");
    }
    void testNegativeLambdaRejected() {
        check(!call(-0.1, weights_), "negative lambda rejected");
        check(!error_.empty(), "negative lambda sets error message");
    }
    void testNonFiniteWeightRejected() {
        check(!call(0.1, {1.0, std::numeric_limits<double>::quiet_NaN()}), "NaN weight rejected");
        check(!error_.empty(), "NaN weight sets error message");
    }
};

int main() { return RegularizationTest().run(); }
