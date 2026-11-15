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

- `include/edusvm/` contains public data structures, class interfaces, and
  function declarations.
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

`DatasetLoader::loadFromCSV` and `loadDatasetCsv` in `src/dataset.cpp` read the
header and data rows. The last header must be named `label`; earlier columns
are treated as features. The loader checks row widths, parses numeric feature
values, rejects non-finite values and invalid labels, and only replaces the
caller's dataset after the entire file has loaded successfully.

`validateDataset` checks that the dataset is non-empty, every row has the same
non-zero number of features, the feature and label counts match, every
feature is finite, and every label is `-1` or `+1`.

### Preprocessing and scaling

`preprocessDataset` in `src/preprocessor.cpp` verifies the data before
training and copies it to its output when valid. `Preprocessor` defines a
virtual `process` operation. Validation, scaling, and imputation classes
implement that interface, so callers can use each operation through the same
base-class API. Imputation can replace missing values with column means or
drop rows that contain missing values.

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
The implementation separates training, decision evaluation, margin
calculation, prediction, and cross-validation into helper classes. The public
functions provide a simple interface to those operations.

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

### Scope coverage

The following table maps the eleven main scope areas to their implementation
and tests. It ends with module evaluation; cross-validation is included as an
evaluation workflow rather than counted as another scope area.

| Scope area | Implementation | Tests |
|---|---|---|
| 1. Dataset loading | `DatasetLoader::loadFromCSV` and `loadDatasetCsv` in `src/dataset.cpp` | `test_dataset_csv.cpp` |
| 2. Dataset validation | `DatasetValidator` and `validateDataset` | `test_dataset_basic.cpp`, `test_dataset_edges.cpp` |
| 3. Validation preprocessing | `DatasetPreprocessor` implements the `Preprocessor` interface | `test_preprocessor.cpp`, `test_oop.cpp` |
| 4. Missing-value imputation | `ImputationPreprocessor` supports mean replacement and dropping rows | `test_preprocessor.cpp` |
| 5. Feature scaling | `Scaler::fit`, `transform`, and `fitTransform` | `test_scaler.cpp` |
| 6. Hyperplane and margins | `Hyperplane`, `decisionFunction`, and `calculateFunctionalMargin` | `test_svm.cpp` |
| 7. Hinge loss | `calculateHingeLoss` | `test_loss.cpp` |
| 8. L2 regularization | `calculateL2Regularization` | `test_regularization.cpp` |
| 9. Training and binary prediction | `trainSvm` and `predictLabel` use labels `-1` and `+1` | `test_svm.cpp` |
| 10. Support-vector identification | `supportVectorIndices` stores training rows with functional margin at most `1.01` | `test_svm.cpp` checks that training identifies support vectors |
| 11. Module evaluation | `evaluateModel` calculates metrics; `crossValidateSvm` supplies pooled held-out predictions | `test_evaluation.cpp`, `test_svm.cpp` |

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

The test cases are separate classes derived from a shared `TestCase` interface.
Each class owns its test data and overrides `run()`. The test program calls
them through base-class pointers, demonstrating runtime polymorphism while
keeping each case independent.

## 5. Compilation and program flow

### From source files to executables

`CMakeLists.txt` selects C++17 and defines the library, demo, and test targets.
During configuration, CMake prepares the build files for the selected compiler.
During compilation, each `.cpp` file is compiled separately; the public headers
in `include/edusvm/` provide the declarations shared by those source files.

| Source file | Main responsibility |
|---|---|
| `src/dataset.cpp` | Reads CSV files and checks dataset contents. |
| `src/preprocessor.cpp` | Implements dataset validation preprocessing. |
| `src/preprocessor_strategies.cpp` | Implements validation, scaling, and missing-value strategies. |
| `src/scaler.cpp` | Learns feature ranges and scales dataset values. |
| `src/loss.cpp` | Calculates hinge loss. |
| `src/regularization.cpp` | Calculates the L2 penalty. |
| `src/svm.cpp` | Trains the model, predicts labels, calculates margins, and runs cross-validation. |
| `src/evaluation.cpp` | Calculates confusion counts and classification metrics. |

