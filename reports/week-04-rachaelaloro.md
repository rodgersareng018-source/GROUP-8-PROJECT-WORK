Week 4 Report – rachaelaloro: Feature Scaling

1. Introduction

This week's work focused on reviewing and verifying the feature scaling module in the EduSVM project. The scaler uses Min-Max normalization to transform feature values based on the minimum and maximum values learned from a dataset.

The main objective was to verify that fitting and transforming work correctly, test constant features and invalid datasets, and document the correct use of the scaler with training and test data.

2. Review of the Scaler Module

The following files were reviewed:

- "include/edusvm/scaler.hpp"
- "src/scaler.cpp"

The scaler provides three main operations:

- "fit()": Learns the minimum and maximum values for each feature.
- "transform()": Applies the learned scaling parameters to a dataset.
- "fitTransform()": Fits the scaler and transforms the same dataset.

The implementation uses the Min-Max normalization formula:

[
x'=\frac{x-x_{\min}}{x_{\max}-x_{\min}}
]

For a constant feature, where the maximum and minimum values are equal, the implementation assigns all transformed values to 0.0.

The implementation also validates datasets before processing them. It checks for empty datasets, mismatched labels, inconsistent feature-row sizes, incorrect feature counts, and non-finite feature values.

3. Testing and Verification

The scaler tests were reviewed and extended to verify the following cases:

1. Ordinary values: Confirmed that Min-Max normalization produces the expected values.
2. Constant features: Confirmed that constant features are transformed to zero.
3. Reuse of fitted parameters: Confirmed that new data is transformed using the parameters learned during fitting.
4. Values outside the training range: Added a check to confirm that a test value of 15.0, using training minimum 0.0 and maximum 10.0, transforms to 1.5.
5. Empty datasets: Confirmed that an empty dataset is rejected.
6. Inconsistent rows: Confirmed that rows with different feature counts are rejected.
7. Non-finite values: Confirmed that NaN values are rejected.
8. Mismatched labels: Confirmed that datasets with different numbers of feature rows and labels are rejected.
9. Wrong feature count: Confirmed that transforming data with a different number of features is rejected.
10. Transform before fitting: Confirmed that transforming before fitting raises a logic error.

4. Correct Use with Training and Test Data

To prevent data leakage, the scaler should be fitted using the training dataset only.

The recommended procedure is:

1. Split the original dataset into training and test datasets.
2. Fit the scaler using the training dataset.
3. Transform the training dataset using the fitted scaler.
4. Transform the test dataset using the same fitted scaler.
5. Do not fit the scaler again using the test dataset.

Example:

Scaler scaler;

scaler.fit(trainData);

Dataset scaledTrain = scaler.transform(trainData);
Dataset scaledTest = scaler.transform(testData);

This ensures that both datasets use the same minimum and maximum values learned from the training data.

5. Test Results

The scaler test was executed independently and passed.

The complete project test suite was also executed using:

ctest --test-dir build --output-on-failure

Result: 7 out of 7 tests passed.

No test failures were reported.

6. Conclusion

The feature scaling module was reviewed and its main operations were verified. The existing scaler tests were extended to check how the scaler handles values outside the training range.

The scaler correctly performs Min-Max normalization, handles constant features, validates invalid datasets, and reuses fitted parameters when transforming new data.

The complete project test suite passed successfully, indicating that the changes did not introduce failures in the registered project tests.