cmake_minimum_required(VERSION 3.16)

project(EduSVM LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(edusvm_lib
    src/dataset.cpp
    src/preprocessor.cpp
    src/preprocessor_strategies.cpp
    src/scaler.cpp
    src/svm.cpp
    src/evaluation.cpp
    src/loss.cpp
    src/regularization.cpp
)

target_include_directories(edusvm_lib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)


# Complete EduSVM classification example
add_executable(end_to_end examples/end_to_end.cpp)
target_link_libraries(end_to_end PRIVATE edusvm_lib)
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")
    add_executable(main main.cpp)
    target_link_libraries(main PRIVATE edusvm_lib)
endif()

enable_testing()

add_executable(test_dataset_basic tests/test_dataset_basic.cpp)
target_link_libraries(test_dataset_basic PRIVATE edusvm_lib)
add_test(NAME test_dataset_basic COMMAND test_dataset_basic)

add_executable(test_dataset_edges tests/test_dataset_edges.cpp)
target_link_libraries(test_dataset_edges PRIVATE edusvm_lib)
add_test(NAME test_dataset_edges COMMAND test_dataset_edges)

# CSV dataset loading tests
add_executable(test_dataset_csv tests/test_dataset_csv.cpp)
target_link_libraries(test_dataset_csv PRIVATE edusvm_lib)
add_test(NAME test_dataset_csv COMMAND test_dataset_csv)

set_tests_properties(test_dataset_csv PROPERTIES
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
)

add_executable(test_preprocessor tests/test_preprocessor.cpp)
target_link_libraries(test_preprocessor PRIVATE edusvm_lib)
add_test(NAME test_preprocessor COMMAND test_preprocessor)

add_executable(test_scaler tests/test_scaler.cpp)
target_link_libraries(test_scaler PRIVATE edusvm_lib)
add_test(NAME test_scaler COMMAND test_scaler)

add_executable(test_svm tests/test_svm.cpp)
target_link_libraries(test_svm PRIVATE edusvm_lib)
add_test(NAME test_svm COMMAND test_svm)

add_executable(test_loss tests/test_loss.cpp)
target_link_libraries(test_loss PRIVATE edusvm_lib)
add_test(NAME test_loss COMMAND test_loss)

add_executable(test_regularization tests/test_regularization.cpp)
target_link_libraries(test_regularization PRIVATE edusvm_lib)
add_test(NAME test_regularization COMMAND test_regularization)


add_executable(test_evaluation tests/test_evaluation.cpp)
target_link_libraries(test_evaluation PRIVATE edusvm_lib)
add_test(NAME test_evaluation COMMAND test_evaluation)
add_executable(test_oop tests/test_oop.cpp)
target_link_libraries(test_oop PRIVATE edusvm_lib)
add_test(NAME test_oop COMMAND test_oop)


