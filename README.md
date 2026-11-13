# EduSVM

EduSVM is a small C++17 educational project that demonstrates a linear binary
Support Vector Machine for student-performance data. It has no third-party
library dependencies.

The program reads numeric features from a CSV file, validates and scales the
data, trains a linear classifier, and prints predictions summarized as
confusion-matrix counts, accuracy, precision, recall, and F1-score.

## Build and run the demo

On a fresh machine, install Git, CMake 3.16 or newer, and a C++17 compiler
(for example, Visual Studio with the C++ workload on Windows, or GCC/Clang on
Linux/macOS). Then clone and build from a terminal:

```sh
git clone https://github.com/rodgersareng018-source/GROUP-8-PROJECT-WORK.git
cd GROUP-8-PROJECT-WORK
cmake -S . -B build
cmake --build build --config Release
ctest --test-dir build -C Release --output-on-failure
```

Run the sample dataset from the repository root (PowerShell):

```powershell
.\build\Release\edusvm.exe data\input\students.csv
```

For single-configuration generators, the executable may instead be at
`build\edusvm.exe`. The CSV argument is optional; when run from the repository
root, it reads `data/input/students.csv`.

## Install and use the C++ library

The install step copies the library, public headers, demo executable, sample
CSV, and CMake package files to a prefix. Choose a prefix you can write to.
For example, to install under a local `install` folder:

```powershell
cmake --install build --config Release --prefix "$PWD\install"
```

On Unix-like shells, use `--prefix "$PWD/install"` instead. To install system
wide on Linux/macOS, omit `--prefix` and use the platform's normal
administrator privileges.

The installed library can be used by another CMake project. In its
`CMakeLists.txt`, locate the package and link the exported target:

```cmake
find_package(EduSVM 1 CONFIG REQUIRED)
add_executable(my_app main.cpp)
target_link_libraries(my_app PRIVATE EduSVM::edusvm_lib)
```

Configure that consumer project with the install prefix so CMake can find
EduSVM:

```powershell
cmake -S . -B build -DCMAKE_PREFIX_PATH="C:\path\to\GROUP-8-PROJECT-WORK\install"
cmake --build build --config Release
```

Replace the example path with the actual absolute path to the install
directory. The installed CSV is under `share/edusvm/students.csv` (or the
platform's equivalent data directory). The demo accepts a CSV path, so pass
that installed file when running the installed executable.

## CSV format

The first non-empty row is a header. It must contain at least one feature
column, and its final column must be named `label` (case-insensitive). Each
following non-empty row must have the same number of columns. Feature values
must be finite numbers, and labels must be `-1` or `+1`.

The included `data/input/students.csv` uses `study_hours`, `attendance`, and
`assignment_score` as features. A label of `+1` represents PASS and `-1`
represents FAIL. The file loader reports malformed rows and does not replace
the destination dataset on failure.

## Project layout

- `include/edusvm/` contains public data structures, module interfaces, and classes.
- `src/` implements CSV loading, validation, scaling, hinge loss, L2
  regularization, and SVM training/prediction/evaluation.
- `main.cpp` connects those modules into the command-line demo.
- `tests/` contains tests for the modules and an executable smoke test; CMake
  builds and registers all of them with CTest.
- `data/input/students.csv` is the sample input dataset.
- `examples/` contains standalone examples, and `reports/` contains project
  progress and review documents.

## Object-oriented design

`Preprocessor` defines a common virtual `process` operation. Validation,
scaling, and imputation classes implement that operation in their own way.
`Scaler` keeps its fitted feature bounds inside the object. SVM training,
prediction, and cross-validation use helper classes behind the public
functions, keeping the library interface simple.

## Classifier details and evaluation

The scaler learns each feature's minimum and maximum from each training fold
and maps values to `[0, 1]`. Constant-valued features map to `0`.
The trainer uses batch subgradient descent on average hinge loss plus an L2
weight penalty; the learning rate decays by the square root of the epoch
number. The default settings are 1,000 epochs, an initial learning rate of
`0.1`, and a regularization coefficient (`lambda`) of `0.01`. Training data
must contain both labels.

The demo uses deterministic, stratified k-fold cross-validation, with up to
five folds and at least one example of each class in every fold. Each row is
predicted by a model that was not trained on that row, and the reported
confusion matrix and scores are pooled across those out-of-fold predictions.
Scaling is fitted independently on each fold's training rows to avoid
information leaking from the validation rows. At least two examples of each
class are required.

Cross-validation makes better use of a small dataset than a single holdout,
but it cannot make a six-row dataset representative. The sample's scores are
still preliminary and can change substantially with new records. Use a larger,
representative, genuinely labeled dataset before drawing conclusions about
real-world performance; do not duplicate records to inflate the sample size.

The public SVM interface also lets callers train and evaluate using their own
datasets and `TrainingOptions`. Support vectors are training rows whose final
functional margin is at most `1.01`; their indices refer to the training
dataset supplied to `trainSvm`.

## Useful interfaces

- `DatasetLoader::loadFromCSV` / `loadDatasetCsv`: load a CSV dataset.
- `preprocessDataset` / `validateDataset`: validate rows, labels, and values.
- `ValidationPreprocessor`, `ScalingPreprocessor`, and
  `ImputationPreprocessor`: use the shared preprocessing interface.
- `Scaler`: fit and apply min-max feature scaling.
- `calculateHingeLoss` and `calculateL2Regularization`: compute individual
  objective components.
- `trainSvm`, `decisionFunction`, `predictLabel`, and `crossValidateSvm`:
  train and use the linear SVM.
- `EvaluationMetrics` and `evaluateModel` in `edusvm/evaluation.hpp`:
  calculate classification metrics separately from model training.
