Week 2 Formula Review and Code Verification
1.Objective
The objective of this review was to examine the main mathematical formulas used in the EduSVM project, compare them with the C++ implementations, identify potential issues, and recommend improvements for reliability and integration.

2.Modules Reviewed
 2.1 SVM Decision Function
The SVM decision function calculates a score using the model weights, input features, and bias
Formula:

\[
f(x)=w\cdot x+b
\]

The implementation in `src/svm.cpp` calculates the weighted sum of the features and adds the bias.

The implementation also checks that:
- The number of features matches the number of weights.
- The bias, weights, and features are finite.
- The final decision score is finite.

Finding:The formula and the inspected validation checks are consistent with the expected decision function.
2.2 Functional Margin
     The functional margin measures how well a labelled example satisfies the classification decision.

Formula:

\[
m=yf(x)
\]

where \(y\) must be either \(+1\) or \(-1\).

 The implementation in `src/svm.cpp` validates the label and uses the decision function to calculate the margin.

Finding:The formula matches the implementation.

 2.3 Hinge Loss
       Hinge loss measures the classification margin violation for a labelled example.

Formula:

\[
L=\max(0,1-y(w\cdot x+b))
\]

The implementation in `src/loss.cpp` uses this formula and checks that the label is either \(+1\) or \(-1\), and that the feature and weight counts match.
Finding:The hinge-loss formula is correct.
Recommendation: Add validation for non-finite bias, feature, and weight values. The current implementation does not perform the same numeric validation as the SVM decision function.

2.4 L2 Regularization
L2 regularization penalizes large model weights.
Formula:

\[
R(w)=\frac{\lambda}{2}\sum_{i=1}^{n}w_i^2
\]

The implementation in `src/regularization.cpp` checks that lambda is finite and non-negative, checks that all weights are finite, calculates the squared-weight sum, and verifies that the resulting penalty is finite.
Finding:The formula and inspected validation checks match the expected L2 regularization penalty.

 2.5 Feature Scaling
The scaler uses min-max normalization.
Formula:

\[
x'=\frac{x-x_{\min}}{x_{\max}-x_{\min}}
\]

The implementation in `src/scaler.cpp` calculates the minimum and maximum for each feature and applies the formula during transformation.
It also handles constant features by assigning them a scaled value of zero when the feature range is zero.
Additional checks cover empty datasets, inconsistent row sizes, mismatched label counts, non-finite feature values, and transformation before fitting.

Finding:The inspected implementation supports the stated min-max scaling and constant-feature handling.
Recommendation:Fit the scaler on training data only and reuse the fitted scaler for validation, test, and prediction data to avoid data leakage.

2.6 Preprocessing
The implementation in `src/preprocessor.cpp` calls `validateDataset()`. If validation succeeds, it copies the input dataset to the output.
  Feature scaling is handled separately by the scaler module.

Finding:The preprocessor validates and copies the dataset. Missing-value imputation is not implemented in this function.

3. Integration and Build Review
The CMake configuration was inspected to confirm which source files are included in the library.
The library includes:
- `src/dataset.cpp`
- `src/preprocessor.cpp`
- `src/scaler.cpp`
- `src/svm.cpp`
- `src/loss.cpp`
- `src/regularization.cpp`
The configuration also defines seven automated test programs and an integration example.

Duplicate Regularization Files

The following duplicate files were found:
- `src/regularization.cpp`
- `src/regularization (1).cpp`
- `include/edusvm/regularization.hpp`
- `include/edusvm/regularization (1).hpp`
SHA256 hash checks confirmed that the two source files have identical contents and that the two header files have identical contents.
Git tracks all four files. However, `CMakeLists.txt` includes only `src/regularization.cpp` as the regularization source for the library.
Recommendation:Coordinate with the relevant contributors before removing the duplicate files. Any cleanup should be made in a separate, documented commit.

4. Automated Test Results
The following command was executed:
`ctest --test-dir build --output-on-failure`
The reported results were:

| Test | Result |
|---|---|
| Dataset basic | Passed |
| Dataset edge cases | Passed |
| Preprocessor | Passed |
| Scaler | Passed |
| SVM | Passed |
| Hinge loss | Passed |
| Regularization | Passed |

Overall result: 7/7 tests passed (100%).

These results confirm that the configured tests passed during this run. They do not prove that every possible input or edge case is handled correctly.

5. Findings and Recommendations

The review identified the following points:

1. The inspected SVM decision function and functional margin match their expected formulas.
2. The hinge-loss formula matches the expected mathematical expression, but its numeric-input validation should be improved.
3. The L2 regularization formula and inspected validation checks are consistent with the expected implementation.
4. The scaler implements min-max normalization and handles constant features.
5. The preprocessor validates and copies the dataset but does not perform missing-value imputation in its current implementation.
6. Duplicate regularization files are tracked by Git and should be reviewed before cleanup.
7. The scaler should be fitted on training data only when a separate validation or test dataset is used.
8. Further testing should cover the complete training and prediction workflow, as well as additional invalid-input cases.

6. Conclusion
The inspected implementations are consistent with the reviewed SVM decision function, functional margin, hinge-loss, L2 regularization, min-max scaling, and preprocessing descriptions.
The automated test run reported seven passing tests. The review also identified follow-up work involving hinge-loss input validation, duplicate regularization files, and verification of the full model-training workflow.
The project should continue through integration testing and code review before claiming that the complete library is fully verified.

7. Verification Status
    Formula review:Completed for the inspected formulas.
Automated tests:Seven tests passed in the reported run.

Follow-up work:Numeric-input validation, duplicate-file cleanup review, and end-to-end workflow verification remain recommended.
