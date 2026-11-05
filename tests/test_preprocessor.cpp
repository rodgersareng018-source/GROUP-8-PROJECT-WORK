#include "edusvm/preprocessor.hpp"
#include "edusvm/preprocessor_strategies.hpp"

#include <cassert>
#include <cmath>
#include <limits>
#include <string>
#include <vector>

namespace {

void testValidDataset() {
    edusvm::Dataset input;
    input.features = {{55.0, 40.0, 12.0}, {60.0, 45.0, 13.0}};
    input.labels = {-1, 1};

    edusvm::Dataset output;
    std::string error;
    assert(edusvm::preprocessDataset(input, output, error));
    assert(error.empty());
    assert(output.features == input.features);
    assert(output.labels == input.labels);
}

void testInvalidDatasetShapesAndValues() {
    std::string error;
    edusvm::Dataset output;

    edusvm::Dataset inconsistentRows;
    inconsistentRows.features = {{55.0, 40.0, 12.0}, {60.0, 45.0}};
    inconsistentRows.labels = {-1, 1};
    assert(!edusvm::preprocessDataset(inconsistentRows, output, error));
    assert(error.find("inconsistent feature count") != std::string::npos);

    edusvm::Dataset nonFinite;
    nonFinite.features = {
        {55.0, std::numeric_limits<double>::quiet_NaN(), 12.0},
        {60.0, 45.0, 13.0}
    };
    nonFinite.labels = {-1, 1};
    assert(!edusvm::preprocessDataset(nonFinite, output, error));
    assert(error.find("non-finite value") != std::string::npos);

    edusvm::Dataset invalidLabels;
    invalidLabels.features = {{55.0, 40.0}, {60.0, 45.0}};
    invalidLabels.labels = {-1, 5};
    assert(!edusvm::preprocessDataset(invalidLabels, output, error));
    assert(error.find("invalid label") != std::string::npos);

    edusvm::Dataset mismatchedLabels;
    mismatchedLabels.features = {{55.0, 40.0}, {60.0, 45.0}};
    mismatchedLabels.labels = {-1};
    assert(!edusvm::preprocessDataset(mismatchedLabels, output, error));
    assert(error.find("labels") != std::string::npos);
}

void testMeanImputation() {
    const double missing = std::numeric_limits<double>::quiet_NaN();
    edusvm::Dataset input;
    input.features = {{2.0, 60.0}, {missing, 80.0}, {4.0, missing}};
    input.labels = {-1, 1, 1};

    edusvm::Dataset output;
    std::string error;
    edusvm::ImputationPreprocessor imputer(
        edusvm::ImputationStrategy::MEAN
    );
    assert(imputer.process(input, output, error));
    assert(output.features.size() == 3);
    assert(output.features[1][0] == 3.0);
    assert(output.features[2][1] == 70.0);
    assert(output.labels == input.labels);
}

void testDropRowImputation() {
    const double missing = std::numeric_limits<double>::quiet_NaN();
    edusvm::Dataset input;
    input.features = {{2.0, 60.0}, {missing, 70.0}, {4.0, 80.0}};
    input.labels = {-1, 1, 1};

    edusvm::Dataset output;
    std::string error;
    edusvm::ImputationPreprocessor imputer(
        edusvm::ImputationStrategy::DROP_ROW
    );
    assert(imputer.process(input, output, error));
    assert(output.features.size() == 2);
    assert(output.labels == std::vector<int>({-1, 1}));
}

void testMeanImputationRejectsAllMissingColumn() {
    const double missing = std::numeric_limits<double>::quiet_NaN();
    edusvm::Dataset input;
    input.features = {{missing, 60.0}, {missing, 70.0}};
    input.labels = {-1, 1};

    edusvm::Dataset output;
    std::string error;
    edusvm::ImputationPreprocessor imputer(
        edusvm::ImputationStrategy::MEAN
    );
    assert(!imputer.process(input, output, error));
    assert(error.find("only missing values") != std::string::npos);
}

} // namespace

int main() {
    testValidDataset();
    testInvalidDatasetShapesAndValues();
    testMeanImputation();
    testDropRowImputation();
    testMeanImputationRejectsAllMissingColumn();
    return 0;
}
