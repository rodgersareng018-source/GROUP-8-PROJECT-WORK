#include "edusvm/dataset.hpp"

#include <cassert>
#include <iostream>
#include <string>

namespace {

class TestCase {
public:
    virtual ~TestCase() = default;
    virtual void run() const = 0;
};

class EmptyDatasetTest final : public TestCase {
public:
    void run() const override {
        std::string error;
        edusvm::Dataset empty;
        assert(!edusvm::validateDataset(empty, error));
        assert(!error.empty());
    }
};

class ValidDatasetTest final : public TestCase {
public:
    void run() const override {
        std::string error;
        edusvm::Dataset valid;
        valid.features = {{2.0, 80.0}, {5.0, 95.0}};
        valid.labels = {-1, 1};
        assert(edusvm::validateDataset(valid, error));
        assert(error.empty());
    }
};

class MismatchedLabelsTest final : public TestCase {
public:
    void run() const override {
        std::string error;
        edusvm::Dataset mismatch;
        mismatch.features = {{2.0, 80.0}, {5.0, 95.0}};
        mismatch.labels = {1};
        assert(!edusvm::validateDataset(mismatch, error));
        assert(!error.empty());
    }
};

} // namespace

int main() {
    EmptyDatasetTest emptyTest;
    ValidDatasetTest validTest;
    MismatchedLabelsTest mismatchTest;
    TestCase* tests[] = {&emptyTest, &validTest, &mismatchTest};

    for (const auto* test : tests) {
        test->run();
    }

    std::cout << "Basic dataset tests passed.\n";
    return 0;
}