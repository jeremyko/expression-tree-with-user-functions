#c++ expression tree library

http://jeremyko.blogspot.kr/2014/03/c-expression-tree.html

#Usage 
 
    ExpressionTree expTree;
    expression_node* pExpressionRslt;
    bool bRslt = false;

    cout << "\n---------------------------------------\n";
    bRslt = expTree.SetInfixExpression("1 * -2 ");
    EXPECT_TRUE(bRslt);
    if (bRslt)
    {
       bRslt = expTree.EvaluateExpression();
       EXPECT_TRUE(bRslt);
       pExpressionRslt = expTree.GetResult();
       cout << "expTree.EvaluateExpression returns :" 
            << pExpressionRslt->variable.nLongValue << "\n";
       EXPECT_EQ(-2, pExpressionRslt->variable.nLongValue);
    }

    cout << "\n---------------------------------------\n";
    bRslt = expTree.SetInfixExpression("SumInt4(SumInt4(1,1+1,1+1,2), 1+1, 1+1, 1+1)");
    EXPECT_TRUE(bRslt);
    bRslt = expTree.EvaluateExpression();
    EXPECT_TRUE(bRslt);
    pExpressionRslt = expTree.GetResult();
    cout << "expTree.EvaluateExpression returns :" 
         << pExpressionRslt->variable.nLongValue << "\n";
    EXPECT_EQ(13, pExpressionRslt->variable.nLongValue);
   
    cout << "\n---------------------------------------\n";
    bRslt = expTree.SetInfixExpression("GetBool('1'='1')");
    EXPECT_TRUE(bRslt);
    bRslt = expTree.EvaluateExpression();
    EXPECT_TRUE(bRslt);
   
    cout << "\n---------------------------------------\n";
    bRslt = expTree.SetInfixExpression("StrCat('ab','cd')=StrCat('a','b')+StrCat('c','d') ");
    EXPECT_TRUE(bRslt);
    bRslt = expTree.EvaluateExpression();
    EXPECT_TRUE(bRslt);
   
    cout << "\n---------------------------------------\n";
    bRslt = expTree.SetInfixExpression("StrCat3('1','2',StrCat3('a','b',StrCat3('X','Y','Z')) )");
    EXPECT_TRUE(bRslt);
    bRslt = expTree.EvaluateExpression();
    EXPECT_TRUE(bRslt);
    EXPECT_EQ(4, expTree.GetDepth());
    pExpressionRslt = expTree.GetResult();
    cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->strVal << "\n";
    EXPECT_STREQ("12abXYZ", pExpressionRslt->strVal);
 
#LICENSE

This projected is licensed under the terms of the BSD license.
