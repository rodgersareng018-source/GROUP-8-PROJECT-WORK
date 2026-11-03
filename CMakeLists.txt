cmake_minimum_required(VERSION 3.16)

project(EduSVM LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB EDUSVM_SOURCES CONFIGURE_DEPENDS
    "${PROJECT_SOURCE_DIR}/src/*.cpp"
)

if(NOT EDUSVM_SOURCES)
    message(FATAL_ERROR "No C++ source files found in src/")
endif()

add_library(edusvm ${EDUSVM_SOURCES})

target_include_directories(edusvm
    PUBLIC "${PROJECT_SOURCE_DIR}/include"
)

enable_testing()

file(GLOB EDUSVM_TEST_FILES CONFIGURE_DEPENDS
    "${PROJECT_SOURCE_DIR}/tests/test_*.cpp"
)

foreach(TEST_FILE IN LISTS EDUSVM_TEST_FILES)
    get_filename_component(TEST_NAME "${TEST_FILE}" NAME_WE)

    add_executable("${TEST_NAME}" "${TEST_FILE}")
    target_link_libraries("${TEST_NAME}" PRIVATE edusvm)

    add_test(
        NAME "${TEST_NAME}"
        COMMAND "${TEST_NAME}"
    )
endforeach()

file(GLOB EDUSVM_EXAMPLES CONFIGURE_DEPENDS
    "${PROJECT_SOURCE_DIR}/examples/*.cpp"
)

foreach(EXAMPLE_FILE IN LISTS EDUSVM_EXAMPLES)
    get_filename_component(EXAMPLE_NAME "${EXAMPLE_FILE}" NAME_WE)

    add_executable("${EXAMPLE_NAME}" "${EXAMPLE_FILE}")
    target_link_libraries("${EXAMPLE_NAME}" PRIVATE edusvm)
endforeach()