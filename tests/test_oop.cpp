
#include <cassert>
#include <cmath>
#include <iostream>
#include <memory>
#include <string>

#include "edusvm/preprocessor_strategies.hpp"

int main() {
    // Provide two features per student and labels that exercise both classes.
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

    // Confirm each concrete strategy keeps the name passed to its base class.
    assert(validation.getName() == "Dataset Validation");
    assert(scaling.getName() == "Feature Scaling");

    // Store both strategies through their common interface to demonstrate
    // runtime polymorphism.
    const edusvm::Preprocessor* processors[] = {
        &validation,
        &scaling
    };

    edusvm::Dataset output;
    std::string errorMessage;

    // Validation copies good data without changing feature values or labels.
    assert(processors[0]->process(data, output, errorMessage));
    assert(output.features == data.features);
    assert(output.labels == data.labels);

    // Scaling maps the minimum of each column to 0 and the maximum to 1.
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
