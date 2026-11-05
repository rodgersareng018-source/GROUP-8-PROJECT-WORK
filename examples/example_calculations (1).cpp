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
    // Demonstrate validation and model calculations on the sample records.
    edusvm::Dataset data;

    // These records mirror data/input/students.csv for a self-contained example.
    // Columns: study_hours, attendance, assignment_score. Label: 1 = PASS, -1 = FAIL.
    data.features = {
        {1.0, 55.0, 40.0},
        {2.0, 60.0, 45.0},
        {3.0, 70.0, 65.0},
        {5.0, 85.0, 80.0},
        {4.0, 78.0, 72.0},
        {1.5, 50.0, 35.0}
    };

    data.labels = {-1, -1, 1, 1, 1, -1};

    std::string error;

    // Reject invalid feature shapes, values, or labels before computation.
    if (!edusvm::validateDataset(data, error)) {
        std::cerr << "Dataset error: " << error << '\n';
        return 1;
    }

    // Preprocessing checks the data and returns a validated copy.
    edusvm::Dataset processed;
    if (!edusvm::preprocessDataset(data, processed, error)) {
        std::cerr << "Preprocessing error: " << error << '\n';
        return 1;
    }

    // Learn scaling bounds from this dataset and apply them to all rows.
    edusvm::Scaler scaler;
    edusvm::Dataset scaled = scaler.fitTransform(processed);

    // Hand-set parameters keep this example focused on scoring and losses;
    // trainSvm() can learn parameters automatically in a full pipeline.
    edusvm::Hyperplane model;
    model.weights = {1.0, 1.0, 1.0};
    model.bias = -1.0;

    // Use the third student (index 2) for the single-row calculations.
    const std::size_t row = 2;

    // Apply the hyperplane equation: weights dot features plus bias.
    double decision = 0.0;
    if (!edusvm::decisionFunction(
            model, scaled.features[row], decision, error)) {
        std::cerr << "Decision error: " << error << '\n';
        return 1;
    }

    // Compare the score with the student's known class to compute hinge loss.
    double hingeLoss = 0.0;
    if (!edusvm::calculateHingeLoss(
            model.weights, model.bias, scaled.features[row],
            scaled.labels[row], hingeLoss, error)) {
        std::cerr << "Loss error: " << error << '\n';
        return 1;
    }

    // Compute the L2 size penalty for the hand-set weights.
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
