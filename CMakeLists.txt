cmake_minimum_required(VERSION 3.16)

project(EduSVM LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Build the EduSVM library using the dataset module.
add_library(edusvm
    src/dataset.cpp
)

# Make public header files available to the library and 2its users.
target_include_directories(edusvm
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)

# Enable automated testing.
enable_testing()

# Basic dataset tests.
add_executable(test_dataset_basic
    tests/test_dataset_basic.cpp
)

target_link_libraries(test_dataset_basic PRIVATE edusvm)
add_test(NAME DatasetBasic COMMAND test_dataset_basic)

# Dataset edge-case tests.
add_executable(test_dataset_edges
    tests/test_dataset_edges.cpp
)

target_link_libraries(test_dataset_edges PRIVATE edusvm)
add_test(NAME DatasetEdges COMMAND test_dataset_edges)

