Week 04 Report: Dataset and Example

 Martha Baranga

Scope
Verify the sample student-performance dataset, check that labels and features follow the project format, review and update 'examples/example_calculations.cpp', check that it demonstrates the library's workflow, and document how to run it.

Verified sample data
File: 'data/input/students.csv'

It has a header row and 6 sample records with four columns: 'study_hours', 'attendance', 'assignment_score' and 'label'. The label convention is 1 = PASS and -1 = FAIL. The values are illustrative sample data, not real student records.


study_hours,attendance,assignment_score,label
1,55,40,-1
2,60,45,-1
3,70,65,1
5,85,80,1
4,78,72,1
1.5,50,35,-1

The file was verified with 'tests/check_students_csv.cpp', a standalone C++ program. It checks that:


- the header is exactly the four expected column names,
- every row has 4 columns,
- the three feature values are finite numbers, with study hours not negative and attendance and assignment score between 0 and 100,
- every label is 1 or -1, with both PASS and FAIL present,
- no row is duplicated.

Result: 6 rows (3 PASS, 3 FAIL), all checks passed. The checker was also run on deliberately bad files (a label of 2, a non-numeric value, a row with 3 columns) and rejected each one.

This checks the file only. It does not test any project loader.

FORMAT CHECK AGAINST LIBRARY
- 'Dataset' holds 'features' (a vector of rows, each a vector of doubles) and 'labels' (a vector of ints). The CSV labels of 1 and -1 fit 'labels'.
- Each CSV row has three feature values, so the example now uses three features per row, matching the CSV.

EXAMPLE
'examples/example_calculations.cpp' was updated so its data matches 'students.csv' (6 rows, 3 features) and the hand-set model has one weight per feature. It demonstrates this workflow:

1. Build a 'Dataset' in code.
2. Validate it with 'validateDataset'.
3. Preprocess it with 'preprocessDataset'.
4. Scale it with 'Scaler::fitTransform'.
5. Compute the decision value for the third student with 'decisionFunction'.
6. Compute that student's hinge loss with 'calculateHingeLoss'.
7. Compute the L2 penalty with 'calculateL2Regularization'.
8. Print the row count, decision value, hinge loss and penalty.

Each call checks its error message and exits with an error if it fails, except 'fitTransform', which has no error output in its declaration.

What was verified

- Compiles against the real headers: 'g++ -std=c++17 -Wall -Wextra -fsyntax-only' against 'dataset.hpp', 'preprocessor.hpp', 'scaler.hpp', 'svm.hpp', 'loss.hpp' and 'regularization.hpp' gave no errors or warnings, so every call matches its declaration.

- Runs end to end with stand-in code: the example was linked and run using simple stand-in versions of the dataset, preprocessor, scaler, SVM and loss functions, written only for this check, together with the real 'regularization.cpp'. It finished with exit code 0 and printed all four result lines. The L2 penalty was 0.15, which matches (0.1 / 2) x (1 + 1 + 1). Assuming preprocessing leaves the data unchanged, the hand calculation for the third student gives a decision value of about 0.738 and a hinge loss of about 0.262, and the run matched both.

NOT VERIFIED: the example has not been run against the team's real implementation files for the other modules. Output from those may differ, for example if 'preprocessDataset' changes the data.

HOW TO RUN THE EXAMPLE

Build the project from the root folder with CMake, then run the example executable:

cmake -S . -B build
cmake --build build
./build/<example target name>


The executable name comes from the root 'CMakeLists.txt'. On success the program prints "EduSVM integration example", followed by the dataset row count, decision value, hinge loss and L2 regularization penalty. To check the data file on its own:

g++ -std=c++17 tests/check_students_csv.cpp -o check_csv
./check_csv

Limitations
-CSV loading: no CSV-loading function exists in the headers reviewed, so the example uses data written in code and never reads `students.csv`. CSV loading is unfinished and is not claimed to work.

- Model weights: the weights (1.0, 1.0, 1.0) and bias (-1.0) are set by hand. No training function appears in `svm.hpp`, so the example does not train a model.
- Loss and penalty: hinge loss and the L2 penalty are shown separately for one student. Combining them into the full training objective belongs to a later integration step.
- Build: a clean CMake build of the example and tests is Monica's check and has not been confirmed here.
