
#include <iostream>

#include "edusvm/preprocessor_strategies.hpp"

int main() {
    // A small dataset containing two numeric features per student.
    edusvm::Dataset data;

    data.features = {
        {2.0, 50.0},
        {4.0, 60.0},
        {6.0, 70.0},
        {8.0, 80.0}
    };

    data.labels = {-1, -1, 1, 1};

    // Each concrete class implements one operation from the Preprocessor API.
    edusvm::ValidationPreprocessor validation;
    edusvm::ScalingPreprocessor scaling;

    // Base-class pointers let the same loop invoke different implementations.
    const edusvm::Preprocessor* processors[] = {
        &validation,
        &scaling
    };

    // Run each processing strategy and display either output or its error.
    for (const auto* processor : processors) {
        edusvm::Dataset output;
        std::string errorMessage;

        std::cout << "\nProcessing: "
                  << processor->getName() << '\n';

        if (processor->process(data, output, errorMessage)) {
            std::cout << "Processing successful.\n";
            std::cout << "First feature values: ";

            for (double value : output.features[0]) {
                std::cout << value << " ";
            }

            std::cout << "\n";
        }
        else {
            std::cout << "Error: "
                      << errorMessage << '\n';
        }
    }

    return 0;
}
