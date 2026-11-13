#include "edusvm/dataset.hpp"

#include <cassert>
#include <limits>
#include <string>

namespace {

class TestCase {
public:
    virtual ~TestCase() = default;
    virtual void run() const = 0;
};

class NoFeaturesTest final : public TestCase {
public:
    void run() const override {
        std::string error;
        edusvm::Dataset noFeatures;
        noFeatures.features = {{}};
        noFeatures.labels = {1};

        assert(!edusvm::validateDataset(noFeatures, error));
    }
};

class UnequalRowsTest final : public TestCase {
public:
    void run() const override {
        std::string error;
        edusvm::Dataset unequalRows;
        unequalRows.features = {{1.0, 2.0}, {3.0}};
        unequalRows.labels = {-1, 1};

        assert(!edusvm::validateDataset(unequalRows, error));
    }
};

class NaNValueTest final : public TestCase {
public:
    void run() const override {
        std::string error;
        edusvm::Dataset nanData;
        nanData.features = {{
            std::numeric_limits<double>::quiet_NaN()
        }};
        nanData.labels = {1};

        assert(!edusvm::validateDataset(nanData, error));
    }
};

class InfiniteValueTest final : public TestCase {
public:
    void run() const override {
        std::string error;
        edusvm::Dataset infiniteData;
        infiniteData.features = {{
            std::numeric_limits<double>::infinity()
        }};
        infiniteData.labels = {1};

        assert(!edusvm::validateDataset(infiniteData, error));
    }
};

class InvalidLabelTest final : public TestCase {
public:
    void run() const override {
        std::string error;
        edusvm::Dataset invalidLabel;
        invalidLabel.features = {{2.0}};
        invalidLabel.labels = {0};

        assert(!edusvm::validateDataset(invalidLabel, error));
    }
};

} // namespace

int main() {
    NoFeaturesTest noFeaturesTest;
    UnequalRowsTest unequalRowsTest;
    NaNValueTest nanValueTest;
    InfiniteValueTest infiniteValueTest;
    InvalidLabelTest invalidLabelTest;
    TestCase* tests[] = {
        &noFeaturesTest,
        &unequalRowsTest,
        &nanValueTest,
        &infiniteValueTest,
        &invalidLabelTest
    };

    for (const auto* test : tests) {
        test->run();
    }

    return 0;
}
