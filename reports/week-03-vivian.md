# Vivian - Week 3 Progress Report

## 1. Completed

During Week 3, Member 8 - Vivian worked on reviewing the EduSVM project requirements, mathematical calculations, and test coverage.

The following tasks were completed:

- Reviewed the project modules against the EduSVM requirements.
- Independently calculated expected results for min-max feature scaling.
- Verified expected results for the SVM decision function and functional margin.
- Calculated expected hinge-loss values for different labels and decision scores.
- Verified expected L2 regularization penalty calculations.
- Reviewed Rodgers's example calculations and checked the expected scaled feature values.
- Reviewed the sample student dataset and its expected structure.
- Examined test coverage for dataset validation, preprocessing, scaling, SVM, hinge loss, and regularization.
- Identified missing edge-case tests and implementation gaps.
- Prepared the Week 3 Test and Requirements Review document.

## 2. In Progress

- Updating the requirements checklist using the complete CTest results.
- Following up on missing edge-case tests, including infinity handling in dataset validation.
- Reviewing additional tests as members submit their contributions.
- Monitoring outstanding requirements such as model training, gradient optimization, support-vector identification, and evaluation metrics.

## 3. Challenges/Blockers

- Some modules have limited test coverage for invalid and edge-case inputs.
- Existing tests use assert, which may be disabled in Release builds.
- The current preprocessing implementation validates and copies data but does not handle missing values or outliers.
- The SVM implementation reviewed provides a functional margin, not a geometric margin.
- CSV loading for the sample student dataset was not yet implemented at the time of the review.
- Hinge loss and regularization were implemented as separate calculations rather than a complete training objective.
- Additional implementation and testing are required for the remaining SVM functionality.

## 4. Next Week

- Follow up on the identified test gaps.
- Review new module contributions and their test results.
- Update the requirements checklist as new functionality is implemented.
- Verify outstanding project requirements with the group.
- Continue checking that the documentation accurately reflects the implemented features.

## 5. AI Use

- Tool: ChatGPT.
- Purpose: Assistance with understanding project structure, Git/GitHub workflows, and preparing starter code and tests.
- Reason: To support learning and help with project requirements and testing procedures.
- Human contribution: Expected mathematical results were independently calculated, while test coverage, implementation gaps, and risks were reviewed and documented.
- AI-assisted work and suggestions should be reviewed and verified by the group before acceptance.
