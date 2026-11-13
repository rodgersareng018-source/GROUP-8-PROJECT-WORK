#include "edusvm/dataset.hpp"

#include <cassert>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <string>

namespace {

class TestCase {
public:
    virtual ~TestCase() = default;
    virtual void run() const = 0;
};

class ValidLoadTest final : public TestCase {
public:
    void run() const override {
        edusvm::Dataset dataset;
        std::string errorMessage;
        assert(edusvm::loadDatasetCsv(
            EDUSVM_TEST_CSV_PATH, dataset, errorMessage
        ));
        assert(errorMessage.empty());
        assert(dataset.features.size() == 6);
        assert(dataset.features.front().size() == 3);
        assert(dataset.labels.size() == 6);
        assert(dataset.labels.front() == -1);
        assert(dataset.labels.back() == -1);
    }
};

class FailedLoadTest final : public TestCase {
public:
    void run() const override {
        edusvm::Dataset dataset;
        std::string errorMessage;
        assert(edusvm::loadDatasetCsv(
            EDUSVM_TEST_CSV_PATH, dataset, errorMessage
        ));

        const auto originalFeatures = dataset.features;
        assert(!edusvm::loadDatasetCsv(
            "edusvm-file-that-does-not-exist.csv", dataset, errorMessage
        ));
        assert(!errorMessage.empty());
        assert(dataset.features == originalFeatures);
    }
};

class InvalidNumericRowTest final : public TestCase {
public:
    void run() const override {
        edusvm::Dataset dataset;
        std::string errorMessage;
        assert(edusvm::loadDatasetCsv(
            EDUSVM_TEST_CSV_PATH, dataset, errorMessage
        ));
        const auto originalFeatures = dataset.features;

        const auto tempName = "edusvm_invalid_dataset_" +
            std::to_string(std::chrono::steady_clock::now()
                .time_since_epoch().count()) + ".csv";
        const auto invalidCsv =
            std::filesystem::temp_directory_path() / tempName;
        {
            std::ofstream output(invalidCsv);
            assert(output);
            output << "feature,label\nnot-a-number,1\n";
        }
        assert(!edusvm::loadDatasetCsv(
            invalidCsv.string(), dataset, errorMessage
        ));
        assert(errorMessage.find("Invalid feature") != std::string::npos);
        assert(dataset.features == originalFeatures);
        std::filesystem::remove(invalidCsv);
    }
};

} // namespace

int main() {
    ValidLoadTest validLoadTest;
    FailedLoadTest failedLoadTest;
    InvalidNumericRowTest invalidRowTest;
    TestCase* tests[] = {&validLoadTest, &failedLoadTest, &invalidRowTest};

    for (const auto* test : tests) {
        test->run();
    }

    return 0;
}
