#include "edusvm/dataset.hpp"

#include <cassert>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <string>

int main() {
    // Load the checked-in sample and verify its dimensions and binary labels.
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

    // Failed loads must leave an already-populated destination untouched.
    const auto originalFeatures = dataset.features;
    assert(!edusvm::loadDatasetCsv(
        "edusvm-file-that-does-not-exist.csv", dataset, errorMessage
    ));
    assert(!errorMessage.empty());
    assert(dataset.features == originalFeatures);

    // Create a malformed numeric row to verify the parser reports bad input.
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
    return 0;
}
