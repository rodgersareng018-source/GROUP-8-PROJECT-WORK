cmake_minimum_required(VERSION 3.16)

project(EduSVM LANGUAGES CXX)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# Use the same language version for the library, demo, and tests.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(edusvm_lib
    src/dataset.cpp
    src/loss.cpp
    src/preprocessor.cpp
    src/preprocessor_strategies.cpp
    src/regularization.cpp
    src/scaler.cpp
    src/svm.cpp
)

# Keep header paths correct in both the build tree and installed package.
target_include_directories(edusvm_lib
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

add_executable(edusvm main.cpp)
target_link_libraries(edusvm PRIVATE edusvm_lib)

# Build every standalone test with the same library used by the demo.
set(EDUSVM_TESTS
    test_dataset_basic
    test_dataset_edges
    test_dataset_csv
    test_loss
    test_oop
    test_preprocessor
    test_regularization
    test_scaler
    test_svm
)

enable_testing()
foreach(test_name IN LISTS EDUSVM_TESTS)
   add_executable(${test_name} tests/${test_name}.cpp)
   target_link_libraries(${test_name} PRIVATE edusvm_lib)
   add_test(NAME ${test_name} COMMAND ${test_name})
endforeach()

# Let the CSV-loader test find the repository's sample data from any build dir.
target_compile_definitions(test_dataset_csv PRIVATE
    EDUSVM_TEST_CSV_PATH="${CMAKE_CURRENT_SOURCE_DIR}/data/input/students.csv"
)

add_test(
    NAME edusvm_demo
    COMMAND edusvm "${CMAKE_CURRENT_SOURCE_DIR}/data/input/students.csv"
)

# Install the library, command-line demo, public headers, and sample input.
install(TARGETS edusvm_lib edusvm
    EXPORT EduSVMTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(FILES data/input/students.csv
    DESTINATION ${CMAKE_INSTALL_DATADIR}/edusvm
)

configure_package_config_file(
    cmake/EduSVMConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/EduSVMConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/EduSVM
)

# Provide version-aware find_package(EduSVM) support to installed consumers.
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/EduSVMConfigVersion.cmake
    VERSION 1.0.0
    COMPATIBILITY SameMajorVersion
)

install(EXPORT EduSVMTargets
    FILE EduSVMTargets.cmake
    NAMESPACE EduSVM::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/EduSVM
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/EduSVMConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/EduSVMConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/EduSVM
)
