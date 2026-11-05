
# Week 4 Report 

## Module
Preprocessing (`preprocessor.hpp`, `preprocessor.cpp`)

## Task
Module verification: review preprocessing code, check dataset validation and copying, identify missing input-validation cases, add or improve preprocessing tests, and submit a short verification report.

---

## 1. Files Reviewed

| File | Status |
|------|--------|
| `include/edusvm/preprocessor.hpp` | Reviewed |
| `src/preprocessor.cpp` | Reviewed |
| `tests/test_preprocessor.cpp` | Reviewed and updated |

---

## 2. Preprocessing Implementation Review

### What `preprocessDataset()` does

The function:

1. Calls `validateDataset(input, errorMessage)` to reject invalid data.
2. Copies the input dataset to the output dataset (`output = input`).
3. Clears the error message on success and returns `true`.

### What it does NOT do yet

- Missing-value handling (no imputation or rejection policy beyond validation).
- Outlier removal.
- Feature scaling (handled separately by the scaler module, as designed).
- CSV loading (not part of this module).

These are future improvements and are **not** claimed as completed.

### Dataset validation and copying — verification

- Validation is delegated to `validateDataset()`, which is the correct design so the rules live in one place.
- Copying is a straightforward value copy of `features` and `labels`, preserving row/label alignment.
- On failure, the output dataset is left untouched and an explanatory message is returned.

**Result: PASS** — implementation matches the declared interface and the agreed Week 2/3 design.

---

## 3. Input-Validation Cases Checked

| Case | Expected | Result |
|------|----------|--------|
| Valid dataset | Accepted, copied correctly | PASS |
| Empty dataset | Rejected | PASS |
| Row count ≠ label count | Rejected | PASS |
| Student row with no features | Rejected | PASS |
| Rows with unequal feature counts | Rejected | PASS |
| NaN / infinity in features | Rejected | PASS |
| Label other than 1 or -1 | Rejected | PASS |
| Output untouched on failure | No partial copy | PASS |

---

## 4. Tests Added / Improved

File: `tests/test_preprocessor.cpp`

Minimum checks now covered:

1. **Valid data is copied correctly** — features and labels compared element-by-element after preprocessing.
2. **Invalid data is rejected** — empty dataset and mismatched label count return `false` with a non-empty error message.
3. **Error message handling** — error is cleared on success and populated on failure.

## Example test logic:
***
`cpp
{std::string error;

// Valid dataset copied correctly
edusvm::Dataset input;
input.features = {{2.0, 80.0}, {5.0, 95.0}};
input.labels = {-1, 1};

edusvm::Dataset output;
assert(edusvm::preprocessDataset(input, output, error));
assert(error.empty());
assert(output.features == input.features);
assert(output.labels == input.labels);

// Invalid dataset rejected
edusvm::Dataset empty;
edusvm::Dataset emptyOut;
assert(!edusvm::preprocessDataset(empty, emptyOut, error));
assert(!error.empty());
  }***

## Completed Validation Framework

Built 4 high-coverage automated testing blocks targeting core pipeline integrity:
1. `testValidDataset`: Assures clean datasets transfer completely.
2. `testStructuralRowMismatch`: Catches dimensional alignment errors.
3. `testNonFiniteCorruptedInputs`: Blocks mathematically destabilizing infinity or NaN metrics.
4. `testInvalidTargetLabelRanges`: Drops data components using unauthorized non-binary classification labels.

## 5. Test and Build

Use the build and run icons below the window page of vs code.
Or use commands below in bash
cmake-S . -B build
cmake --build build
ctest --test-dir build --output-on-failure

Note: only tests that run and build are reported as passing, else listed as blocker

### 6. Issue and  Action
No NaN/infinity policy beyond validation: Documented as a future improvement; not claimed complete
No missing-value handling: Documented as future work
Scaling left to scaler module: Confirmed correct separation of concernsand 

## 7. Deliverables
Reviewed: Preprocessor.hpp and preprocessor.cpp
Updated/added tests in tests/test_preprocessor.cpp
This report: reports/week-04-charity.md

### AI use
Tool: Chatgpt
Purpose: Explanation of concept, review guidance.
Reason: To help organise verification steps 
Human review: All code, test and results and findings were checked and run before submission.

