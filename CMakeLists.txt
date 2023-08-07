cmake_minimum_required(VERSION 3.21)
project(cppli)

set(CMAKE_CXX_STANDARD 20)

include(CTest)


file(GLOB_RECURSE LIBRARY_SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)
add_library(cppli ${LIBRARY_SOURCES})
target_include_directories(cppli PRIVATE ${PROJECT_SOURCE_DIR}/include/cppli/detail/)

add_library(cppli-full-error-check ${LIBRARY_SOURCES})
target_include_directories(cppli-full-error-check PRIVATE ${PROJECT_SOURCE_DIR}/include/cppli/detail/)
target_compile_definitions(cppli-full-error-check PRIVATE CPPLI_FULL_ERROR_CHECKING_BEFORE_RUN)

file(GLOB_RECURSE TEST_SOURCES ${PROJECT_SOURCE_DIR}/test/*.cpp)

add_executable(testexe ${TEST_SOURCES})
target_include_directories(testexe PRIVATE include test lib/Catch2/src/catch2)
target_compile_definitions(testexe PRIVATE CPPLI_FULL_ERROR_CHECKING_BEFORE_RUN)

add_executable(testexe-singleheader ${TEST_SOURCES})
target_include_directories(testexe-singleheader PRIVATE test single_header lib/Catch2/src/catch2)
target_compile_definitions(testexe-singleheader PRIVATE CPPLI_SINGLE_HEADER CPPLI_FULL_ERROR_CHECKING_BEFORE_RUN)

add_subdirectory(lib/Catch2)
target_link_libraries(testexe PRIVATE Catch2 cppli-full-error-check)
target_link_libraries(testexe-singleheader PRIVATE Catch2)

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(testexe PRIVATE /Zc:preprocessor)
    target_compile_options(testexe-singleheader PRIVATE /Zc:preprocessor)
    target_compile_options(cppli PRIVATE /Zc:preprocessor)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(testexe PRIVATE -Werror=return-type)
endif()

add_test(Test testexe)
add_test(Test-singleheader testexe-singleheader)
