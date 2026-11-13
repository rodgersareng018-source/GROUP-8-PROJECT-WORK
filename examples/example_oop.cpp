
#include <iostream>
#include <string>

#include "edusvm/preprocessor_strategies.hpp"

int main() {
    edusvm::Dataset dataset;

    dataset.features = {
        {2.0, 50.0},
        {4.0, 60.0},
        {6.0, 70.0},
        {8.0, 80.0}
    };

    dataset.labels = {-1, -1, 1, 1};

    edusvm::ValidationPreprocessor validation;
    edusvm::ScalingPreprocessor scaling;

    const edusvm::Preprocessor* processors[] = {
        &validation,
        &scaling
    };

    for (const auto* processor : processors) {
        edusvm::Dataset output;
        std::string errorMessage;

        std::cout << "\nProcessing: "
                  << processor->getName() << '\n';

        if (processor->process(dataset, output, errorMessage)) {
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
