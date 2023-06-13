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


file(GLOB_RECURSE TEST_SOURCES ${PROJECT_SOURCE_DIR}/test/*.cpp) #globbing expressions intentionally excludes main, because these sources will be linked to test too
add_executable(Test_Executable ${TEST_SOURCES} ${COMMON_SOURCES})
target_include_directories(Test_Executable PRIVATE ${PROJECT_HEADERS} lib/Catch2/src/catch2)
target_compile_options(cppli PRIVATE /Zc:preprocessor)


add_subdirectory(lib/Catch2)
target_link_libraries(Test_Executable PRIVATE Catch2)

target_compile_options(Test_Executable PRIVATE /Zc:preprocessor)

add_test(Test Test_Executable)