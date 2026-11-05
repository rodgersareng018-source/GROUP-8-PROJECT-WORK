#include <cmath>
#include <iostream>
#include <limits>
#include <stdexcept>

#include "edusvm/scaler.hpp"

using edusvm::Dataset;
using edusvm::Scaler;

static int failures = 0;

// Keep floating-point comparisons tolerant of tiny rounding differences.
#define CHECK(condition)                                              \
    do {                                                              \
        if (!(condition)) {                                           \
            std::cout << "FAILED line " << __LINE__ << ": "           \
                      << #condition << "\n";                          \
            ++failures;                                               \
        }                                                             \
    } while (0)

static bool near(double a, double b) { return std::fabs(a - b) < 1e-9; }

// Check that each feature column is independently mapped into [0, 1].
static void testOrdinaryValues() {
    Dataset d;
    d.features = {{10.0, 0.0}, {20.0, 50.0}, {30.0, 100.0}};
    d.labels = {1, -1, 1};

    Scaler s;
    Dataset out = s.fitTransform(d);

    CHECK(near(out.features[0][0], 0.0));
    CHECK(near(out.features[1][0], 0.5));
    CHECK(near(out.features[2][0], 1.0));
    CHECK(near(out.features[1][1], 0.5));
    CHECK(out.features.size() == 3);
    CHECK(out.labels == d.labels);
}

// A constant column has a zero range and is defined to scale to all zeros.
static void testConstantFeature() {
    Dataset d;
    d.features = {{5.0, 1.0}, {5.0, 2.0}};
    d.labels = {1, -1};

    Scaler s;
    Dataset out = s.fitTransform(d);

    CHECK(near(out.features[0][0], 0.0));
    CHECK(near(out.features[1][0], 0.0));
    CHECK(near(out.features[1][1], 1.0));
}

// transform() must reuse min/max learned from training, not refit on new rows.
static void testSameParametersOnNewData() {
    Dataset train;
    train.features = {{0.0}, {10.0}};
    train.labels = {1, -1};
    Scaler s;
    s.fit(train);

    Dataset fresh;
    fresh.features = {{5.0}};
    fresh.labels = {1};
    CHECK(near(s.transform(fresh).features[0][0], 0.5));
}

// Fitting requires at least one valid row.
static void testEmptyInput() {
    Scaler s;
    bool threw = false;
    try { s.fit(Dataset{}); } catch (const std::invalid_argument&) { threw = true; }
    CHECK(threw);
}

// The scaler cannot compare columns when rows have different widths.
static void testInconsistentRows() {
    Dataset d;
    d.features = {{1.0, 2.0}, {3.0}};
    d.labels = {1, -1};
    Scaler s;
    bool threw = false;
    try { s.fit(d); } catch (const std::invalid_argument&) { threw = true; }
    CHECK(threw);
}

// Non-finite measurements cannot define reliable min/max bounds.
static void testNonFiniteValue() {
    Dataset d;
    d.features = {{1.0}, {std::numeric_limits<double>::quiet_NaN()}};
    d.labels = {1, -1};
    Scaler s;
    bool threw = false;
    try { s.fit(d); } catch (const std::invalid_argument&) { threw = true; }
    CHECK(threw);
}
static void testMismatchedLabels() {
    Dataset d;
    d.features = {{1.0}, {2.0}};
    d.labels = {1};

    Scaler s;
    bool threw = false;

    try {
        s.fit(d);
    } catch (const std::invalid_argument&) {
        threw = true;
    }

    CHECK(threw);
}
// Transforming requires the same number of columns used during fit().
static void testWrongFeatureCount() {
    Dataset train;
    train.features = {{1.0, 2.0}, {3.0, 4.0}};
    train.labels = {1, -1};

    Scaler s;
    s.fit(train);

    Dataset test;
    test.features = {{5.0}};
    test.labels = {1};

    bool threw = false;

    try {
        s.transform(test);
    } catch (const std::invalid_argument&) {
        threw = true;
    }

    CHECK(threw);
}
// The scaler has no bounds to apply until fit() has been called.
static void testTransformBeforeFit() {
    Dataset d;
    d.features = {{1.0}};
    d.labels = {1};
    Scaler s;
    bool threw = false;
    try { s.transform(d); } catch (const std::logic_error&) { threw = true; }
    CHECK(threw);
}

int main() {
    // Run normal behavior and error-path checks.
    testOrdinaryValues();
    testConstantFeature();
    testSameParametersOnNewData();
    testEmptyInput();
    testInconsistentRows();
    testNonFiniteValue();
    testMismatchedLabels();
    testWrongFeatureCount();
    testTransformBeforeFit();

    if (failures == 0) {
        std::cout << "All scaler tests passed\n";
        return 0;
    }
    std::cout << failures << " check(s) failed\n";
    return 1;
}