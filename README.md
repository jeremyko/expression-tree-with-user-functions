# c++ expression tree library

## usage 
 
    #include "gtest/gtest.h"
    #include "expression_tree.h"
    #include "user_functions.h"
    
    ExpressionTree expTree;
    bool bRslt = false;
    expression_node* pExpressionRslt;

    cout << "\n---------------------------------------\n";
    bRslt = expTree.SetInfixExpression("StrCat3('1','2',StrCat3('a','b',StrCat3('X','Y','Z')) )");
    EXPECT_TRUE(bRslt);
    bRslt = expTree.EvaluateExpression();
    EXPECT_TRUE(bRslt);
    EXPECT_EQ(4, expTree.GetDepth());
    pExpressionRslt = expTree.GetResult();
    cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->strVal << "\n";
    EXPECT_STREQ("12abXYZ", pExpressionRslt->strVal);
 
