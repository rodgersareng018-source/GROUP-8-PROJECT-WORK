# EduSVM - Student Performance Classification

## 1. Project Overview

EduSVM is a reusable educational C++17 Support Vector Machine (SVM) library developed by Group 8.

It demonstrates binary classification using numerical student academic performance features.

Classification labels:
- +1 = PASS
- -1 = FAIL

## 2. Implemented Features

- Dataset representation and validation.
- CSV dataset loading.
- Mean imputation and incomplete-row removal.
- Min-max feature scaling.
- Linear SVM training using batch subgradient descent.
- Decision function and functional margin.
- Hinge loss and L2 regularization.
- Support vector identification.
- PASS/FAIL prediction.
- Model evaluation using accuracy, precision, recall, F1 score and confusion matrix.
- Complete end-to-end classification example.

## 3. Project Structure

- include/edusvm/ - Public header files.
- src/ - C++ implementation files.
- tests/ - Automated test programs.
- examples/ - Demonstration programs.
- data/input/ - Input datasets.
- CMakeLists.txt - Build configuration.

## 4. Technologies

- C++17
- CMake
- MinGW/GCC
- Git and GitHub
- CTest

## 5. Build Instructions

Configure:

    cmake -S . -B build

Build:

    cmake --build build

## 6. Run the End-to-End Example

    .\build\end_to_end.exe

The example loads data/input/students.csv, separates training and testing records, scales features, trains the SVM, makes predictions and evaluates the results.

## 7. Dataset Format

The CSV dataset contains numerical features and a binary label.

Example columns:

    study_hours,attendance,assignment_score,label

Labels must be +1 (PASS) or -1 (FAIL).

## 8. Automated Testing

Run all registered tests:

    ctest --test-dir build --output-on-failure

The verified test suite contains 10 test groups covering datasets, CSV loading, preprocessing, scaling, SVM, loss, regularization, evaluation and OOP.

## 9. Evaluation Metrics

EduSVM calculates:
- Accuracy
- Precision
- Recall
- F1 score
- True positives and true negatives
- False positives and false negatives

The sample achieved 100% accuracy on two test records. This small result does not guarantee performance on larger datasets.

## 10. Project Status

The core library, end-to-end example and all 10 automated test groups have been built and verified successfully.

EduSVM demonstrates a modular and reusable educational SVM classification workflow.
