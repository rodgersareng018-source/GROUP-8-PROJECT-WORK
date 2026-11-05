# EduSVM Final Project Report

## 1. Project summary

EduSVM is a C++17 educational project that demonstrates a linear binary
Support Vector Machine (SVM) for classifying student records. It uses three
numeric features in the sample data—study hours, attendance, and assignment
score—and labels each row as `+1` (PASS) or `-1` (FAIL).

The project is organized as a reusable C++ library, a command-line demo, and a
set of module tests. Its intended flow is:

1. Read student records from a CSV file.
2. Check that the rows, values, and labels are valid.
3. Split data into stratified cross-validation folds.
4. Fit feature scaling on each training fold only.
5. Train a linear classifier on the scaled training rows.
6. Predict labels for the held-out rows.
7. Pool the predictions and calculate classification metrics.

## 2. Project structure

- `include/edusvm/` contains public data structures and function declarations.
- `src/` contains the corresponding library implementations.
- `main.cpp` connects the library into a command-line program.
- `tests/` contains standalone tests, including `test_svm.cpp`.
- `examples/` contains small programs demonstrating individual APIs.
- `data/input/students.csv` contains the sample student dataset.
- `cmake/` contains configuration used when installing and exporting the
  library for other CMake projects.
- `reports/` contains progress notes, reviews, and this report.

The main build configuration is `CMakeLists.txt`. It builds the library,
command-line executable, test executables, and installable CMake package.

## 3. How the main code works

### Data representation and CSV loading

`edusvm::Dataset`, declared in `include/edusvm/dataset.hpp`, stores features as
a two-dimensional vector of numbers. Each inner vector is one student's
feature row. A separate labels vector stores the corresponding `-1` or `+1`
class for each row.

`loadDatasetCsv` in `src/dataset.cpp` reads the header and data rows. The last
header must be named `label`; earlier columns are treated as features. It
checks the row widths, parses numeric feature values, rejects non-finite
values and invalid labels, and only replaces the caller's dataset after the
entire file has loaded successfully.

`validateDataset` checks that the dataset is non-empty, every row has the same
non-zero number of features, the feature and label counts match, every
feature is finite, and every label is `-1` or `+1`.

### Preprocessing and scaling

`preprocessDataset` in `src/preprocessor.cpp` verifies the data before
training and copies it to its output when valid. The strategy classes in
`preprocessor_base.hpp` and `preprocessor_strategies.hpp` provide a common
interface for validation and scaling steps.

`Scaler`, declared in `include/edusvm/scaler.hpp` and implemented in
`src/scaler.cpp`, learns a minimum and maximum for each feature column. It
maps values with `(value - minimum) / (maximum - minimum)`. Constant-valued
columns map to zero rather than dividing by zero. During cross-validation, a
separate scaler is fitted on each fold's training rows to avoid using
validation-fold information during training.

### Loss and regularization

`calculateHingeLoss` in `src/loss.cpp` computes the per-row hinge loss:

```text
max(0, 1 - label * (weights dot features + bias))
```

`calculateL2Regularization` in `src/regularization.cpp` computes
`lambda / 2 * sum(weight * weight)`. This penalty discourages unnecessarily
large model weights.

### SVM operations

The public SVM declarations are in `include/edusvm/svm.hpp`. Training,
prediction, and cross-validation are implemented in `src/svm.cpp`. The
evaluation API and its `EvaluationMetrics` type are declared in
`include/edusvm/evaluation.hpp` and implemented in `src/evaluation.cpp`.

- `Hyperplane` contains a weight for each feature, a bias, and indices of
  training rows identified near the decision margin.
- `TrainingOptions` configures the regularization amount, initial learning
  rate, and number of training epochs.
- `trainSvm` checks the dataset and training options, initializes the
  parameters, and applies batch subgradient updates to reduce average hinge
  loss plus the L2 weight penalty. Its learning rate decreases as training
  proceeds.
- `decisionFunction` calculates `weights dot features + bias`. A positive
  score predicts `+1`; a negative score predicts `-1`.
- `calculateFunctionalMargin` multiplies the score by the true label. The
  result describes whether a row is correctly classified and how far it is
  from the decision boundary.
- `predictLabel` converts the decision score to `+1` or `-1`.
- `evaluateModel` compares predictions with known labels, counts true
  positives, false positives, true negatives, and false negatives, then
  calculates accuracy, precision, recall, and F1-score. It is declared with
  `EvaluationMetrics` in `include/edusvm/evaluation.hpp`.
- `crossValidateSvm` shuffles each class deterministically, distributes its
  rows across folds, and trains/evaluates one model per fold. It aggregates
  the held-out predictions so every row is evaluated once.

### Command-line program

`main.cpp` accepts an optional CSV path. If none is provided, it uses
`data/input/students.csv`. It loads and validates the data, selects up to five
folds while requiring at least two records from each class, calls
`crossValidateSvm`, and prints the sample size, metrics, and confusion
matrix. The sample data has only six records, so its reported metrics are
demonstrative and not a reliable estimate of real-world performance.

## 4. SVM test implementation

The SVM implementation is **not** inside a test file under `src/`. Training,
prediction, and cross-validation are implemented in `src/svm.cpp`, with model
evaluation in `src/evaluation.cpp`. Their test files are
`tests/test_svm.cpp` and `tests/test_evaluation.cpp`.

The current `tests/test_svm.cpp` includes cases intended to check:

1. A known positive decision score, a negative score, and a score of zero.
2. Rejection of a model/input feature-count mismatch.
3. Functional-margin calculation.
4. Training on a small linearly separable dataset and prediction on new rows.
5. Confusion-matrix counts and accuracy, precision, recall, and F1-score.
6. Rejection of a training dataset that contains only one class.
7. Cross-validation producing one held-out prediction per row and rejecting
   more folds than the class counts allow.

The test cases are now organized into independent functions that own their
test data and are called by `main`. This avoids the previous scope error, in
which test code referred to a model local to another function, and ensures the
training, prediction, evaluation, and cross-validation cases actually run.

## 5. Build, tests, and current status

The intended commands from the repository root are:

```powershell
cmake -S . -B build
cmake --build build --config Debug
ctest --test-dir build -C Debug --output-on-failure
```

The project has CMake install rules for the library, headers, demo, sample
CSV, and package files. The README documents how a downstream CMake project
can find and link the installed `EduSVM::edusvm_lib` target.

**Verified status:** the current CMake build completes, and all 11 tests
registered with CTest pass, including the demo smoke test. During repair, the
missing `ImputationStrategy` enum was added to the preprocessing interface,
`<cmath>` was included for its numeric checks, and the preprocessing tests
were repaired so their imputation cases run from one valid `main`.

The SVM implementation was brought back in line with the public
`TrainingOptions` API by removing an incompatible duplicate training
implementation. Evaluation has a dedicated public header and metrics type,
is compiled into the library, and has its own registered test target.
The SVM test file was reorganized so every test runs with valid local fixtures.
These results apply to the CMake-registered build and tests on the current
checkout.

## 6. Limitations and next steps for future use

- The sample CSV contains only six rows. Add a larger representative dataset
  with trusted labels before using the metrics to make performance claims.
- Add more representative, genuinely labeled student records before drawing
  conclusions about real-world model performance.
