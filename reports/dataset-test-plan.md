# Dataset test plan

# Purpose
To verify that EduSVM accepts valid students data and rejects invalid data

# Test cases
1. A valid dataset with two students and two features per student should pass.
2. An empty dataset should fail.
3. A datset with a different number of feature rows and labels should fail.
4. A student row with no features should fail.
5. Students rows with different numbers of features should fail.
6. A dataset containing NaN or infinity should fail.
7. A label other than 1 or -1 should fail.

# Expected Results
The validation function should return true for valid data and false for invalid data, with an explanatory error message.

