#include "gtest/gtest.h"
#include "expression_tree.h"
#include "user_functions.h"


    ExpressionTree expTree;

    ///////////////////////////////////////////////////////////////////////////////
    TEST(ExpressionTest, TODO_NotImplemented)
    {

        //ExpressionTree expTree;
        bool bRslt = false;
        expression_node* pExpressionRslt;       
        
        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("StrCat3('1','2',StrCat3('a','b',StrCat('x',StrCat3('1','2','3'))) )");
        EXPECT_TRUE(bRslt);
        bRslt = expTree.EvaluateExpression();
        EXPECT_TRUE(bRslt);
        pExpressionRslt = expTree.GetResult();
        cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->strVal << "\n";
        EXPECT_STREQ("12abx123", pExpressionRslt->strVal);

        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("StrCat3('1','2',StrCat3('a','b',StrCat('x','y')) )");
        EXPECT_TRUE(bRslt);
        bRslt = expTree.EvaluateExpression();
        EXPECT_TRUE(bRslt);
        pExpressionRslt = expTree.GetResult();
        cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->strVal << "\n";
        EXPECT_STREQ("12abxy", pExpressionRslt->strVal);

        
        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("StrCat3('1','2',StrCat3('a','b',StrCat3('!','?','X')) )");
        EXPECT_TRUE(bRslt);
        bRslt = expTree.EvaluateExpression();
        EXPECT_TRUE(bRslt);
        pExpressionRslt = expTree.GetResult();
        cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->strVal << "\n";
        EXPECT_STREQ("12ab!?X", pExpressionRslt->strVal);

    }

    ///////////////////////////////////////////////////////////////////////////////
    TEST(ExpressionTest, CHK)
    {
        {
            ExpressionTree expTree;
            bool bRslt = false;
            expression_node* pExpressionRslt;

            cout << "\n---------------------------------------\n";
            bRslt = expTree.SetInfixExpression("StrCat('1',StrCat('a',StrCat('X','Y')) )");
            EXPECT_TRUE(bRslt);
            bRslt = expTree.EvaluateExpression();
            EXPECT_TRUE(bRslt);
            pExpressionRslt = expTree.GetResult();
            cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->strVal << "\n";
            EXPECT_STREQ("1aXY", pExpressionRslt->strVal);
        }

        {
            ExpressionTree expTree;
            bool bRslt = false;
            expression_node* pExpressionRslt;

            cout << "\n---------------------------------------\n";
            bRslt = expTree.SetInfixExpression("StrCat3('1','2',StrCat3('a','b',StrCat3('X','Y','Z')) )");
            EXPECT_TRUE(bRslt);
            bRslt = expTree.EvaluateExpression();
            EXPECT_TRUE(bRslt);
            pExpressionRslt = expTree.GetResult();
            cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->strVal << "\n";
            EXPECT_STREQ("12abXYZ", pExpressionRslt->strVal);
        }               
    }
    ///////////////////////////////////////////////////////////////////////////////
    TEST(ExpressionTest, FLOAT)
    {
        //ExpressionTree expTree;
        bool bRslt = false;
        expression_node* pExpressionRslt;

        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("1.2+2");
        EXPECT_TRUE(bRslt);
        bRslt = expTree.EvaluateExpression();
        EXPECT_TRUE(bRslt);
        pExpressionRslt = expTree.GetResult();
        cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->variable.nFloatValue << "\n";
        EXPECT_FLOAT_EQ((float)3.2, pExpressionRslt->variable.nFloatValue);

        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("1+2.2");
        EXPECT_TRUE(bRslt);
        bRslt = expTree.EvaluateExpression();
        EXPECT_TRUE(bRslt);
        pExpressionRslt = expTree.GetResult();
        cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->variable.nFloatValue << "\n";
        EXPECT_FLOAT_EQ((float)3.2, pExpressionRslt->variable.nFloatValue);

        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("1.2+2.2");
        EXPECT_TRUE(bRslt);
        bRslt = expTree.EvaluateExpression();
        EXPECT_TRUE(bRslt);
        pExpressionRslt = expTree.GetResult();
        cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->variable.nFloatValue << "\n";
        EXPECT_FLOAT_EQ((float)3.4, pExpressionRslt->variable.nFloatValue);

        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("1+2");
        EXPECT_TRUE(bRslt);
        bRslt = expTree.EvaluateExpression();
        EXPECT_TRUE(bRslt);
        pExpressionRslt = expTree.GetResult();
        cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->variable.nLongValue << "\n";
        EXPECT_EQ(3, pExpressionRslt->variable.nLongValue);

        /////////////

        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("2.2-2");
        EXPECT_TRUE(bRslt);
        bRslt = expTree.EvaluateExpression();
        EXPECT_TRUE(bRslt);
        pExpressionRslt = expTree.GetResult();
        cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->variable.nFloatValue << "\n";
        EXPECT_FLOAT_EQ((float)0.2, pExpressionRslt->variable.nFloatValue);

        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("3-2.2");
        EXPECT_TRUE(bRslt);
        bRslt = expTree.EvaluateExpression();
        EXPECT_TRUE(bRslt);
        pExpressionRslt = expTree.GetResult();
        cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->variable.nFloatValue << "\n";
        EXPECT_FLOAT_EQ((float)0.8, pExpressionRslt->variable.nFloatValue);

        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("3.2-2.1");
        EXPECT_TRUE(bRslt);
        bRslt = expTree.EvaluateExpression();
        EXPECT_TRUE(bRslt);
        pExpressionRslt = expTree.GetResult();
        cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->variable.nFloatValue << "\n";
        EXPECT_FLOAT_EQ((float)1.1, pExpressionRslt->variable.nFloatValue);

        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("3-2");
        EXPECT_TRUE(bRslt);
        bRslt = expTree.EvaluateExpression();
        EXPECT_TRUE(bRslt);
        pExpressionRslt = expTree.GetResult();
        cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->variable.nLongValue << "\n";
        EXPECT_EQ(1, pExpressionRslt->variable.nLongValue);


    }

    ///////////////////////////////////////////////////////////////////////////////
    TEST(ExpressionTest, Functions)
    {
        //ExpressionTree expTree;
        bool bRslt = false;
        expression_node* pExpressionRslt;
                
        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("-1.0 / 2.0");
        EXPECT_TRUE(bRslt);
        bRslt = expTree.EvaluateExpression();
        EXPECT_TRUE(bRslt);
        pExpressionRslt = expTree.GetResult();
        cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->variable.nFloatValue << "\n";
        EXPECT_EQ(-0.5, pExpressionRslt->variable.nFloatValue);

        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("-1 + 2");
        EXPECT_TRUE(bRslt);
        bRslt = expTree.EvaluateExpression();
        EXPECT_TRUE(bRslt);
        pExpressionRslt = expTree.GetResult();
        cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->variable.nLongValue << "\n";
        EXPECT_EQ(1, pExpressionRslt->variable.nLongValue);

        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("1+ +2");
        EXPECT_TRUE(bRslt);
        bRslt = expTree.EvaluateExpression();
        EXPECT_TRUE(bRslt);
        pExpressionRslt = expTree.GetResult();
        cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->variable.nLongValue << "\n";
        EXPECT_EQ(3, pExpressionRslt->variable.nLongValue);

        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("StrLen( StrCat('AA', 'BB'))");
        EXPECT_TRUE(bRslt);
        bRslt = expTree.EvaluateExpression();
        EXPECT_TRUE(bRslt);
        pExpressionRslt = expTree.GetResult();
        cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->variable.nLongValue << "\n";
        EXPECT_EQ(4, pExpressionRslt->variable.nLongValue);

        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("StrCat3('1','2','3')");
        EXPECT_TRUE(bRslt);
        bRslt = expTree.EvaluateExpression();
        EXPECT_TRUE(bRslt);
        pExpressionRslt = expTree.GetResult();
        cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->strVal << "\n";
        EXPECT_STREQ("123", pExpressionRslt->strVal);

        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("1+2");
        EXPECT_TRUE(bRslt);
        bRslt = expTree.EvaluateExpression();
        EXPECT_TRUE(bRslt);
        pExpressionRslt = expTree.GetResult();
        cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->variable.nLongValue << "\n";
        EXPECT_EQ(3, pExpressionRslt->variable.nLongValue);


        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("testBool2(10,  1=1)");
        EXPECT_TRUE(bRslt);
        bRslt = expTree.EvaluateExpression();
        EXPECT_TRUE(bRslt);

        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("testBool2(10,  2=1)");
        EXPECT_TRUE(bRslt);
        bRslt = expTree.EvaluateExpression();
        EXPECT_FALSE(bRslt);

        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("SumInt4(SumInt4(1,1+1,1+1,2), 1+1, 1+1, 1+1)");
        EXPECT_TRUE(bRslt);
        bRslt = expTree.EvaluateExpression();
        EXPECT_TRUE(bRslt);
        pExpressionRslt = expTree.GetResult();
        cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->variable.nLongValue << "\n";
        EXPECT_EQ(13, pExpressionRslt->variable.nLongValue);

        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("SumInt4(1+1, 1+1, 1+1, 1+1)");
        EXPECT_TRUE(bRslt);
        bRslt = expTree.EvaluateExpression();
        EXPECT_TRUE(bRslt);
        pExpressionRslt = expTree.GetResult();
        cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->variable.nLongValue << "\n";
        EXPECT_EQ(8, pExpressionRslt->variable.nLongValue);

        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("StrCat3('1','2',StrCat3('a','b','c') )");
        EXPECT_TRUE(bRslt);
        bRslt = expTree.EvaluateExpression();
        EXPECT_TRUE(bRslt);
        pExpressionRslt = expTree.GetResult();
        cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->strVal << "\n";
        EXPECT_STREQ("12abc", pExpressionRslt->strVal);

        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("SumInt4(1+1, 1+1, 1+1, 1+1)");
        EXPECT_TRUE(bRslt);
        bRslt = expTree.EvaluateExpression();
        EXPECT_TRUE(bRslt);
        pExpressionRslt = expTree.GetResult();
        cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->variable.nLongValue << "\n";
        EXPECT_EQ(8, pExpressionRslt->variable.nLongValue);


        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("SumInt3(1+2, 3+4,5+6)");
        EXPECT_TRUE(bRslt);
        bRslt = expTree.EvaluateExpression();
        EXPECT_TRUE(bRslt);
        pExpressionRslt = expTree.GetResult();
        cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->variable.nLongValue << "\n";
        EXPECT_EQ(21, pExpressionRslt->variable.nLongValue);

        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("testBool(10, 'a', 1=1)");
        EXPECT_TRUE(bRslt);
        bRslt = expTree.EvaluateExpression();
        EXPECT_TRUE(bRslt);

        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("StrCat('1'+'2', '3')");
        EXPECT_TRUE(bRslt);
        bRslt = expTree.EvaluateExpression();
        EXPECT_TRUE(bRslt);
        pExpressionRslt = expTree.GetResult();
        cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->strVal << "\n";
        EXPECT_STREQ("123", pExpressionRslt->strVal);

        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("SumInt(1+2, 2)");
        EXPECT_TRUE(bRslt);
        bRslt = expTree.EvaluateExpression();
        EXPECT_TRUE(bRslt);
        pExpressionRslt = expTree.GetResult();
        cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->variable.nLongValue << "\n";
        EXPECT_EQ(5, pExpressionRslt->variable.nLongValue);

        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("GetBool('1'='1')");
        EXPECT_TRUE(bRslt);
        bRslt = expTree.EvaluateExpression();
        EXPECT_TRUE(bRslt);

        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("GetBool('1'='2')");
        EXPECT_TRUE(bRslt);
        bRslt = expTree.EvaluateExpression();
        EXPECT_FALSE(bRslt);

        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("GetBool(1=1)");
        EXPECT_TRUE(bRslt);
        bRslt = expTree.EvaluateExpression();
        EXPECT_TRUE(bRslt);

        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("GetBool(1=2)");
        EXPECT_TRUE(bRslt);
        bRslt = expTree.EvaluateExpression();
        EXPECT_FALSE(bRslt);

        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("AsciiToInt( StrCat('1','0')+'0')");
        EXPECT_TRUE(bRslt);
        bRslt = expTree.EvaluateExpression();
        EXPECT_TRUE(bRslt);
        pExpressionRslt = expTree.GetResult();
        cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->variable.nLongValue << "\n";
        EXPECT_EQ(100, pExpressionRslt->variable.nLongValue);

        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("SumInt( SumInt( 1, 1), 2)");
        EXPECT_TRUE(bRslt);
        bRslt = expTree.EvaluateExpression();
        EXPECT_TRUE(bRslt);
        pExpressionRslt = expTree.GetResult();
        cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->variable.nLongValue << "\n";
        EXPECT_EQ(4, pExpressionRslt->variable.nLongValue);

        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("SumInt( 1, 2)");
        EXPECT_TRUE(bRslt);
        bRslt = expTree.EvaluateExpression();
        EXPECT_TRUE(bRslt);
        pExpressionRslt = expTree.GetResult();
        cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->variable.nLongValue << "\n";
        EXPECT_EQ(3, pExpressionRslt->variable.nLongValue);

        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("SumInt( (1), (2))");
        EXPECT_TRUE(bRslt);
        bRslt = expTree.EvaluateExpression();
        EXPECT_TRUE(bRslt);
        pExpressionRslt = expTree.GetResult();
        cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->variable.nLongValue << "\n";
        EXPECT_EQ(3, pExpressionRslt->variable.nLongValue);

        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("(( SumInt(0,1) ))");
        EXPECT_TRUE(bRslt);
        bRslt = expTree.EvaluateExpression();
        EXPECT_TRUE(bRslt);
        pExpressionRslt = expTree.GetResult();
        cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->variable.nLongValue << "\n";
        EXPECT_EQ(1, pExpressionRslt->variable.nLongValue);

        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("SumInt( (SumInt(0,1)),SumInt(1,1) )");
        EXPECT_TRUE(bRslt);
        bRslt = expTree.EvaluateExpression();
        EXPECT_TRUE(bRslt);
        pExpressionRslt = expTree.GetResult();
        cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->variable.nLongValue << "\n";
        EXPECT_EQ(3, pExpressionRslt->variable.nLongValue);

        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("StrCat('a','b')");
        EXPECT_TRUE(bRslt);
        bRslt = expTree.EvaluateExpression();
        EXPECT_TRUE(bRslt);
        pExpressionRslt = expTree.GetResult();
        cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->strVal << "\n";
        EXPECT_STREQ("ab", pExpressionRslt->strVal);

        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("StrCat('a','b')+StrCat('c','d')");
        EXPECT_TRUE(bRslt);
        bRslt = expTree.EvaluateExpression();
        EXPECT_TRUE(bRslt);
        pExpressionRslt = expTree.GetResult();
        cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->strVal << "\n";
        EXPECT_STREQ("abcd", pExpressionRslt->strVal);

        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("'abcd'=StrCat('a','b')+StrCat('c','d')");
        EXPECT_TRUE(bRslt);
        bRslt = expTree.EvaluateExpression();
        EXPECT_TRUE(bRslt);

        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("StrCat('a','b')+StrCat('c','d')='abcd' ");
        EXPECT_TRUE(bRslt);
        bRslt = expTree.EvaluateExpression();
        EXPECT_TRUE(bRslt);

        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("StrCat('a','b')=StrCat('c','d') ");
        EXPECT_TRUE(bRslt);
        bRslt = expTree.EvaluateExpression();
        EXPECT_FALSE(bRslt);

        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("StrCat('ab','cd')=StrCat('a','b')+StrCat('c','d') ");
        EXPECT_TRUE(bRslt);
        bRslt = expTree.EvaluateExpression();
        EXPECT_TRUE(bRslt);

        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("AsciiToInt('123') ");
        EXPECT_TRUE(bRslt);
        if (bRslt)
        {
            bRslt = expTree.EvaluateExpression();
            pExpressionRslt = expTree.GetResult();
            cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->variable.nLongValue << "\n";
            EXPECT_EQ(123, pExpressionRslt->variable.nLongValue);
        }

        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("AsciiToInt('123')+StrLen('abc') ");
        EXPECT_TRUE(bRslt);
        if (bRslt)
        {
            bRslt = expTree.EvaluateExpression();
            pExpressionRslt = expTree.GetResult();
            cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->variable.nLongValue << "\n";
            EXPECT_EQ(126, pExpressionRslt->variable.nLongValue);
        }

        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("126=AsciiToInt('123')+StrLen('abc') ");
        EXPECT_TRUE(bRslt);
        if (bRslt)
        {
            bRslt = expTree.EvaluateExpression();
            EXPECT_TRUE(bRslt);
        }

        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("SumInt(123,3)=AsciiToInt('123')+StrLen('abc') ");
        EXPECT_TRUE(bRslt);
        if (bRslt)
        {
            bRslt = expTree.EvaluateExpression();
            EXPECT_TRUE(bRslt);
        }

        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("123+3=AsciiToInt('123')+StrLen('abc') ");
        EXPECT_TRUE(bRslt);
        if (bRslt)
        {
            bRslt = expTree.EvaluateExpression();
            EXPECT_TRUE(bRslt);
        }

        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("10 - SumInt(2, 3) > 2");
        EXPECT_TRUE(bRslt);
        if (bRslt)
        {

            bRslt = expTree.EvaluateExpression();
            EXPECT_TRUE(bRslt);
        }

        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("10 - SumInt(2, 3) ");
        EXPECT_TRUE(bRslt);
        if (bRslt)
        {

            bRslt = expTree.EvaluateExpression();
            pExpressionRslt = expTree.GetResult();
            cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->variable.nLongValue << "\n";
            EXPECT_EQ(5, pExpressionRslt->variable.nLongValue);
        }


        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("StrLen( StrCat( StrCat('AA', 'BB'), 'XX' ) )+2");
        EXPECT_TRUE(bRslt);
        bRslt = expTree.EvaluateExpression();
        EXPECT_TRUE(bRslt);
        pExpressionRslt = expTree.GetResult();
        cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->variable.nLongValue << "\n";
        EXPECT_EQ(8, pExpressionRslt->variable.nLongValue);
        // Postfix is:   'AA' 'BB' StrCat StrLen 2 +



        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("StrLen( 'AA')");
        EXPECT_TRUE(bRslt);
        bRslt = expTree.EvaluateExpression();
        EXPECT_TRUE(bRslt);
        pExpressionRslt = expTree.GetResult();
        cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->variable.nLongValue << "\n";
        EXPECT_EQ(2, pExpressionRslt->variable.nLongValue);

        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("StrLen( StrCat('AA', 'BB'))");
        EXPECT_TRUE(bRslt);
        bRslt = expTree.EvaluateExpression();
        EXPECT_TRUE(bRslt);
        pExpressionRslt = expTree.GetResult();
        cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->variable.nLongValue << "\n";
        EXPECT_EQ(4, pExpressionRslt->variable.nLongValue);


        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("StrLen( StrCat('A', 'B')+StrCat('C', 'D'))");
        EXPECT_TRUE(bRslt);
        bRslt = expTree.EvaluateExpression();
        EXPECT_TRUE(bRslt);
        pExpressionRslt = expTree.GetResult();
        cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->variable.nLongValue << "\n";
        EXPECT_EQ(4, pExpressionRslt->variable.nLongValue);


        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("StrCat3('1','2', '3')");
        EXPECT_TRUE(bRslt);
        bRslt = expTree.EvaluateExpression();
        EXPECT_TRUE(bRslt);
        pExpressionRslt = expTree.GetResult();
        cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->strVal << "\n";
        EXPECT_STREQ("123", pExpressionRslt->strVal);

        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("StrCat4('1','2', '3','4')");
        EXPECT_TRUE(bRslt);
        bRslt = expTree.EvaluateExpression();
        EXPECT_TRUE(bRslt);
        pExpressionRslt = expTree.GetResult();
        cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->strVal << "\n";
        EXPECT_STREQ("1234", pExpressionRslt->strVal);

        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("StrCat('a','b')+StrCat4('1','2', '3','4')");
        EXPECT_TRUE(bRslt);
        bRslt = expTree.EvaluateExpression();
        EXPECT_TRUE(bRslt);
        pExpressionRslt = expTree.GetResult();
        cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->strVal << "\n";
        EXPECT_STREQ("ab1234", pExpressionRslt->strVal);

        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("GetInt100()");
        EXPECT_TRUE(bRslt);
        bRslt = expTree.EvaluateExpression();
        EXPECT_TRUE(bRslt);
        pExpressionRslt = expTree.GetResult();
        cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->variable.nLongValue << "\n";
        EXPECT_EQ(100, pExpressionRslt->variable.nLongValue);

        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("((1+2)*(4-2))+SumInt(1,2)");
        EXPECT_TRUE(bRslt);
        bRslt = expTree.EvaluateExpression();
        EXPECT_TRUE(bRslt);
        pExpressionRslt = expTree.GetResult();
        cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->variable.nLongValue << "\n";
        EXPECT_EQ(9, pExpressionRslt->variable.nLongValue);

        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("AsciiToInt( '1')");
        EXPECT_TRUE(bRslt);
        bRslt = expTree.EvaluateExpression();
        EXPECT_TRUE(bRslt);
        pExpressionRslt = expTree.GetResult();
        cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->variable.nLongValue << "\n";
        EXPECT_EQ(1, pExpressionRslt->variable.nLongValue);

        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("10 - AsciiToInt( StrCat('1','0')+'0')");
        EXPECT_TRUE(bRslt);
        bRslt = expTree.EvaluateExpression();
        EXPECT_TRUE(bRslt);
        pExpressionRslt = expTree.GetResult();
        cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->variable.nLongValue << "\n";
        EXPECT_EQ(-90, pExpressionRslt->variable.nLongValue);


        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("StrLen('abc')");
        EXPECT_TRUE(bRslt);
        bRslt = expTree.EvaluateExpression();
        EXPECT_TRUE(bRslt);
        pExpressionRslt = expTree.GetResult();
        cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->variable.nLongValue << "\n";
        EXPECT_EQ(3, pExpressionRslt->variable.nLongValue);

        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("StrLen( 'abs')");
        EXPECT_TRUE(bRslt);
        bRslt = expTree.EvaluateExpression();
        EXPECT_TRUE(bRslt);
        pExpressionRslt = expTree.GetResult();
        cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->variable.nLongValue << "\n";
        EXPECT_EQ(3, pExpressionRslt->variable.nLongValue);

        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("StrLen( 'abs 2' )");
        EXPECT_TRUE(bRslt);
        bRslt = expTree.EvaluateExpression();
        EXPECT_TRUE(bRslt);
        pExpressionRslt = expTree.GetResult();
        cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->variable.nLongValue << "\n";
        EXPECT_EQ(5, pExpressionRslt->variable.nLongValue);


        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("StrLen( 'abs 2' )+StrLen( 'aaa' )");
        EXPECT_TRUE(bRslt);
        bRslt = expTree.EvaluateExpression();
        EXPECT_TRUE(bRslt);
        pExpressionRslt = expTree.GetResult();
        cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->variable.nLongValue << "\n";
        EXPECT_EQ(8, pExpressionRslt->variable.nLongValue);

        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("StrLen('a')+StrLen('b')+StrLen('c')+StrLen('d')");
        EXPECT_TRUE(bRslt);
        bRslt = expTree.EvaluateExpression();
        EXPECT_TRUE(bRslt);
        pExpressionRslt = expTree.GetResult();
        cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->variable.nLongValue << "\n";
        EXPECT_EQ(4, pExpressionRslt->variable.nLongValue);

        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("StrLen('aa')+StrLen('bb')*StrLen('ccc')");
        EXPECT_TRUE(bRslt);
        bRslt = expTree.EvaluateExpression();
        EXPECT_TRUE(bRslt);
        pExpressionRslt = expTree.GetResult();
        cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->variable.nLongValue << "\n";
        EXPECT_EQ(8, pExpressionRslt->variable.nLongValue);



        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("8=StrLen( 'abs 2' )+StrLen( 'aaa' )");
        EXPECT_TRUE(bRslt);
        bRslt = expTree.EvaluateExpression();
        EXPECT_TRUE(bRslt);

        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("2!=StrLen( 'abs 2' )+StrLen( 'aaa' )");
        EXPECT_TRUE(bRslt);
        bRslt = expTree.EvaluateExpression();
        EXPECT_TRUE(bRslt);

        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("2=StrLen( 'abs 2' )+StrLen( 'aaa' )");
        EXPECT_TRUE(bRslt);
        bRslt = expTree.EvaluateExpression();
        EXPECT_FALSE(bRslt);

        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("StrLen( 'abs 2' )+StrLen( 'aaa' )=8");
        EXPECT_TRUE(bRslt);
        bRslt = expTree.EvaluateExpression();
        EXPECT_TRUE(bRslt);

        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("StrLen( 'abs 2' )+StrLen( 'aaa' )!=1");
        EXPECT_TRUE(bRslt);
        bRslt = expTree.EvaluateExpression();
        EXPECT_TRUE(bRslt);

        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("StrLen( 'abs 2' )+StrLen( 'aaa' )=1");
        EXPECT_TRUE(bRslt);
        bRslt = expTree.EvaluateExpression();
        EXPECT_FALSE(bRslt);


        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("StrLen('abc')+1+StrLen('efg')");
        EXPECT_TRUE(bRslt);
        bRslt = expTree.EvaluateExpression();
        EXPECT_TRUE(bRslt);
        cout << "expTree.EvaluateExpression returns :" << bRslt << "\n";
        pExpressionRslt = expTree.GetResult();
        cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->variable.nLongValue << "\n";
        EXPECT_EQ(7, pExpressionRslt->variable.nLongValue);

        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("7=StrLen('abc')+1+StrLen('efg')");
        EXPECT_TRUE(bRslt);
        bRslt = expTree.EvaluateExpression();
        EXPECT_TRUE(bRslt);
        cout << "expTree.EvaluateExpression returns :" << bRslt << "\n";

        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("SumInt( StrLen( 'abc'), StrLen('def')) )");
        EXPECT_TRUE(bRslt);
        bRslt = expTree.EvaluateExpression();
        EXPECT_TRUE(bRslt);
        pExpressionRslt = expTree.GetResult();
        cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->variable.nLongValue << "\n";
        EXPECT_EQ(6, pExpressionRslt->variable.nLongValue);

        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("SumInt( 1, StrLen( StrCat('A', 'B')) )");
        EXPECT_TRUE(bRslt);
        bRslt = expTree.EvaluateExpression();
        EXPECT_TRUE(bRslt);
        pExpressionRslt = expTree.GetResult();
        cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->variable.nLongValue << "\n";
        EXPECT_EQ(3, pExpressionRslt->variable.nLongValue);

        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("SumInt( 1, SumInt( 1, StrLen( StrCat('A', 'B')) ))");
        EXPECT_TRUE(bRslt);
        bRslt = expTree.EvaluateExpression();
        EXPECT_TRUE(bRslt);
        pExpressionRslt = expTree.GetResult();
        cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->variable.nLongValue << "\n";
        EXPECT_EQ(4, pExpressionRslt->variable.nLongValue);

        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("100 - SumInt( 1, SumInt( 1, StrLen( StrCat('A', 'B')) ))");
        EXPECT_TRUE(bRslt);
        bRslt = expTree.EvaluateExpression();
        EXPECT_TRUE(bRslt);
        pExpressionRslt = expTree.GetResult();
        cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->variable.nLongValue << "\n";
        EXPECT_EQ(96, pExpressionRslt->variable.nLongValue);

        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("100 - AsciiToInt( '100')");
        EXPECT_TRUE(bRslt);
        bRslt = expTree.EvaluateExpression();
        EXPECT_TRUE(bRslt);
        pExpressionRslt = expTree.GetResult();
        cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->variable.nLongValue << "\n";
        EXPECT_EQ(0, pExpressionRslt->variable.nLongValue);

        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("GetStrSum3(10,'20',30)");
        EXPECT_TRUE(bRslt);
        bRslt = expTree.EvaluateExpression();
        EXPECT_TRUE(bRslt);
        pExpressionRslt = expTree.GetResult();
        cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->strVal << "\n";
        EXPECT_STREQ("60", pExpressionRslt->strVal);

        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("GetStrSum(10,'20')");
        EXPECT_TRUE(bRslt);
        bRslt = expTree.EvaluateExpression();
        EXPECT_TRUE(bRslt);
        pExpressionRslt = expTree.GetResult();
        cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->strVal << "\n";
        EXPECT_STREQ("30", pExpressionRslt->strVal);


        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("GetStrSumAndPlus100(10,20)");
        EXPECT_TRUE(bRslt);
        bRslt = expTree.EvaluateExpression();
        EXPECT_TRUE(bRslt);
        pExpressionRslt = expTree.GetResult();
        cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->strVal << "\n";
        EXPECT_STREQ("130", pExpressionRslt->strVal);

        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("GetStrPlus100(10)");
        EXPECT_TRUE(bRslt);
        bRslt = expTree.EvaluateExpression();
        EXPECT_TRUE(bRslt);
        pExpressionRslt = expTree.GetResult();
        cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->strVal << "\n";

    }


    ///////////////////////////////////////////////////////////////////////////////
    TEST(ExpressionTest, Literal)
    {
        //ExpressionTree expTree;
        bool bRslt = false;

        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("'12'='1'+'2'");
        EXPECT_TRUE(bRslt);
        bRslt = expTree.EvaluateExpression();
        EXPECT_TRUE(bRslt);
        cout << "expTree.EvaluateExpression returns :" << bRslt << "\n";
        EXPECT_TRUE(bRslt);

        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("ABC='A'+B+'C' ");
        EXPECT_TRUE(bRslt);
        bRslt = expTree.EvaluateExpression();
        EXPECT_TRUE(bRslt);
        cout << "expTree.EvaluateExpression returns :" << bRslt << "\n";

        // Postfix is:  ABC 'A' B+ 'C' + =

        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("'1'='1'");
        EXPECT_TRUE(bRslt);
        bRslt = expTree.EvaluateExpression();
        cout << "expTree.EvaluateExpression returns :" << bRslt << "\n";
        EXPECT_TRUE(bRslt);

        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("'1'='2'");
        EXPECT_TRUE(bRslt);
        bRslt = expTree.EvaluateExpression();
        cout << "expTree.EvaluateExpression returns :" << bRslt << "\n";
        EXPECT_FALSE(bRslt);

        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("AB=A+B");
        EXPECT_TRUE(bRslt);
        bRslt = expTree.EvaluateExpression();
        cout << "expTree.EvaluateExpression returns :" << bRslt << "\n";
        EXPECT_TRUE(bRslt);

        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("10='2'");
        EXPECT_TRUE(bRslt);
        bRslt = expTree.EvaluateExpression();
        cout << "expTree.EvaluateExpression returns :" << bRslt << "\n";
        EXPECT_FALSE(bRslt);

        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression(" '1' = '1' ");
        EXPECT_TRUE(bRslt);
        //Postfix is:  '1' '1' =
        bRslt = expTree.EvaluateExpression();
        cout << "expTree.EvaluateExpression returns :" << bRslt << "\n";
        EXPECT_TRUE(bRslt);

    }



    ///////////////////////////////////////////////////////////////////////////////
    TEST(ExpressionTest, Numeric)
    {
        //ExpressionTree expTree;
        expression_node* pExpressionRslt;
        bool bRslt = false;

        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("2+1*3/3-1");
        EXPECT_TRUE(bRslt);
        bRslt = expTree.EvaluateExpression();
        EXPECT_TRUE(bRslt);
        pExpressionRslt = expTree.GetResult();
        cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->variable.nLongValue << "\n";
        EXPECT_EQ(2, pExpressionRslt->variable.nLongValue);

        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("10=9+1");
        EXPECT_TRUE(bRslt);
        bRslt = expTree.EvaluateExpression();
        cout << "expTree.EvaluateExpression returns :" << bRslt << "\n";
        EXPECT_TRUE(bRslt);

        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("3=1+2");
        EXPECT_TRUE(bRslt);
        bRslt = expTree.EvaluateExpression();
        cout << "expTree.EvaluateExpression returns :" << bRslt << "\n";
        EXPECT_TRUE(bRslt);

        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("1+1");
        EXPECT_TRUE(bRslt);
        bRslt = expTree.EvaluateExpression();
        EXPECT_TRUE(bRslt);
        pExpressionRslt = expTree.GetResult();
        cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->variable.nLongValue << "\n";
        EXPECT_EQ(2, pExpressionRslt->variable.nLongValue);

        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("10='2'");
        EXPECT_TRUE(bRslt);
        bRslt = expTree.EvaluateExpression();
        cout << "expTree.EvaluateExpression returns :" << bRslt << "\n";
        EXPECT_FALSE(bRslt);

        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("2>1");
        EXPECT_TRUE(bRslt);
        bRslt = expTree.EvaluateExpression();
        cout << "expTree.EvaluateExpression returns :" << bRslt << "\n";
        EXPECT_TRUE(bRslt);

        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("(1+1)>1");
        EXPECT_TRUE(bRslt);
        bRslt = expTree.EvaluateExpression();
        cout << "expTree.EvaluateExpression returns :" << bRslt << "\n";
        EXPECT_TRUE(bRslt);

        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("1+1=2");
        EXPECT_TRUE(bRslt);
        bRslt = expTree.EvaluateExpression();
        cout << "expTree.EvaluateExpression returns :" << bRslt << "\n";
        EXPECT_TRUE(bRslt);

        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("1+1>1");
        EXPECT_TRUE(bRslt);
        bRslt = expTree.EvaluateExpression();
        cout << "expTree.EvaluateExpression returns :" << bRslt << "\n";
        EXPECT_TRUE(bRslt);

        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("2 >= 1");
        EXPECT_TRUE(bRslt);
        bRslt = expTree.EvaluateExpression();
        cout << "expTree.EvaluateExpression returns :" << bRslt << "\n";
        EXPECT_TRUE(bRslt);

        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("2 <= 1");
        EXPECT_TRUE(bRslt);
        bRslt = expTree.EvaluateExpression();
        cout << "expTree.EvaluateExpression returns :" << bRslt << "\n";
        EXPECT_FALSE(bRslt);

        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("1+2+3");
        EXPECT_TRUE(bRslt);
        bRslt = expTree.EvaluateExpression();
        EXPECT_TRUE(bRslt);
        pExpressionRslt = expTree.GetResult();
        cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->variable.nLongValue << "\n";
        EXPECT_EQ(6, pExpressionRslt->variable.nLongValue);


        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("1+2*3");
        EXPECT_TRUE(bRslt);
        bRslt = expTree.EvaluateExpression();
        EXPECT_TRUE(bRslt);
        pExpressionRslt = expTree.GetResult();
        cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->variable.nLongValue << "\n";
        EXPECT_EQ(7, pExpressionRslt->variable.nLongValue);

        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("(1+2)*3");
        EXPECT_TRUE(bRslt);
        bRslt = expTree.EvaluateExpression();
        EXPECT_TRUE(bRslt);
        pExpressionRslt = expTree.GetResult();
        cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->variable.nLongValue << "\n";
        EXPECT_EQ(9, pExpressionRslt->variable.nLongValue);

        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("1*2+3");
        EXPECT_TRUE(bRslt);
        bRslt = expTree.EvaluateExpression();
        EXPECT_TRUE(bRslt);
        pExpressionRslt = expTree.GetResult();
        cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->variable.nLongValue << "\n";
        EXPECT_EQ(5, pExpressionRslt->variable.nLongValue);


        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("1+4/2");
        EXPECT_TRUE(bRslt);
        bRslt = expTree.EvaluateExpression();
        EXPECT_TRUE(bRslt);
        pExpressionRslt = expTree.GetResult();
        cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->variable.nLongValue << "\n";
        EXPECT_EQ(3, pExpressionRslt->variable.nLongValue);

        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("(1+(2-3))*4");
        EXPECT_TRUE(bRslt);
        bRslt = expTree.EvaluateExpression();
        EXPECT_TRUE(bRslt);
        pExpressionRslt = expTree.GetResult();
        cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->variable.nLongValue << "\n";
        EXPECT_EQ(0, pExpressionRslt->variable.nLongValue);

        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("10+20+30");
        EXPECT_TRUE(bRslt);
        bRslt = expTree.EvaluateExpression();
        EXPECT_TRUE(bRslt);
        pExpressionRslt = expTree.GetResult();
        cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->variable.nLongValue << "\n";
        EXPECT_EQ(60, pExpressionRslt->variable.nLongValue);

        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("10=20");
        EXPECT_TRUE(bRslt);
        bRslt = expTree.EvaluateExpression();
        cout << "expTree.EvaluateExpression returns :" << bRslt << "\n";
        EXPECT_FALSE(bRslt);

        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("10!=20");
        EXPECT_TRUE(bRslt);
        bRslt = expTree.EvaluateExpression();
        cout << "expTree.EvaluateExpression returns :" << bRslt << "\n";
        EXPECT_TRUE(bRslt);

        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("GetInt100()");
        EXPECT_TRUE(bRslt);
        bRslt = expTree.EvaluateExpression();
        EXPECT_TRUE(bRslt);
        pExpressionRslt = expTree.GetResult();
        cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->variable.nLongValue << "\n";
        EXPECT_EQ(100, pExpressionRslt->variable.nLongValue);


        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("SumInt(StrLen('a'), GetInt100())");
        EXPECT_TRUE(bRslt);
        bRslt = expTree.EvaluateExpression();
        EXPECT_TRUE(bRslt);
        pExpressionRslt = expTree.GetResult();
        cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->variable.nLongValue << "\n";
        EXPECT_EQ(101, pExpressionRslt->variable.nLongValue);
        //EXPECT_GT(2, 0);

        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("1+2+3+4+5+6+7+8+9+10+11+12+13+14+15+16+17+18+19+20");
        EXPECT_TRUE(bRslt);
        bRslt = expTree.EvaluateExpression();
        EXPECT_TRUE(bRslt);
        pExpressionRslt = expTree.GetResult();
        cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->variable.nLongValue << "\n";
        EXPECT_EQ(210, pExpressionRslt->variable.nLongValue);

    }

    ///////////////////////////////////////////////////////////////////////////////
    TEST(ExpressionTest, negative_positive)
    {
        // ExpressionTree expTree;
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
            cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->variable.nLongValue << "\n";
            EXPECT_EQ(-2, pExpressionRslt->variable.nLongValue);
        }

        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("1 * (-2) ");
        EXPECT_TRUE(bRslt);
        if (bRslt)
        {
            bRslt = expTree.EvaluateExpression();
            EXPECT_TRUE(bRslt);
            pExpressionRslt = expTree.GetResult();
            cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->variable.nLongValue << "\n";
            EXPECT_EQ(-2, pExpressionRslt->variable.nLongValue);
        }

        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("1*-2 ");
        EXPECT_TRUE(bRslt);
        if (bRslt)
        {
            bRslt = expTree.EvaluateExpression();
            EXPECT_TRUE(bRslt);
            pExpressionRslt = expTree.GetResult();
            cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->variable.nLongValue << "\n";
            EXPECT_EQ(-2, pExpressionRslt->variable.nLongValue);
        }

        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("1 - -1 ");
        EXPECT_TRUE(bRslt);
        if (bRslt)
        {
            bRslt = expTree.EvaluateExpression();
            EXPECT_TRUE(bRslt);
            pExpressionRslt = expTree.GetResult();
            cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->variable.nLongValue << "\n";
            EXPECT_EQ(2, pExpressionRslt->variable.nLongValue);
        }

        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("1 - (-1) ");
        EXPECT_TRUE(bRslt);
        if (bRslt)
        {
            bRslt = expTree.EvaluateExpression();
            EXPECT_TRUE(bRslt);
            pExpressionRslt = expTree.GetResult();
            cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->variable.nLongValue << "\n";
            EXPECT_EQ(2, pExpressionRslt->variable.nLongValue);
        }

        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("1 - -1 + 2 * -1");
        EXPECT_TRUE(bRslt);
        if (bRslt)
        {
            bRslt = expTree.EvaluateExpression();
            EXPECT_TRUE(bRslt);
            pExpressionRslt = expTree.GetResult();
            cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->variable.nLongValue << "\n";
            EXPECT_EQ(0, pExpressionRslt->variable.nLongValue);
        }

    }

    ///////////////////////////////////////////////////////////////////////////////
    TEST(ExpressionTest, AND_OR)
    {
        //ExpressionTree expTree;
        //expression_node* pExpressionRslt;
        bool bRslt = false;


        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("3=StrLen('abs') & 1=1");
        EXPECT_TRUE(bRslt);
        if (bRslt)
        {
            bRslt = expTree.EvaluateExpression();
            EXPECT_TRUE(bRslt);
        }

        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("1=StrLen('abs') & 1=1");
        EXPECT_TRUE(bRslt);
        if (bRslt)
        {
            bRslt = expTree.EvaluateExpression();
            EXPECT_FALSE(bRslt);
        }

        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("2=1+1 & 1=1");
        EXPECT_TRUE(bRslt);
        if (bRslt)
        {
            bRslt = expTree.EvaluateExpression();
            EXPECT_TRUE(bRslt);
        }

        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("2=1+1 & 1=1 & 2=0");
        EXPECT_TRUE(bRslt);
        if (bRslt)
        {
            bRslt = expTree.EvaluateExpression();
            EXPECT_FALSE(bRslt);
        }

        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("'11'='1'+'1' & 'A'='A'");
        EXPECT_TRUE(bRslt);
        if (bRslt)
        {
            bRslt = expTree.EvaluateExpression();
            EXPECT_TRUE(bRslt);
        }
        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("3=1+1 | 1=1");
        EXPECT_TRUE(bRslt);
        if (bRslt)
        {
            bRslt = expTree.EvaluateExpression();
            EXPECT_TRUE(bRslt);
        }
        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("3=1+2 | 1=0");
        EXPECT_TRUE(bRslt);
        if (bRslt)
        {
            bRslt = expTree.EvaluateExpression();
            EXPECT_TRUE(bRslt);
        }

        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("3=1 | 1=0");
        EXPECT_TRUE(bRslt);
        if (bRslt)
        {
            bRslt = expTree.EvaluateExpression();
            EXPECT_FALSE(bRslt);
        }

        cout << "\n---------------------------------------\n";
        bRslt = expTree.SetInfixExpression("'13'='1'+'3' | 'A'='X'");
        EXPECT_TRUE(bRslt);
        if (bRslt)
        {
            bRslt = expTree.EvaluateExpression();
            EXPECT_TRUE(bRslt);
        }
    }
