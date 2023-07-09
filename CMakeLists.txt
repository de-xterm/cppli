cmake_minimum_required(VERSION 3.21)
project(cppli)

set(CMAKE_CXX_STANDARD 20)

include(CTest)

MACRO(HEADER_DIRECTORIES return_list pattern) #recursively adds all directories containing header files to return_list
    FILE(GLOB_RECURSE new_list ${pattern}*.h ${pattern}*.hpp ${pattern}*.tpp)
    SET(dir_list "")
    FOREACH(file_path ${new_list})
        GET_FILENAME_COMPONENT(dir_path ${file_path} PATH)
        SET(dir_list ${dir_list} ${dir_path})
    ENDFOREACH()
    LIST(REMOVE_DUPLICATES dir_list)
    SET(${return_list} ${dir_list})
ENDMACRO()

HEADER_DIRECTORIES(PROJECT_HEADERS src/)

file(GLOB_RECURSE COMMON_SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)
add_library(cppli ${COMMON_SOURCES})
target_include_directories(cppli PRIVATE ${PROJECT_HEADERS})


file(GLOB_RECURSE TEST_SOURCES ${PROJECT_SOURCE_DIR}/test/*.cpp)
add_executable(testexe ${TEST_SOURCES} ${COMMON_SOURCES})
target_include_directories(testexe PRIVATE ${PROJECT_HEADERS} test lib/Catch2/src/catch2)


file(GLOB_RECURSE TEST_SOURCES ${PROJECT_SOURCE_DIR}/test/*.cpp)
add_executable(testexe-singleheader ${TEST_SOURCES} )
target_include_directories(testexe-singleheader PRIVATE test single_header lib/Catch2/src/catch2)
target_compile_definitions(testexe-singleheader PRIVATE CPPLI_SINGLE_HEADER)

add_subdirectory(lib/Catch2)
target_link_libraries(testexe PRIVATE Catch2)
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
