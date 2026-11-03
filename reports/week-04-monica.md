WEEK 4 REPORT

Project: EduSVM — Educational Support Vector Machine
Group: Group 8
Member: Monica Namagembe
Member Number: 6
Role: Build and Integration
Branch: "monica-branch"

1. Introduction

Week 4 marked the final integration phase of the EduSVM project. As Member 6, my responsibility was to review the root CMake build configuration, verify the inclusion of required source files, ensure that the example and tests could build, resolve build configuration issues, and perform build verification.

The main objective was to ensure that the project could be configured, compiled, and tested successfully before final submission.

2. Work Done

During Week 4, I carried out the following activities:

- Reviewed the root "CMakeLists.txt" file.
- Checked the inclusion of the required source files in the build configuration.
- Reviewed the test executables and example program.
- Investigated and resolved the initial CMake build-tool configuration problem.
- Configured the project using the MinGW Makefiles generator.
- Successfully configured and built the project using CMake.
- Used CTest to identify and execute the registered tests.
- Verified the results of the project's dataset, preprocessing, scaling, and SVM tests.

3. Build Verification

The project was configured using:

cmake -S . -B build-mingw -G "MinGW Makefiles"

The project was compiled using:

cmake --build build-mingw

The configuration and compilation completed successfully.

This confirmed that the CMake build configuration was functioning with the available MinGW build environment.

4. Testing and Verification

The project tests were executed using CTest.

The following tests were successfully completed:

1. "test_dataset_basic"
2. "test_dataset_edges"
3. "test_preprocessor"
4. "test_scaler"
5. "test_svm"

Testing result:

- Total tests: 5
- Passed: 5
- Failed: 0
- Success rate: 100%

The successful test execution demonstrated that the tested dataset, preprocessing, scaling, and SVM components passed their registered tests.

5. Challenges Encountered

One of the major challenges was the initial CMake configuration attempting to use NMake, which was not recognized by the terminal.

The issue resulted in compiler and build-tool configuration errors.

To address this problem, I investigated the available compiler and build tools and configured CMake to use the MinGW Makefiles generator.

This allowed the project to configure and compile successfully.

6. How Week 4 Improved on Week 3

Week 3 focused primarily on reviewing the CMake configuration and investigating build errors.

Week 4 progressed from troubleshooting to actual build verification and testing.

The build configuration was successfully tested using MinGW, and the registered project tests were executed.

This provided evidence that the tested project targets could be compiled and that the registered tests passed.

7. Use of AI

AI assistance was used to:

- Interpret CMake configuration errors.
- Understand the difference between NMake and MinGW build tools.
- Configure CMake using the appropriate generator.
- Guide build verification and testing.
- Explain CTest output and test results.
- Assist in documenting the build and integration process.

The assistance improved my understanding of CMake, compiler configuration, and project integration.

8. Conclusion

Week 4 completed the build verification activities for my assigned role in the EduSVM project.

The CMake configuration was successfully used to configure and compile the project, and all five registered tests passed.

The work contributed to the reliability of the project's build configuration and supported the final integration and submission process.

Member: Monica Namagembe
Role: Member 6 — Build and Integration