
#include <cassert>
#include <cmath>
#include <iostream>
#include <memory>
#include <string>

#include "edusvm/preprocessor_strategies.hpp"

int main() {
    edusvm::Dataset data;

    data.features = {
        {2.0, 50.0},
        {4.0, 60.0},
        {6.0, 70.0},
        {8.0, 80.0}
    };

    data.labels = {-1, -1, 1, 1};

    edusvm::ValidationPreprocessor validation;
    edusvm::ScalingPreprocessor scaling;

    // Test encapsulated names.
    assert(validation.getName() == "Dataset Validation");
    assert(scaling.getName() == "Feature Scaling");

    // Test polymorphism through base-class pointers.
    const edusvm::Preprocessor* processors[] = {
        &validation,
        &scaling
    };

    edusvm::Dataset output;
    std::string errorMessage;

    // Test validation preprocessing.
    assert(processors[0]->process(data, output, errorMessage));
    assert(output.features == data.features);
    assert(output.labels == data.labels);

    // Test feature scaling.
    assert(processors[1]->process(data, output, errorMessage));

    assert(std::abs(output.features[0][0] - 0.0) < 1e-9);
    assert(std::abs(output.features[0][1] - 0.0) < 1e-9);
    assert(std::abs(output.features[3][0] - 1.0) < 1e-9);
    assert(std::abs(output.features[3][1] - 1.0) < 1e-9);

    // Labels should remain unchanged.
    assert(output.labels == data.labels);

    std::cout << "All OOP tests passed successfully!\n";

    return 0;
}
