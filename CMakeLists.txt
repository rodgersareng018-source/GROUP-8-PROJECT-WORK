cmake_minimum_required(VERSION 3.16)
project(EduSVM LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
add_library(edusvm
src/dataset.cpp
src/preprocessor.cpp
src/scaler.cpp
src/svm.cpp
src/loss.cpp
src/regularization.cpp
)
target_include_directories(edusvm
PUBLIC ${PROJECT_SOURCE_DIR}/include
)
enable_testing()
function(add_edusvm_test test_name test_file)
add_executable(${test_name} ${test_file})
target_link_libraries(${test_name} PRIVATE edusvm)
add_test(NAME ${test_name} COMMAND ${test_name})
endfunction()
add_edusvm_test(test_dataset_basic tests/test_dataset_basic.cpp)
add_edusvm_test(test_dataset_edges tests/test_dataset_edges.cpp)
add_edusvm_test(test_preprocessor tests/test_preprocessor.cpp)
add_edusvm_test(test_scaler tests/test_scaler.cpp)
add_edusvm_test(test_svm tests/test_svm.cpp)
add_edusvm_test(test_loss tests/test_loss.cpp)
add_edusvm_test(test_regularization tests/test_regularization.cpp)
add_executable(example_calculations
examples/example_calculations.cpp
)
target_link_libraries(example_calculations PRIVATE edusvm)