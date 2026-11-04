EduSVM Requirements Checklist

Status key: Done / Partial / Missing. Reviewed by Vivian.
A feature is marked Done only if the code exists and its test passes.
 Test results are from Monica's/Rodgers's ctest output, not run by me.

Code and library
| Requirement | Status | Note |
| Reusable C++ library with a public API | Done | Built as a CMake library edusvm with public headers. |
| Headers in include/edusvm/, implementation in src/ | Done | Interface and implementation are separated. |
| Dataset validation | Done | Rejects empty data, mismatched labels, unequal rows, NaN/infinity and bad labels.|
| Preprocessing | Partial | Validates and copies only. No missing-value or outlier handling. |
| Feature scaling | Done | Min-max scaling, constant features map to 0.|
| Hyperplane and margin calculations | Done | Decision function and functional margin. Geometric margin not implemented. |
| Hinge loss | Done |improved non-finite validation was merged |
| L2 regularization | Done | Calculated separately, not yet combined with hinge loss. |
| Training (optimizer) | Missing | mark Done only if an optimizer and its test are on main. |
| Prediction | Partial | Decision score works with manual weights. No trained PASS/FAIL prediction. |
| Evaluation metrics | Missing | No accuracy or similar metrics implemented. |
| Invalid inputs handled | Done | Functions return false with an error message. |
| No unnecessary duplicate code or files | Partial | Duplicate regularization file reported. Levy and Rodgers are resolving it.|

Testing
| Requirement | Status | Note |
| Test for every module | Done | Seven registered tests.  test_regularization.cpp is on main |
| Normal cases, edge cases and invalid input tested | Done | Covered in each test file. |
| All tests pass from a clean build | Partial |change to Done only if all 7 of 7 passed. |

Build and examples
| Requirement | Status | Note |
| CMakeLists.txt builds library, tests and example | Done |
| Builds from a clean environment | Partial | Must be tested on a fresh machine before submission. |
| Example program shows library usage | Partial | Shows preprocessing, scaling, score, loss and penalty. Does not show training. |
| Sample data in data/input/ | Done | students.csv with study hours, attendance, assignment score and label. CSV loading is not implemented. |

Documentation
| Requirement | Status | Note |
| README has all required sections | Partial |
| README is hand-typed by the group | Partial | Group must confirm it was not AI-generated. |
| Instructions to build, use and run tests | Partial | Commands: cmake -S . -B build, cmake --build build, ctest -test-dir build. Must be in the README. |
| Limitations stated honestly | Partial | Must state: no training, no evaluation metrics, basic preprocessing. |
| Member contributions and teamwork described | Partial | Must be written in the README and final report. |

Process and reports
| Requirement | Status | Note|
| Weekly reports week-01 to week-04 | Partial | Week 4 reports are per member. |
| final-report.md | Missing | Required for submission. Owner: Rodgers and all members. |
| AI-use declaration (weekly and final) | Partial | Weekly sections must match the final declaration. |
| Every member has visible commits | Partial | yes|
| Pull requests used and reviewed | Partial | all members have pull requests|

Submission package
| Requirement | Status | Note |
| LICENSE and .gitignore | Partial | both exist in the root. |
| GitHub repository final and merged into main | Partial | Final integration due November 5. |

Outstanding items
1.Complete the hand-typed README with all required sections and honest limitations. Owner: all members, coordinated by Rodgers.
2.Write final-report.md and the complete AI-use declaration. Owner:  all members.
3.Confirm all tests pass from a clean build on a fresh environment. 
4.Resolve the duplicate regularization file. Owner: Levy and Rodgers.
5.Merge all eight members' pull requests into main so everyone has visible commits. Owner: Rodgers.
6.Decide whether training and evaluation metrics will be completed. If not, state them as limitations in the README and presentation. Owner: whole group.
7.Confirm .gitignore exist. Owner: Rodgers.
8.Make sure weekly AI-use sections match the final declaration. Owner: all member.