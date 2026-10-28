 Week 3 Test and Requirements Review
Purpose;
   To check the EduSVM modules against the project requirements, calculate the expected results independently, and record gaps, failed tests and missing functionality honestly.

Expected results
I worked these out myself so I can compare them with the test and program output.

| Module | Input | My calculation | Expected result |
|---|---|---|---|
| Min-max scaling | value 50, range 0 to 100 | (50 - 0) / (100 - 0) | 0.5 |
| Min-max scaling | constant feature (max = min) | defined as 0 in scaler.cpp | 0 |
| Decision function | w = {2, 1}, b = -3, x = {2, 1} | 2(2) + 1(1) - 3 | 2 |
| Functional margin | same model, label 1 | 1 x 2 | 2 |
| Functional margin | same model, label -1 | -1 x 2 | -2 |
| Hinge loss | label 1, score 2 | max(0, 1 - 2) | 0 |
| Hinge loss | label 1, score 0.5 | max(0, 1 - 0.5) | 0.5 |
| Hinge loss | label -1, score 2 | max(0, 1 + 2) | 3 |
| L2 penalty | w = {2, 1}, lambda = 0.1 | (0.1 / 2)(4 + 1) | 0.25 |

Rodgers's example program (examples/example_calculations.cpp)
Data: {1,55}, {2,60}, {3,70}, {5,85}. Feature minimums are {1, 55} and maximums are {5, 85}.

| Step | My calculation | Expected |
|---|---|---|
| First student scaled | (1-1)/4 and (55-55)/30 | {0, 0} |
| Second student scaled | 1/4 and 5/30 | {0.25, 0.1667} |
| Third student scaled | 2/4 and 15/30 | {0.5, 0.5} |
| Fourth student scaled | 4/4 and 30/30 | {1, 1} |
| Model | w = {1, 1}, b = -1 | |
| Decision score (first student) | 1(0) + 1(0) - 1 | -1 |
| Hinge loss (label -1) | max(0, 1 - (-1)(-1)) = max(0, 0) | 0 |
| L2 penalty (lambda 0.1) | (0.1 / 2)(1 + 1) | 0.1 |

The program should print: Decision score: -1, Hinge loss: 0, L2 penalty: 0.1.

Sample dataset (data/input/students.csv)

Expected: 1 header line and 6 data rows. Each row has 4 values. Labels: 3 PASS (1) and 3 FAIL (-1).

Test coverage by module

| Module | Test file | Valid input | Invalid input | Edge cases | Gaps |
|---|---|---|---|---|---|
| Dataset | test_dataset_basic.cpp, test_dataset_edges.cpp | valid 2x2 dataset accepted | empty, label mismatch, no features, unequal rows, NaN, label 0 | NaN, empty | Infinity test not in the original code. Levy must add it. |
| Preprocessor | test_preprocessor.cpp | data copied correctly | invalid data rejected | none | Missing values are not handled, so they cannot be tested. |
| Scaler | test_scaler.cpp | min to 0, max to 1 | wrong feature count | constant feature | Invalid range (min greater than max) and non-finite scaler values are not tested. |
| Decision function / margin | test_svm.cpp | score = 2 | wrong feature count, invalid label | none | Empty weights, NaN weights and NaN bias are not tested. |
| Hinge loss | test_loss.cpp | losses 0 and 0.5 | label 0 | none | Label -1 case and a feature count mismatch are not tested. |
| Regularization | test_regularization.cpp | penalty 0.25 | negative lambda | none | Empty weights (penalty 0) and NaN weights are not tested. |
| Example | examples/example_calculations.cpp | runs and prints values | error paths stop the program | none | It is a demonstration, not a test. |

Problems and risks found
1.Regularization file location.
2.Missing infinity test.The code only tests NaN.
3.assert-based tests.The tests use `assert`. If anyone builds in Release mode (`-DNDEBUG`), the assertions are skipped and tests would pass without checking anything.
4.Preprocessing is basic.It only validates and copies. We should not claim it handles missing values or outliers.
5.Margin.Only the functional margin is implemented, not the geometric margin.
6.CSV loading.No function loads `students.csv` yet, so the file is sample data only.
7.Regularization and hinge loss are calculated separately and are not yet combined into one training objective.

 Missing functionality
  Gradient-based optimizer
  Model training
  Support vector identification
  Evaluation metrics (accuracy and others)
  A dedicated prediction function using trained weights
  CSV loading
  Group-written README

So, the next steps are;
1.Monica shares the full `ctest` output, and I update the checklist from it.
2.Levy adds the infinity test.
3.Group assigns owners for the optimizer, training, support vectors and evaluation.
4.Each owner adds tests for the gaps listed in section 2.
5.I recheck the checklist after each merged pull request.

AI use
Tool: ChatGPT
Purpose: explained the project structure and the Git workflow, and suggested starter code and tests.
 Reason: we are beginners and needed help understanding the steps.
 Human review: I calculated the expected results by hand and compared them with the program output.
