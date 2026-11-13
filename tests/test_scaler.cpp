#include <cmath>
#include <iostream>
#include <limits>
#include <stdexcept>

#include "edusvm/scaler.hpp"

using edusvm::Dataset;
using edusvm::Scaler;

namespace {

class TestCase {
public:
    virtual ~TestCase() = default;
    virtual void run() const = 0;
};

static int failures = 0;

#define CHECK(condition)                                              \
    do {                                                              \
        if (!(condition)) {                                           \
            std::cout << "FAILED line " << __LINE__ << ": "           \
                      << #condition << "\n";                          \
            ++failures;                                               \
        }                                                             \
    } while (0)

static bool near(double value, double expected) {
    return std::fabs(value - expected) < 1e-9;
}

class OrdinaryValuesTest final : public TestCase {
public:
    void run() const override {
        Dataset dataset;
        dataset.features = {{10.0, 0.0}, {20.0, 50.0}, {30.0, 100.0}};
        dataset.labels = {1, -1, 1};

        Scaler scaler;
        Dataset scaled = scaler.fitTransform(dataset);

        CHECK(near(scaled.features[0][0], 0.0));
        CHECK(near(scaled.features[1][0], 0.5));
        CHECK(near(scaled.features[2][0], 1.0));
        CHECK(near(scaled.features[1][1], 0.5));
        CHECK(scaled.features.size() == 3);
        CHECK(scaled.labels == dataset.labels);
    }
};

class ConstantFeatureTest final : public TestCase {
public:
    void run() const override {
        Dataset dataset;
        dataset.features = {{5.0, 1.0}, {5.0, 2.0}};
        dataset.labels = {1, -1};

        Scaler scaler;
        Dataset scaled = scaler.fitTransform(dataset);

        CHECK(near(scaled.features[0][0], 0.0));
        CHECK(near(scaled.features[1][0], 0.0));
        CHECK(near(scaled.features[1][1], 1.0));
    }
};

class SameParametersOnNewDataTest final : public TestCase {
public:
    void run() const override {
        Dataset trainingData;
        trainingData.features = {{0.0}, {10.0}};
        trainingData.labels = {1, -1};
        Scaler scaler;
        scaler.fit(trainingData);

        Dataset newData;
        newData.features = {{5.0}};
        newData.labels = {1};
        CHECK(near(scaler.transform(newData).features[0][0], 0.5));
    }
};

class EmptyInputTest final : public TestCase {
public:
    void run() const override {
        Scaler scaler;
        bool threw = false;
        try {
            scaler.fit(Dataset{});
        } catch (const std::invalid_argument&) {
            threw = true;
        }
        CHECK(threw);
    }
};

class InconsistentRowsTest final : public TestCase {
public:
    void run() const override {
        Dataset dataset;
        dataset.features = {{1.0, 2.0}, {3.0}};
        dataset.labels = {1, -1};
        Scaler scaler;
        bool threw = false;
        try {
            scaler.fit(dataset);
        } catch (const std::invalid_argument&) {
            threw = true;
        }
        CHECK(threw);
    }
};

class NonFiniteValueTest final : public TestCase {
public:
    void run() const override {
        Dataset dataset;
        dataset.features = {{1.0}, {std::numeric_limits<double>::quiet_NaN()}};
        dataset.labels = {1, -1};
        Scaler scaler;
        bool threw = false;
        try {
            scaler.fit(dataset);
        } catch (const std::invalid_argument&) {
            threw = true;
        }
        CHECK(threw);
    }
};

class MismatchedLabelsTest final : public TestCase {
public:
    void run() const override {
        Dataset dataset;
        dataset.features = {{1.0}, {2.0}};
        dataset.labels = {1};

        Scaler scaler;
        bool threw = false;
        try {
            scaler.fit(dataset);
        } catch (const std::invalid_argument&) {
            threw = true;
        }
        CHECK(threw);
    }
};

class WrongFeatureCountTest final : public TestCase {
public:
    void run() const override {
        Dataset trainingData;
        trainingData.features = {{1.0, 2.0}, {3.0, 4.0}};
        trainingData.labels = {1, -1};

        Scaler scaler;
        scaler.fit(trainingData);

        Dataset testData;
        testData.features = {{5.0}};
        testData.labels = {1};

        bool threw = false;
        try {
            scaler.transform(testData);
        } catch (const std::invalid_argument&) {
            threw = true;
        }
        CHECK(threw);
    }
};

class TransformBeforeFitTest final : public TestCase {
public:
    void run() const override {
        Dataset dataset;
        dataset.features = {{1.0}};
        dataset.labels = {1};
        Scaler scaler;
        bool threw = false;
        try {
            scaler.transform(dataset);
        } catch (const std::logic_error&) {
            threw = true;
        }
        CHECK(threw);
    }
};

} // namespace

int main() {
    OrdinaryValuesTest ordinaryTest;
    ConstantFeatureTest constantTest;
    SameParametersOnNewDataTest newDataTest;
    EmptyInputTest emptyTest;
    InconsistentRowsTest inconsistentRowsTest;
    NonFiniteValueTest nonFiniteTest;
    MismatchedLabelsTest mismatchedLabelsTest;
    WrongFeatureCountTest wrongFeatureCountTest;
    TransformBeforeFitTest transformBeforeFitTest;
    TestCase* tests[] = {
        &ordinaryTest,
        &constantTest,
        &newDataTest,
        &emptyTest,
        &inconsistentRowsTest,
        &nonFiniteTest,
        &mismatchedLabelsTest,
        &wrongFeatureCountTest,
        &transformBeforeFitTest
    };

    for (const auto* test : tests) {
        test->run();
    }

    if (failures == 0) {
        std::cout << "All scaler tests passed\n";
        return 0;
    }
    std::cout << failures << " check(s) failed\n";
    return 1;
}