CMake links the compiled library sources into `edusvm_lib`. It then compiles
`main.cpp` and links it with that library to create the `edusvm` executable.
Each `tests/test_*.cpp` file is compiled into a separate test executable and
linked to the same library. CTest runs those executables and the demo smoke
test.

### What happens when the program runs

`main.cpp` accepts a CSV path, or uses `data/input/students.csv` when no path
is supplied. It calls `DatasetLoader::loadFromCSV` from `src/dataset.cpp`,
then validates and copies the data through `preprocessDataset` in
`src/preprocessor.cpp`. It chooses up to five folds, requiring at least two
rows from each label.

Next, `crossValidateSvm` in `src/svm.cpp` creates the folds. For each fold it
fits a `Scaler` using only the training rows, trains a model with
`SvmTrainer`, and evaluates predictions on the held-out rows through
`evaluateModel` in `src/evaluation.cpp`. The results are combined and printed
as accuracy, precision, recall, F1-score, and confusion counts. If loading,
validation, training, or evaluation fails, the program prints an error and
exits with a non-zero status.

### Build, run, and test

Run these commands from the repository root:

```powershell
cmake -S . -B build
cmake --build build --config Debug
ctest --test-dir build -C Debug --output-on-failure
.\build\edusvm.exe data\input\students.csv
```

Visual Studio generators usually place the executable in
`build\Debug\edusvm.exe` instead.

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

## 7. Project file guide

This guide covers the maintained project files. The eight implementation
files in `src/` are described in the compilation table in Section 5, and
`main.cpp` is described in the program-flow section there. Generated files in
`build/` and compiled `.exe` files are build outputs, not source files.

### Public headers

| File | What it contains |
|---|---|
| `include/edusvm/dataset.hpp` | The `Dataset` structure, CSV loader class, and dataset validation/loading declarations. |
| `include/edusvm/evaluation.hpp` | The `EvaluationMetrics` structure and model-evaluation declaration. |
| `include/edusvm/loss.hpp` | The hinge-loss function declaration. |
| `include/edusvm/preprocessor.hpp` | The dataset-validation preprocessor class and compatibility function. |
| `include/edusvm/preprocessor_base.hpp` | The abstract `Preprocessor` class and virtual `process` interface. |
| `include/edusvm/preprocessor_strategies.hpp` | The validation, scaling, and missing-value strategy classes and imputation options. |
| `include/edusvm/regularization.hpp` | The L2-regularization function declaration. |
| `include/edusvm/scaler.hpp` | The `Scaler` class interface for fitting and applying feature ranges. |
| `include/edusvm/svm.hpp` | The model and training-option structures and SVM operation declarations. |

### Tests

Each test executable has its own `main`. Most group checks into classes that
implement the shared `TestCase` interface; `test_oop.cpp` uses direct
assertions to demonstrate polymorphism.

| File | What it checks |
|---|---|
| `tests/test_dataset_basic.cpp` | Accepts a valid dataset and rejects empty data or mismatched label counts. |
| `tests/test_dataset_csv.cpp` | Loads the sample CSV and rejects missing files or malformed numeric rows. |
| `tests/test_dataset_edges.cpp` | Rejects missing features, uneven rows, NaN, infinity, and invalid labels. |
| `tests/test_evaluation.cpp` | Checks confusion counts and scores for perfect, imperfect, empty, and invalid evaluation inputs. |
| `tests/test_loss.cpp` | Checks hinge loss for positive and negative labels and rejects an invalid label. |
| `tests/test_oop.cpp` | Calls validation and scaling through `Preprocessor` pointers to demonstrate runtime polymorphism. |
| `tests/test_preprocessor.cpp` | Checks dataset validation plus mean and drop-row imputation behavior. |
| `tests/test_regularization.cpp` | Checks a known L2 result and rejects invalid lambda or non-finite weights. |
| `tests/test_scaler.cpp` | Checks ordinary and constant-feature scaling, fitted-state reuse, and invalid inputs. |
| `tests/test_svm.cpp` | Checks decision scores, margins, training, prediction, evaluation, and cross-validation. |

### Examples

