WEEK 3 WEEKLY PROGRESS REPORT

Project: EduSVM – Support Vector Machine Implementation
Member: Member 2 – Feature Scaling
Week: 3

1. Introduction

During Week 3, I worked on reviewing and testing the feature-scaling module of the EduSVM project. The main objective was to ensure that input features are properly scaled before being used in the machine-learning process.

2. Activities Carried Out

The following activities were completed:

1. Reviewed the existing feature-scaling header and implementation files.

2. Reviewed the Min-Max scaling formula:
   
   [
   x' = \frac{x-x_{\min}}{x_{\max}-x_{\min}}
   ]

3. Reviewed the handling of constant features, which are mapped to zero.

4. Added a test for mismatched feature rows and labels.

5. Added a test for incorrect feature counts during transformation.

6. Reviewed tests for empty datasets, inconsistent row sizes and invalid numerical values.

7. Tested transformation before fitting and reuse of fitted scaling parameters on separate data.

3. Testing and Verification

The project was configured and built using CMake.

The registered test suite was executed using CTest.

Test result: All 7 registered tests passed, with zero failures.

4. GitHub Contribution

The changes were committed and pushed to my working branch:

"rachaelaloro-branch"

The changes were made available for review through the group's GitHub workflow.

5. Challenges Encountered

An older test file using a previous scaler interface caused compatibility issues during compilation. It was renamed to prevent it from being automatically included in the current test suite.

6. Conclusion

Week 3 focused on reviewing the feature-scaling module, improving its validation tests and verifying successful project compilation. The changes were committed and pushed to the working branch for review.