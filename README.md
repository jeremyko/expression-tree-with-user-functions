#c++ expression tree library

http://jeremyko.blogspot.kr/2014/03/c-expression-tree.html

#Usage 

##general arithmetic
```cpp
ExpressionTree expTree;
expression_result* pExpressionRslt;
bool bRslt = false;

bRslt = expTree.SetInfixExpression("1 * -2 ");
EXPECT_TRUE(bRslt);
if (bRslt)
{
   bRslt = expTree.EvaluateExpression();
   EXPECT_TRUE(bRslt);
   pExpressionRslt = expTree.GetResult();
   cout << "expTree.EvaluateExpression returns :" 
        << pExpressionRslt->nResultLong << "\n";
   EXPECT_EQ(-2, pExpressionRslt->nResultLong);
}
```
##custom user functions
```cpp
//see user_functions.h, user_functions.cpp
//-----------------------------------------------------
bRslt = expTree.SetInfixExpression("SumInt4(SumInt4(1,1+1,1+1,2), 1+1, 1+1, 1+1)");
EXPECT_TRUE(bRslt);
bRslt = expTree.EvaluateExpression();
EXPECT_TRUE(bRslt);
pExpressionRslt = expTree.GetResult();
cout << "expTree.EvaluateExpression returns :" 
     << pExpressionRslt->nResultLong << "\n";
EXPECT_EQ(13, pExpressionRslt->nResultLong);
   
//-----------------------------------------------------
bRslt = expTree.SetInfixExpression("GetBool('1'='1')");
EXPECT_TRUE(bRslt);
bRslt = expTree.EvaluateExpression();
EXPECT_TRUE(bRslt);
   
//-----------------------------------------------------
bRslt = expTree.SetInfixExpression("StrCat('ab','cd')=StrCat('a','b')+StrCat('c','d') ");
EXPECT_TRUE(bRslt);
bRslt = expTree.EvaluateExpression();
EXPECT_TRUE(bRslt);
   
//-----------------------------------------------------
bRslt = expTree.SetInfixExpression("StrCat3('1','2',StrCat3('a','b',StrCat3('X','Y','Z')) )");
EXPECT_TRUE(bRslt);
bRslt = expTree.EvaluateExpression();
EXPECT_TRUE(bRslt);
EXPECT_EQ(4, expTree.GetDepth());
pExpressionRslt = expTree.GetResult();
cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->strResult << "\n";
EXPECT_STREQ("12abXYZ", pExpressionRslt->strResult);
```

##using placeholder
```cpp
// placeholder identifier is :$ph1, :$ph2, .... :$ph50 (max)
bRslt = expTree.SetInfixExpression ( ":$ph1+:$ph2" );
EXPECT_TRUE ( bRslt );
if ( bRslt )
{
    expTree.SetNumberLongValueOfPlaceHolder ( 1, 1 );
    expTree.SetNumberLongValueOfPlaceHolder ( 2, 2 );
    bRslt = expTree.EvaluateExpression ( );
    EXPECT_TRUE ( bRslt );
    pExpressionRslt = expTree.GetResult ( );
    cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->nResultLong << "\n";
    EXPECT_EQ ( 3, pExpressionRslt->nResultLong );
        
    //-----------------------------------------------------
    //there's no rebuild tree
    expTree.SetNumberFloatValueOfPlaceHolder ( 1, 2.0f );
    expTree.SetNumberFloatValueOfPlaceHolder ( 2, 3.0f );
    bRslt = expTree.EvaluateExpression ( );
    EXPECT_TRUE ( bRslt );
    pExpressionRslt = expTree.GetResult ( );
    cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->nResultFloat << "\n";
    EXPECT_FLOAT_EQ ( 5.0f, pExpressionRslt->nResultFloat );

    //-----------------------------------------------------
    expTree.SetStringValueOfPlaceHolder ( 1, "A" );
    expTree.SetStringValueOfPlaceHolder ( 2, "B" );
    bRslt = expTree.EvaluateExpression ( );
    EXPECT_TRUE ( bRslt );
    pExpressionRslt = expTree.GetResult ( );
    cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->strResult << "\n";
    EXPECT_STREQ ( "AB", pExpressionRslt->strResult );
}
```

#LICENSE

This projected is licensed under the terms of the BSD license.
