
1.Reusable C++ library with a public interface
Group:
include/edusvm/*.hpp, CMake target edusvm
Implemented but not tested
Six headers (dataset, preprocessor, scaler, svm, loss, regularization) are built into one library. Needs a clean build to confirm.

2.Implementation separated into source files
Group:
src/*.cpp
Implemented but not tested
Each module has its own .cpp file. 

3.Dataset validation
Charity, Komdit, Levy:
dataset.hpp, dataset.cpp, tests/test_dataset_basic.cpp, tests/test_dataset_edges.cpp
Implemented but not tested.

4.Preprocessing
Charity:
preprocessor.hpp, preprocessor.cpp, tests/test_preprocessor.cpp
Implemented and tested
This only validates and copies the data. It does not handle missing values or outliers.
implemented strict precondition checks to ensure data integrity before model execution.

5.Feature scaling (min-max)
Rachael:
scaler.hpp, scaler.cpp, tests/test_scaler.cpp
Implemented but not tested
Minimum maps to 0, maximum to 1, constant features map to 0, mismatched scaler is rejected.
all 7 tests passed when compiled with g++

6.Hyperplane and margin calculations
Komdit:
svm.hpp, svm.cpp, tests/test_svm.cpp
In progress
Decision function f(x) = w.x + b and functional margin y * f(x) exist. Geometric margin (dividing by the length of w) is not implemented yet.

7.Hinge loss
Levy
loss.hpp, loss.cpp, tests/test_loss.cpp
Implemented 
Formula max(0, 1 - y f(x)).
standardized error propagationusing boolean status returns and string message references rather than relying on silent failiures or unhandled exceptions.

8.Regularization (L2 penalty)
Martha:
regularization.hpp, regularization.cpp, tests/test_regularization.cpp
Implemented but not tested
Formula (lambda/2) * sum of squared weights. It is calculated separately and not yet combined with hinge loss into one training objective.

9.Binary prediction (PASS = 1, FAIL = -1)
Komdit:
svm.cpp
In progress
The decision score is calculated (positive = PASS, negative = FAIL). 
There is no separate predict function and no trained weights yet.

10.Automated tests including edge cases
All:
tests/
In progress
Seven test files are planned. Edge cases still missing: infinity in the dataset (Levy), invalid scaler range, zero-length weights.

11.Example demonstrating library use
Rodgers, Martha:
examples/example_calculations.cpp
Implemented but not tested
Demonstration only, not a trained model. Martha's sample CSV is data/input/students.csv, and CSV loading does not exist yet.

12.CMake build configuration
Monica:
CMakeLists.txt
Implemented but not tested
Registers seven tests and the example. Needs a confirmed clean build.

13.Group-written README
Group
README.md
Not started
 We will do this after the implementation is finished.

14.Weekly and final reports
Rodgers, Vivian:
reports/
In progress
No final report yet.

15.Individual contributions documented
All:
reports/, commit history
In progress

16.Extra requirements from the instructor's brief
Vivian:
In progress
I still need to compare this list with the instructor's brief and add anything missing.

Not started: 
optimization
training
support vectors
evaluation metrics
README

So, the next steps are;
Monica confirms a clean build and shares the full ctest output.
Levy adds the infinity test.
The group decides who owns the optimizer, training, support vectors and evaluation metrics.
Komdit and Levy agree whether geometric margin and a combined objective are needed.
I update the statuses above after each merged pull request.