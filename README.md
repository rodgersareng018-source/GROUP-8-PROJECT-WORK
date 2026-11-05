# EduSVM – Student Performance Classification

## 1. Project Overview

EduSVM is a modular C++17 educational Support Vector Machine (SVM) library developed by Group 8.

The project demonstrates the mathematical foundations of binary classification using student academic features. The intended classification labels are:

- `+1` – PASS
- `-1` – FAIL

The project uses object-oriented programming and CMake for building and testing.

## 2. Implemented Features

The current implementation includes:

1. **Dataset representation and validation** – Stores feature matrices and binary labels and validates dataset structure.
2. **Data preprocessing** – Provides dataset validation and copying.
3. **Feature scaling** – Implements min-max normalization and handles constant-valued features.
4. **Hyperplane representation** – Represents an SVM hyperplane using weights and bias.
5. **Functional margin** – Calculates the functional margin of a labelled sample.
6. **Hinge loss** – Calculates hinge loss for an individual sample.
7. **L2 regularization** – Calculates the L2 regularization penalty.
8. **Object-oriented programming** – Demonstrates encapsulation, inheritance and runtime polymorphism.

## 3. Features Planned for Further Development

The following features are not yet available as complete library functionality:

- CSV dataset loading.
- Missing-value imputation.
- Gradient-based SVM training and optimization.
- Support-vector identification.
- A public PASS/FAIL prediction interface.
- Model evaluation metrics, including accuracy, precision, recall and F1-score.

These features should be implemented and tested before being described as completed.

## 4. Project Structure

```text
GROUP-8-PROJECT-WORK/
├── CMakeLists.txt
├── README.md
├── include/
│   └── edusvm/
├── src/
├── tests/
├── examples/
├── data/
└── reports/
```

## 5. Object-Oriented Design

The project demonstrates the three main OOP principles:

- **Encapsulation:** Private class members and controlled public access.
- **Inheritance:** `ValidationPreprocessor` and `ScalingPreprocessor` inherit from the `Preprocessor` base class.
- **Polymorphism:** Derived preprocessing classes override the virtual `process()` function.

The OOP demonstration is in `examples/example_oop.cpp`, with tests in `tests/test_oop.cpp`.

## 6. Requirements

- C++17-compatible compiler.
- CMake.
- Git.
- MinGW on Windows when using the MinGW Makefiles generator.

## 7. Installation and Build

### Clone the repository

```bash
git clone https://github.com/rodgersareng018-source/GROUP-8-PROJECT-WORK.git
cd GROUP-8-PROJECT-WORK
```

### Configure on Windows using MinGW

```bash
cmake -S . -B build -G "MinGW Makefiles"
```

### Build the project

```bash
cmake --build build
```

### Run the tests

```bash
ctest --test-dir build --output-on-failure
```

## 8. Testing

The project currently has eight registered test programs:

1. `test_dataset_basic`
2. `test_dataset_edges`
3. `test_preprocessor`
4. `test_scaler`
5. `test_svm`
6. `test_loss`
7. `test_regularization`
8. `test_oop`

**Latest verification:** All eight registered tests passed on the current main branch on 5 November 2026.

## 9. Group Contributions

The project was developed collaboratively by eight group members. Individual contributions and weekly progress are documented in the `reports/` directory and Git history.

## 10. Project Status

EduSVM has an integrated C++ library foundation, mathematical calculation modules, OOP demonstrations, automated build configuration and passing tests.

Further development is required to complete the full SVM training and student-classification workflow.