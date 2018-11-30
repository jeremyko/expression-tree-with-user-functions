cmake_minimum_required (VERSION 2.8) 

project (expression-tree)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
SET(CMAKE_CXX_STANDARD 11) 
#SET(CMAKE_BUILD_TYPE Debug)
SET(CMAKE_BUILD_TYPE Release)

ADD_DEFINITIONS(-Wall)

ADD_EXECUTABLE (test_cases main.cpp      expression_tree.cpp expression_tree.h expression_tree_common.h testCases.cpp token_parser.cpp token_parser.h user_functions.cpp)
ADD_EXECUTABLE (test_perf  test_perf.cpp expression_tree.cpp expression_tree.h expression_tree_common.h token_parser.cpp token_parser.h user_functions.cpp)

TARGET_LINK_LIBRARIES (test_cases LINK_PUBLIC gtest)
TARGET_LINK_LIBRARIES (test_cases LINK_PUBLIC gtest_main)



