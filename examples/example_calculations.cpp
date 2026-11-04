
#include <iostream>
#include <string>
#include <vector>

#include "edusvm/dataset.hpp"
#include "edusvm/preprocessor.hpp"
#include "edusvm/scaler.hpp"
#include "edusvm/svm.hpp"
#include "edusvm/loss.hpp"
#include "edusvm/regularization.hpp"

int main() {
    // Build a small dataset directly in code to demonstrate the library APIs.
    edusvm::Dataset data;

    data.features = {
        {1.0, 55.0},
        {2.0, 60.0},
        {3.0, 70.0},
        {5.0, 85.0}
    };

    data.labels = {-1, -1, 1, 1};

    std::string error;

    // Validate the dataset before passing it through later processing steps.
    if (!edusvm::validateDataset(data, error)) {
        std::cerr << "Dataset error: " << error << '\n';
        return 1;
    }

    // Preprocessing verifies rows and labels, then copies the unchanged data.
    edusvm::Dataset processed;
    if (!edusvm::preprocessDataset(data, processed, error)) {
        std::cerr << "Preprocessing error: " << error << '\n';
        return 1;
    }

    // Fit per-feature min/max bounds and scale a separate dataset copy.
    edusvm::Scaler scaler;
    edusvm::Dataset scaled = scaler.fitTransform(processed);

    // Set example parameters explicitly so the score and losses are predictable.
    edusvm::Hyperplane model;
    model.weights = {1.0, 1.0};
    model.bias = -1.0;

    // Calculate the signed score for the first scaled student row.
    double decision = 0.0;
    if (!edusvm::decisionFunction(
            model, scaled.features[0], decision, error)) {
        std::cerr << "Decision error: " << error << '\n';
        return 1;
    }

    // Hinge loss is based on the row's true label and its model score.
    double hingeLoss = 0.0;
    if (!edusvm::calculateHingeLoss(
            model.weights, model.bias, scaled.features[0],
            scaled.labels[0], hingeLoss, error)) {
        std::cerr << "Loss error: " << error << '\n';
        return 1;
    }

    // L2 regularization measures the size of the model's weight vector.
    double penalty = 0.0;
    if (!edusvm::calculateL2Regularization(
            model.weights, 0.1, penalty, error)) {
        std::cerr << "Regularization error: " << error << '\n';
        return 1;
    }

    std::cout << "EduSVM integration example\n";
    std::cout << "Dataset rows: " << scaled.features.size() << '\n';
    std::cout << "Decision value: " << decision << '\n';
    std::cout << "Hinge loss: " << hingeLoss << '\n';
    std::cout << "L2 regularization penalty: " << penalty << '\n';

    return 0;
}