| File | What it demonstrates |
|---|---|
| `examples/end_to_end.cpp` | Loads CSV data, preprocesses it, splits training and test rows, scales, trains, predicts, and evaluates. |
| `examples/example_calculations.cpp` | Runs validation, scaling, a fixed-model decision score, hinge loss, and L2 penalty on a small two-feature dataset. |
| `examples/example_calculations_1.cpp` | Demonstrates the same calculations with a three-feature dataset. |
| `examples/example_calculations (1).cpp` | A duplicate copy of the three-feature calculation example. |
| `examples/example_dataset.cpp` | Builds a sample dataset and reports valid and invalid dataset cases. |
| `examples/example_oop.cpp` | Applies validation and scaling strategies through their shared base-class interface. |

### Other project files

| File | What it contains |
|---|---|
| `CMakeLists.txt` | C++17 build rules for the library, demo, tests, CTest registration, and install package. |
| `cmake/EduSVMConfig.cmake.in` | Template that lets another CMake project import the installed EduSVM library target. |
| `.gitignore` | Build, executable, compiler, and temporary-file patterns that should not be committed. |
| `.vscode/settings.json` | Enables Markdown validation in VS Code. |
| `README.md` | Project overview, build instructions, CSV format, interfaces, classifier details, and limitations. |
| `data/input/students.csv` | Six illustrative student rows used by the demo and CSV tests; it is not real student data. |
| `examples/.gitkeep`, `reports/.gitkeep`, `tests/.gitkeep` | Empty marker files that keep these directories in version control. |
| `data/input/.gitkeep`, `data/output/.gitkeep` | Empty marker files that keep the input and output directories in version control. |
| `week 3 ngor ahok ayuen` | A standalone Week 3 progress note about the SVM module and its tests. |

### Reports

Weekly reports and old checklists are historical records; some describe the
project before training, evaluation, or imputation was added. The current
implementation is described in this final report and the README.

| File | What it contains |
|---|---|
| `reports/dataset-test-plan.md` | Dataset validation test cases and expected outcomes. |
| `reports/final-project-report.md` | Current project design, scope coverage, compilation flow, and limitations. |
| `reports/interface-review.md` | An earlier review of the public dataset interface and validation behavior. |
| `reports/requiements-checklist.md` | A historical requirements checklist with its original status notes. |
| `reports/week-01.md` | Week 1 progress, planned work, blockers, and AI-use notes. |
| `reports/week-02.md` | Week 2 technical review of formulas, modules, and test/build status at that time. |
| `reports/week-02-vivian.md` | Vivian's Week 2 formula review, findings, and recommendations. |
| `reports/week2_martha_regularization.md` | Martha's Week 2 work on the L2 regularization module and its test. |
| `reports/week3_martha.md` | A Week 3 explanation of the dataset-building and validation example. |
| `reports/week_03_report_rachaelaloro.md` | Rachael's Week 3 feature-scaling work and verification notes. |
| `reports/week-04.md` | Week 4 integration and OOP work, including the shared preprocessor design. |
| `reports/week-04-charity.md` | Charity's Week 4 preprocessing review and validation tests. |
| `reports/week-04-martha (1).md` | Martha's Week 4 report on a calculation/integration example. |
| `reports/week-04-monica.md` | Monica's Week 4 integration and test verification report. |
| `reports/week-04-ngor ahok ayuen.md` | Ngor Ahok's Week 4 SVM implementation and test notes. |
| `reports/week-04-rachaelaloro.md` | Rachael's Week 4 scaler review and test notes. |
| `reports/week-04-vivian.md` | Vivian's Week 4 formula checks, test summary, and outstanding work. |
| `reports/reports/dataset_format.md` | The sample CSV columns, labels, and CSV-loader test reference. |
| `reports/reports/requirements-checklist.md` | A short list of project requirements. |
| `reports/reports/week-03-test-review.md` | An earlier test review with expected calculations and gaps noted at the time. |
| `reports/reports/reports/requiements-checklist.md` | A historical detailed requirements/status checklist. |
| `reports/reports/reports/week-03-test-review.md` | A nested Week 3 test review with expected calculations and test gaps. |
| `reports/reports/reports/reports/requirements3-checklist.md` | Another historical requirements checklist with contributor and status notes. |
