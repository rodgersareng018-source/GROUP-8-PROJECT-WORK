
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
    edusvm::Dataset data;

    data.features = {
        {1.0, 55.0},
        {2.0, 60.0},
        {3.0, 70.0},
        {5.0, 85.0}
    };

    data.labels = {-1, -1, 1, 1};

    std::string error;

    if (!edusvm::validateDataset(data, error)) {
        std::cerr << "Dataset error: " << error << '\n';
        return 1;
    }

    edusvm::Dataset processed;
    if (!edusvm::preprocessDataset(data, processed, error)) {
        std::cerr << "Preprocessing error: " << error << '\n';
        return 1;
    }

    edusvm::Scaler scaler;
    edusvm::Dataset scaled = scaler.fitTransform(processed);

    edusvm::Hyperplane model;
    model.weights = {1.0, 1.0};
    model.bias = -1.0;

    double decision = 0.0;
    if (!edusvm::decisionFunction(
            model, scaled.features[0], decision, error)) {
        std::cerr << "Decision error: " << error << '\n';
        return 1;
    }

    double hingeLoss = 0.0;
    if (!edusvm::calculateHingeLoss(
            model.weights, model.bias, scaled.features[0],
            scaled.labels[0], hingeLoss, error)) {
        std::cerr << "Loss error: " << error << '\n';
        return 1;
    }

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
