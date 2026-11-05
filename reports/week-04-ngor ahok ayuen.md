# Week 4 Progress Report - ngor ahok ayuen

## 1. Work Completed
- Created and implemented the SVM Hyperplane struct (`include/edusvm/svm.hpp`).
- Implemented the decision function `f(x) = w · x + b` and functional margin calculations (`src/svm.cpp`).
- Created comprehensive unit tests for the SVM module (`tests/test_svm.cpp`).

## 2. Tests Performed and Results
- Tested the decision function with weights {2.0, 1.0}, bias -3.0, and features {2.0, 1.0}. The result correctly returned 2.0.
- Tested mismatched feature counts and confirmed the function correctly rejects them with an error message.
- Tested invalid labels (e.g., 0) and confirmed the margin function rejects them.
- Note: Tests are written using `assert` and are ready to be run in the CMake build by Monica.

## 3. Challenges Encountered
- **Challenge:** Using the online GitHub web editor meant I could not run the CMake build or `ctest` locally in a terminal. 
- **Solution:** I verified the mathematical logic manually and relied on Monica's integrated CMake setup to run the final build.

## 4. Remaining Work
- My individual SVM module is complete.
- Waiting for final integration into `main` and for the group README/documentation to be finalized.
