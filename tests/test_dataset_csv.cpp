
#include "edusvm/dataset.hpp"

#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>

using namespace edusvm;

bool writeTestFile(
    const std::string& fileName,
    const std::string& contents
) {
    std::ofstream file(fileName);

    if (!file.is_open()) {
        return false;
    }

    file << contents;
    return file.good();
}

bool expectFailure(const std::string& fileName) {
    Dataset dataset;
    std::string errorMessage;

    const bool result = DatasetLoader::loadFromCSV(
        fileName, dataset, errorMessage
    );

    return !result && !errorMessage.empty();
}

int main() {
    int passed = 0;
    int failed = 0;

    // Test 1: Load the actual student dataset.
    {
        Dataset dataset;
        std::string errorMessage;

        const bool result = DatasetLoader::loadFromCSV(
            "data/input/students.csv",
            dataset,
            errorMessage
        );

        if (result &&
            dataset.features.size() == 6 &&
            dataset.labels.size() == 6 &&
            dataset.features[0].size() == 3 &&
            dataset.features[0][0] == 1.0 &&
            dataset.features[0][1] == 55.0 &&
            dataset.features[0][2] == 40.0 &&
            dataset.labels[0] == -1 &&
            dataset.labels[2] == 1) {
            std::cout << "PASS: Valid student CSV loads correctly.\n";
            ++passed;
        } else {
            std::cout << "FAIL: Valid student CSV did not load correctly. "
                      << errorMessage << '\n';
            ++failed;
        }
    }

    // Test 2: Reject a file that does not exist.
    if (expectFailure("file_that_does_not_exist.csv")) {
        std::cout << "PASS: Missing file is rejected.\n";
        ++passed;
    } else {
        std::cout << "FAIL: Missing file was not rejected correctly.\n";
        ++failed;
    }

    // Test 3: Reject an invalid class label.
    {
        const std::string fileName = "csv_test_invalid_label.csv";

        if (writeTestFile(
                fileName,
                "hours,attendance,label\n"
                "2,60,0\n"
            ) &&
            expectFailure(fileName)) {
            std::cout << "PASS: Invalid label is rejected.\n";
            ++passed;
        } else {
            std::cout << "FAIL: Invalid label test failed.\n";
            ++failed;
        }

        std::remove(fileName.c_str());
    }

    // Test 4: Reject a non-numeric feature.
    {
        const std::string fileName = "csv_test_bad_number.csv";

        if (writeTestFile(
                fileName,
                "hours,attendance,label\n"
                "abc,60,-1\n"
            ) &&
            expectFailure(fileName)) {
            std::cout << "PASS: Non-numeric feature is rejected.\n";
            ++passed;
        } else {
            std::cout << "FAIL: Non-numeric feature test failed.\n";
            ++failed;
        }

        std::remove(fileName.c_str());
    }

    // Test 5: Reject a row with the wrong number of columns.
    {
        const std::string fileName = "csv_test_wrong_columns.csv";

        if (writeTestFile(
                fileName,
                "hours,attendance,label\n"
                "2,60\n"
            ) &&
            expectFailure(fileName)) {
            std::cout << "PASS: Incorrect column count is rejected.\n";
            ++passed;
        } else {
            std::cout << "FAIL: Incorrect column count test failed.\n";
            ++failed;
        }

        std::remove(fileName.c_str());
    }

    // Test 6: Reject an empty file.
    {
        const std::string fileName = "csv_test_empty.csv";

        if (writeTestFile(fileName, "") &&
            expectFailure(fileName)) {
            std::cout << "PASS: Empty CSV file is rejected.\n";
            ++passed;
        } else {
            std::cout << "FAIL: Empty CSV file test failed.\n";
            ++failed;
        }

        std::remove(fileName.c_str());
    }

    std::cout << "\nCSV tests passed: " << passed << '\n';
    std::cout << "CSV tests failed: " << failed << '\n';

    return failed == 0 ? 0 : 1;
}
