cmake_minimum_required(VERSION 3.10)
project(EduSVM_Project LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(edusvm_lib
    src/preprocessor.cpp
    src/dataset.cpp
)

target_include_directories(edusvm_lib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")
    add_executable(main main.cpp)
    target_link_libraries(main PRIVATE edusvm_lib)
endif()

add_executable(test_preprocessor tests/test_preprocessor.cpp)
target_link_libraries(test_preprocessor PRIVATE edusvm_lib)

enable_testing()
add_test(NAME test_preprocessor COMMAND test_preprocessor)