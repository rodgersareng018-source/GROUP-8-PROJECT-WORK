# Vivian — Week 2 Formula Review and Progress Report

## Objective

Review the EduSVM implementation to verify that the core mathematical formulas match their intended SVM definitions and document issues requiring follow-up.

## Work Completed

- Reviewed the SVM decision function in `src/svm.cpp`.
- Verified that the decision score follows \(f(x)=w\cdot x+b\).
- Verified that the functional margin follows \(m=yf(x)\), with labels restricted to `-1` and `1`.
- Reviewed `src/loss.cpp` and confirmed the hinge loss formula \(L=\max(0,1-yf(x))\).
- Reviewed `src/regularization.cpp` and confirmed the L2 penalty \(R(w)=\frac{\lambda}{2}\sum_i w_i^2\).
- Reviewed `src/scaler.cpp` and confirmed the min-max scaling formula, including handling of constant features.
- Reviewed `src/preprocessor.cpp` and confirmed that preprocessing currently validates and copies the dataset, while scaling is handled separately.

## Findings and Recommendations

1. **SVM formulas:** The decision score and functional margin are consistent with standard binary SVM definitions.
2. **Hinge loss:** The formula is correct. Additional checks for non-finite input values and results would improve robustness.
3. **Regularization:** The L2 penalty formula is correct. Two identical-looking regularization source files and two identical-looking headers are tracked in Git. The group should review and agree on cleanup before removing duplicates.
4. **Scaling:** Constant-valued features are safely transformed to zero. During model evaluation, fit the scaler on training data and reuse it on test data to avoid data leakage.
5. **Preprocessing:** The current implementation validates and copies the data; it does not implement missing-value imputation. This limitation should be considered when documenting the library's current capabilities.

## In Progress / Pending Verification

- Run the full build and automated tests on the updated branch.
- Review the duplicate regularization files with the group before making changes.
- Confirm that the README and project documentation accurately describe implemented functionality.

## Challenges / Blockers

- Duplicate regularization files may cause confusion during future maintenance.
- The hinge-loss function has less input validation than the decision-function and regularization implementations.
- Missing-value imputation is not currently implemented in the inspected preprocessing function.

## Conclusion

The reviewed decision-function, functional-margin, hinge-loss, L2-regularization, and min-max-scaling formulas match their intended mathematical definitions. The report also identifies validation and maintenance improvements. Full build and test results remain pending verification for this report.
