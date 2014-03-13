
#include<iostream>
#include "gtest/gtest.h"

using namespace std;

///////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[])
{           
    //::testing::GTEST_FLAG(filter) = "ExpressionTest.Functions";
    //::testing::GTEST_FLAG(filter) = "ExpressionTest.AND_OR";
    //::testing::GTEST_FLAG(filter) = "ExpressionTest.negative_positive:ExpressionTest.AND_OR";
    //::testing::GTEST_FLAG(filter) = "ExpressionTest.TODO_NotImplemented";
    //::testing::GTEST_FLAG(filter) = "ExpressionTest.CHK";
    //::testing::GTEST_FLAG(filter) = "ExpressionTest.Literal";
    //::testing::GTEST_FLAG(filter) = "ExpressionTest.Numeric";
        
    ::testing::GTEST_FLAG(filter) = "ExpressionTest.*";
    
    testing::InitGoogleTest(&argc, argv);   
    return RUN_ALL_TESTS(); 
}

