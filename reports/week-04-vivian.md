Role
Quality assurance: review the project against the instructor's requirements, check test coverage, review the README and example, and prepare the final requirements checklist.

Work done
- Reviewed the repository on branch main after pulling the latest changes on November 4, 2026.
- Checked every folder and root file against the instructor's required structure.
- Compared each module with its test file.
- Reviewed README.md and examples/example_calculations.cpp.
- Build and test results were taken from Levy and Monica, not run by me, because I work in vscode.dev, which cannot compile C++.
- Checked the mathematical formulas by hand: min-max scaling, decision function, functional margin, hinge loss and L2 penalty.

Modules vs tests
| Module | Header | Source | Test file | Test result |
|---|---|---|---|---|
| Dataset | dataset.hpp | dataset.cpp | test_dataset_basic.cpp, test_dataset_edges.cpp | pass |
| Preprocessor | preprocessor.hpp | preprocessor.cpp | test_preprocessor.cpp | pass |
| Scaler | scaler.hpp | scaler.cpp | test_scaler.cpp | fail |
| SVM decision/margin | svm.hpp | svm.cpp | test_svm.cpp | pass|
| Hinge loss | loss.hpp | loss.cpp | test_loss.cpp | pass |
| Regularization | regularization.hpp | regularization.cpp | test_regularization.cpp | pass|

Total tests registered: 7 in the CMake configuration (dataset basic, dataset edges, preprocessor, scaler, svm, loss, regularization). Total passed: 5, according to Monica's ctest output.

Project structure review
  Required files present: CMakeLists.txt, include/edusvm/, src/, tests/, examples/, reports/, data/input/ with the sample students.csv, and the six modules above.
   Missing or misplaced: a duplicate regularization file was reported and is being cleaned up by Levy and Rodgers. I did not delete anything myself. 
 .gitignore must be present before submission, and the root README.md must be written by the group.

README review
  Present and adequate: the README exists in the repository root.
  Missing or incomplete: the instructor requires description, objectives, features, structure, requirements, build instructions, usage, examples, how to run tests, limitations, member contributions, and how the team collaborated. Any section not yet written must be completed before submission.
 README is written by the group.

 Example review
  examples/example_calculations.cpp builds: yes, per Monica.
  It demonstrates: preprocessing, scaling, decision score, hinge loss, L2 penalty.
  It does not demonstrate: model training or reliable PASS/FAIL prediction. The weights are chosen by hand and the code says it is not a trained model.
  Comments or output that overclaim: none found. The example is honest that its weights are manual.

Incomplete requirements
1.Training (optimizer) and prediction: no training module is in the work, so the library cannot yet learn its own weights. Owner: whole group.
2.Evaluation metrics (for example accuracy): not implemented. Owner: whole group.
3.README: must be completed by hand by the group. Owner: all members.
4.Final report and the complete AI-use declaration: required for submission. Owner: Rodgers and all members.
5.Duplicate regularization file: to be resolved. Owner: Levy and Rodgers.
6.Geometric margin, missing-value handling and outlier removal: not implemented, and they are stated as future improvements, not claimed features.

 Recommendations
1.Run the full clean build and test on a fresh environment before submitting, as required.
2.Remove the duplicate regularization file only after Rodgers confirms which copy the build uses.
3.Complete the README by hand, with an honest limitations section stating that training is not implemented.
4.Make sure every member has at least one visible commit and a merged pull request.
5.Keep the AI-use declarations in all weekly reports consistent with the final declaration.
6.State clearly in the presentation which features are implemented and tested, and which are future work.

Challenges
  I work in vscode.dev, which cannot compile C++, so I relied on Monica's and Rodgers's build and test output.
 Early in the project our files did not appear on GitHub because they had not been pushed or merged correctly, and we learned to use branches and pull requests.
  Keeping reports truthful, so that I did not mark features complete just because a file exists.

 AI Use
 Tool: Claude. The group's weekly task guide was also written with an AI assistant.
   Purpose: explained the review steps, the Git and pull request workflow, and the formulas, and drafted this report template and the presentation notes.
 Reason: to understand the task as a beginner and prepare accurate documentation.
   Human review: I checked every item against the actual repository and did not copy results I had not verified.