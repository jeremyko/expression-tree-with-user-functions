#include <gtest/gtest.h>
#include "expression_tree.h"
#include "user_functions.h"
#include <math.h>


///////////////////////////////////////////////////////////////////////////////
TEST(ExpressionTest, PlaceHolderTest)
{       
    ExpressionTree expTree;
    bool bRslt = false;    
    expression_result * pExpressionRslt;

    cout << "\n     1+2\n";
    bRslt = expTree.SetInfixExpression ( ":$ph1+ AsciiToInt(:$ph2)" );
    EXPECT_TRUE ( bRslt );
    if ( bRslt )
    {
        expTree.SetNumberLongValueOfPlaceHolder ( 1, 1 );
        expTree.SetStringValueOfPlaceHolder ( 2, "2" );        
        bRslt = expTree.EvaluateExpression ( );
        EXPECT_TRUE ( bRslt );
        pExpressionRslt = expTree.GetResult ( );
        cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->nResultLong << "\n";
        EXPECT_EQ ( 3, pExpressionRslt->nResultLong );

        expTree.SetNumberLongValueOfPlaceHolder ( 1, 2 );
        expTree.SetStringValueOfPlaceHolder ( 2, "3" );
        bRslt = expTree.EvaluateExpression ( );
        EXPECT_TRUE ( bRslt );
        pExpressionRslt = expTree.GetResult ( );
        cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->nResultLong << "\n";
        EXPECT_EQ ( 5, pExpressionRslt->nResultLong );

        expTree.SetNumberLongValueOfPlaceHolder ( 1, 4 );
        expTree.SetStringValueOfPlaceHolder ( 2, "5" );
        bRslt = expTree.EvaluateExpression ( );
        EXPECT_TRUE ( bRslt );
        pExpressionRslt = expTree.GetResult ( );
        cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->nResultLong << "\n";
        EXPECT_EQ ( 9, pExpressionRslt->nResultLong );
    }

    cout << "\n     1+2+3+4+5 \n";
    bRslt = expTree.SetInfixExpression ( ":$ph1+ AsciiToInt(:$ph2)+ SumInt(:$ph3, SumInt(:$ph4, :$ph5))" );
    EXPECT_TRUE ( bRslt );
    if ( bRslt )
    {
        expTree.SetNumberLongValueOfPlaceHolder ( 1, 1 );
        expTree.SetStringValueOfPlaceHolder ( 2, "2" );
        expTree.SetNumberLongValueOfPlaceHolder ( 3, 3 );
        expTree.SetNumberLongValueOfPlaceHolder ( 4, 4 );
        expTree.SetNumberLongValueOfPlaceHolder ( 5, 5 );
        bRslt = expTree.EvaluateExpression ( );
        EXPECT_TRUE ( bRslt );
        pExpressionRslt = expTree.GetResult ( );
        cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->nResultLong << "\n";
        EXPECT_EQ ( 15, pExpressionRslt->nResultLong );
    }

    cout << "\n---------------------------------------\n";
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

        expTree.SetNumberLongValueOfPlaceHolder ( 1, 2 );
        expTree.SetNumberLongValueOfPlaceHolder ( 2, 3 );
        bRslt = expTree.EvaluateExpression ( );
        EXPECT_TRUE ( bRslt );
        pExpressionRslt = expTree.GetResult ( );
        cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->nResultLong << "\n";
        EXPECT_EQ ( 5, pExpressionRslt->nResultLong );

        expTree.SetStringValueOfPlaceHolder ( 1, "A" );
        expTree.SetStringValueOfPlaceHolder ( 2, "B" );
        bRslt = expTree.EvaluateExpression ( );
        EXPECT_TRUE ( bRslt );
        pExpressionRslt = expTree.GetResult ( );
        cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->strResult << "\n";
        EXPECT_STREQ ( "AB", pExpressionRslt->strResult );
    }

    cout << "\n---------------------------------------\n";
    bRslt = expTree.SetInfixExpression ( "StrCat3(:$ph1,StrCat3(:$ph2,:$ph3,StrCat3(:$ph4,:$ph5,StrCat3(:$ph6,:$ph7,StrCat3(:$ph8,:$ph9,:$ph10)))),'X')" );
    EXPECT_TRUE ( bRslt );
    EXPECT_TRUE ( bRslt );
    if ( bRslt )
    {
        expTree.SetStringValueOfPlaceHolder ( 1, "1" );
        expTree.SetStringValueOfPlaceHolder ( 2, "2" );
        expTree.SetStringValueOfPlaceHolder ( 3, "3" );
        expTree.SetStringValueOfPlaceHolder ( 4, "4" );
        expTree.SetStringValueOfPlaceHolder ( 5, "5" );
        expTree.SetStringValueOfPlaceHolder ( 6, "6" );
        expTree.SetStringValueOfPlaceHolder ( 7, "7" );
        expTree.SetStringValueOfPlaceHolder ( 8, "8" );
        expTree.SetStringValueOfPlaceHolder ( 9, "9" );
        expTree.SetStringValueOfPlaceHolder ( 10, "10" );
        bRslt = expTree.EvaluateExpression ( );
        EXPECT_TRUE ( bRslt );
        pExpressionRslt = expTree.GetResult ( );
        cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->strResult << "\n";
        EXPECT_STREQ ( "12345678910X", pExpressionRslt->strResult );
    }   

    cout << "\n---------------------------------------\n";
    bRslt = expTree.SetInfixExpression("StrCat3(:$ph1, :$ph2, :$ph3)");
    EXPECT_TRUE(bRslt);
    if (bRslt)
    {
        expTree.SetStringValueOfPlaceHolder(1, "A");
        expTree.SetStringValueOfPlaceHolder(2, "B");
        expTree.SetStringValueOfPlaceHolder(3, "C");        
        bRslt = expTree.EvaluateExpression();
        EXPECT_TRUE(bRslt);
        pExpressionRslt = expTree.GetResult();
        cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->strResult << "\n";
        EXPECT_STREQ("ABC", pExpressionRslt->strResult);
    }

    cout << "\n---------------------------------------\n";
    bRslt = expTree.SetInfixExpression(":$ph1 + :$ph2");
    EXPECT_TRUE(bRslt);
    if (bRslt)
    {
        expTree.SetNumberLongValueOfPlaceHolder(1, 10);
        expTree.SetNumberLongValueOfPlaceHolder(2, 20);
        bRslt = expTree.EvaluateExpression();
        EXPECT_TRUE(bRslt);
        pExpressionRslt = expTree.GetResult();
        cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->nResultLong << "\n";
        EXPECT_EQ(30, pExpressionRslt->nResultLong);

        expTree.SetNumberLongValueOfPlaceHolder(1, 1);
        expTree.SetNumberLongValueOfPlaceHolder(2, 2);
        bRslt = expTree.EvaluateExpression();
        EXPECT_TRUE(bRslt);
        pExpressionRslt = expTree.GetResult();
        cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->nResultLong << "\n";
        EXPECT_EQ(3, pExpressionRslt->nResultLong);
    }

    bRslt = expTree.SetInfixExpression(":$ph1 + :$ph2");
    EXPECT_TRUE(bRslt);
    if (bRslt)
    {
        expTree.SetNumberFloatValueOfPlaceHolder(1, 10.1f);
        expTree.SetNumberFloatValueOfPlaceHolder(2, 20.2f);
        bRslt = expTree.EvaluateExpression();
        EXPECT_TRUE(bRslt);
        pExpressionRslt = expTree.GetResult();
        cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->nResultFloat << "\n";
        EXPECT_FLOAT_EQ(30.3f, pExpressionRslt->nResultFloat);

        expTree.SetNumberFloatValueOfPlaceHolder ( 1, 20.1f );
        expTree.SetNumberFloatValueOfPlaceHolder ( 2, 20.2f );
        bRslt = expTree.EvaluateExpression ( );
        EXPECT_TRUE ( bRslt );
        pExpressionRslt = expTree.GetResult ( );
        cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->nResultFloat << "\n";
        EXPECT_FLOAT_EQ ( 40.3f, pExpressionRslt->nResultFloat );
    }

    cout << "\n---------------------------------------\n";
    bRslt = expTree.SetInfixExpression("SumInt(:$ph1, :$ph2)");
    EXPECT_TRUE(bRslt);
    if (bRslt)
    {
        expTree.SetNumberLongValueOfPlaceHolder(1, 10);
        expTree.SetNumberLongValueOfPlaceHolder(2, 20);
        bRslt = expTree.EvaluateExpression();
        EXPECT_TRUE(bRslt);
        pExpressionRslt = expTree.GetResult();
        cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->nResultLong << "\n";
        EXPECT_EQ(30, pExpressionRslt->nResultLong);
    }

    cout << "\n---------------------------------------\n";
    bRslt = expTree.SetInfixExpression("SumInt(:$ph1, SumInt(:$ph2, :$ph3))");
    EXPECT_TRUE(bRslt);
    if (bRslt)
    {
        expTree.SetNumberLongValueOfPlaceHolder(1, 10);
        expTree.SetNumberLongValueOfPlaceHolder(2, 20);
        expTree.SetNumberLongValueOfPlaceHolder(3, 30);
        bRslt = expTree.EvaluateExpression();
        EXPECT_TRUE(bRslt);
        pExpressionRslt = expTree.GetResult();
        cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->nResultLong << "\n";
        EXPECT_EQ(60, pExpressionRslt->nResultLong);

        expTree.SetNumberLongValueOfPlaceHolder ( 1, 10 );
        expTree.SetNumberLongValueOfPlaceHolder ( 2, 20 );
        expTree.SetNumberLongValueOfPlaceHolder ( 3, -30 );
        bRslt = expTree.EvaluateExpression ( );
        EXPECT_TRUE ( bRslt );
        pExpressionRslt = expTree.GetResult ( );
        cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->nResultLong << "\n";
        EXPECT_EQ ( 0, pExpressionRslt->nResultLong );
    }
    
}

///////////////////////////////////////////////////////////////////////////////
TEST(ExpressionTest, CHK)
{
    ExpressionTree expTree;
    bool bRslt = false;
    expression_result* pExpressionRslt;
   
/*
    cout << "\n---------------------------------------\n";
    bRslt = expTree.SetInfixExpression ( "'%678%'='12345678A'" );
    EXPECT_TRUE ( bRslt );
    bRslt = expTree.EvaluateExpression ( );
    EXPECT_TRUE ( bRslt );
    pExpressionRslt = expTree.GetResult ( );
    cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->bResult << "\n";
    EXPECT_TRUE ( pExpressionRslt->bResult );

    cout << "\n---------------------------------------\n";
    bRslt = expTree.SetInfixExpression ( "'12345678A'='%678%'" );
    EXPECT_TRUE ( bRslt );
    bRslt = expTree.EvaluateExpression ( );
    EXPECT_TRUE ( bRslt );
    pExpressionRslt = expTree.GetResult ( );
    cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->bResult << "\n";
    EXPECT_TRUE ( pExpressionRslt->bResult );
    
    cout << "\n---------------------------------------\n";
    bRslt = expTree.SetInfixExpression ( "'%678'='12345678'" );
    EXPECT_TRUE ( bRslt );
    bRslt = expTree.EvaluateExpression ( );
    EXPECT_TRUE ( bRslt );
    pExpressionRslt = expTree.GetResult ( );
    cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->bResult << "\n";
    EXPECT_TRUE ( pExpressionRslt->bResult );

    cout << "\n---------------------------------------\n";
    bRslt = expTree.SetInfixExpression ( "'12345678'='%678'" );
    EXPECT_TRUE ( bRslt );
    bRslt = expTree.EvaluateExpression ( );
    EXPECT_TRUE ( bRslt );
    pExpressionRslt = expTree.GetResult ( );
    cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->bResult << "\n";
    EXPECT_TRUE ( pExpressionRslt->bResult );
    */


    cout << "\n---------------------------------------\n";
    bRslt = expTree.SetInfixExpression ( "3=1+2 & 'AB' = 'A'+'B' " );
    EXPECT_TRUE ( bRslt );
    bRslt = expTree.EvaluateExpression ( );
    EXPECT_TRUE ( bRslt );


    cout << "\n---------------------------------------\n";
    bRslt = expTree.SetInfixExpression ( "'1' = '1'" );
    EXPECT_TRUE ( bRslt );
    bRslt = expTree.EvaluateExpression ( );
    EXPECT_TRUE ( bRslt );

    cout << "\n---------------------------------------\n";
    bRslt = expTree.SetInfixExpression ( "1+1" );
    EXPECT_TRUE ( bRslt );
    bRslt = expTree.EvaluateExpression ( );
    EXPECT_TRUE ( bRslt );
    pExpressionRslt = expTree.GetResult ( );
    cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->nResultLong << "\n";
    EXPECT_EQ ( 2, pExpressionRslt->nResultLong );

    cout << "\n---------------------------------------\n";
    bRslt = expTree.SetInfixExpression ( "1+1.0" );
    EXPECT_TRUE ( bRslt );
    bRslt = expTree.EvaluateExpression ( );
    EXPECT_TRUE ( bRslt );
    pExpressionRslt = expTree.GetResult ( );
    cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->nResultFloat << "\n";
    EXPECT_FLOAT_EQ ( 2.0f, pExpressionRslt->nResultFloat );

    cout << "\n---------------------------------------\n";
    bRslt = expTree.SetInfixExpression ( "1.0 + 1" );
    EXPECT_TRUE ( bRslt );
    bRslt = expTree.EvaluateExpression ( );
    EXPECT_TRUE ( bRslt );
    pExpressionRslt = expTree.GetResult ( );
    cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->nResultFloat << "\n";
    EXPECT_FLOAT_EQ ( 2.0f, pExpressionRslt->nResultFloat );

    cout << "\n---------------------------------------\n";
    bRslt = expTree.SetInfixExpression ( "1.0 + 1.0" );
    EXPECT_TRUE ( bRslt );
    bRslt = expTree.EvaluateExpression ( );
    EXPECT_TRUE ( bRslt );
    pExpressionRslt = expTree.GetResult ( );
    cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->nResultFloat << "\n";
    EXPECT_FLOAT_EQ ( 2.0f, pExpressionRslt->nResultFloat );


    cout << "\n---------------------------------------\n";
    bRslt = expTree.SetInfixExpression ( "2 <= 1" );
    EXPECT_TRUE ( bRslt );
    bRslt = expTree.EvaluateExpression ( );
    cout << "expTree.EvaluateExpression returns :" << bRslt << "\n";
    EXPECT_FALSE ( bRslt );

    cout << "\n---------------------------------------\n";
    bRslt = expTree.SetInfixExpression ( "1+1+1" );
    EXPECT_TRUE ( bRslt );
    bRslt = expTree.EvaluateExpression ( );
    EXPECT_TRUE ( bRslt );
    pExpressionRslt = expTree.GetResult ( );
    cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->nResultLong << "\n";
    EXPECT_EQ ( 3, pExpressionRslt->nResultLong );

    cout << "\n---------------------------------------\n";
    bRslt = expTree.SetInfixExpression("1+1");
    EXPECT_TRUE(bRslt);
    bRslt = expTree.EvaluateExpression();
    EXPECT_TRUE(bRslt);
    pExpressionRslt = expTree.GetResult();
    cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->nResultLong << "\n";
    EXPECT_EQ(2, pExpressionRslt->nResultLong);
       

    cout << "\n---------------------------------------\n";
    bRslt = expTree.SetInfixExpression("StrCat(':', '(' )");
    EXPECT_TRUE(bRslt);
    bRslt = expTree.EvaluateExpression();
    EXPECT_TRUE(bRslt);
    //EXPECT_EQ(2, expTree.GetDepth());
    pExpressionRslt = expTree.GetResult();
    cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->strResult << "\n";
    EXPECT_STREQ(":(", pExpressionRslt->strResult);
    
    cout << "\n---------------------------------------\n";
    bRslt = expTree.SetInfixExpression("StrCat('1',StrCat('a',StrCat('X','Y')) )");
    EXPECT_TRUE(bRslt);
    bRslt = expTree.EvaluateExpression();
    EXPECT_TRUE(bRslt);    
    pExpressionRslt = expTree.GetResult();
    cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->strResult << "\n";
    EXPECT_STREQ("1aXY", pExpressionRslt->strResult);

    cout << "\n---------------------------------------\n";
    bRslt = expTree.SetInfixExpression("StrCat3('1','2',StrCat3('a','b',StrCat3('X','Y','Z')) )");
    EXPECT_TRUE(bRslt);
    bRslt = expTree.EvaluateExpression();
    EXPECT_TRUE(bRslt);    
    pExpressionRslt = expTree.GetResult();
    cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->strResult << "\n";
    EXPECT_STREQ("12abXYZ", pExpressionRslt->strResult);

}

#define EPSILON 0.00001
bool float_compare(float a, float b)
{
    return fabs(a - b) < EPSILON; 
}

///////////////////////////////////////////////////////////////////////////////
TEST(ExpressionTest, FLOAT)
{
    ExpressionTree expTree;
    bool bRslt = false;
    expression_result* pExpressionRslt;

    cout << "\n---------------------------------------\n";
    bRslt = expTree.SetInfixExpression ( "2.0=2" );
    EXPECT_TRUE ( bRslt );
    bRslt = expTree.EvaluateExpression ( );
    EXPECT_TRUE ( bRslt );
    pExpressionRslt = expTree.GetResult ( );
    cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->bResult << "\n";
    EXPECT_TRUE ( pExpressionRslt->bResult );

    cout << "\n---------------------------------------\n";
    bRslt = expTree.SetInfixExpression("1.2+2");
    EXPECT_TRUE(bRslt);
    bRslt = expTree.EvaluateExpression();
    EXPECT_TRUE(bRslt);
    pExpressionRslt = expTree.GetResult();
    cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->nResultFloat << "\n";
    EXPECT_TRUE(float_compare(3.2f, pExpressionRslt->nResultFloat));
    //EXPECT_DOUBLE_EQ( (float) 3.2, (float) pExpressionRslt->nResultFloat);  

    cout << "\n---------------------------------------\n";
    bRslt = expTree.SetInfixExpression("1+2.2");
    EXPECT_TRUE(bRslt);
    bRslt = expTree.EvaluateExpression();
    EXPECT_TRUE(bRslt);
    pExpressionRslt = expTree.GetResult();
    cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->nResultFloat << "\n";
    EXPECT_TRUE(float_compare(3.2f, pExpressionRslt->nResultFloat));
    //EXPECT_DOUBLE_EQ(3.2, pExpressionRslt->nResultFloat);

    cout << "\n---------------------------------------\n";
    bRslt = expTree.SetInfixExpression("1.2+2.2");
    EXPECT_TRUE(bRslt);
    bRslt = expTree.EvaluateExpression();
    EXPECT_TRUE(bRslt);
    pExpressionRslt = expTree.GetResult();
    cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->nResultFloat << "\n";
    EXPECT_TRUE(float_compare(3.4f, pExpressionRslt->nResultFloat));
    //EXPECT_DOUBLE_EQ(3.4, pExpressionRslt->nResultFloat);

    cout << "\n---------------------------------------\n";
    bRslt = expTree.SetInfixExpression("1+2");
    EXPECT_TRUE(bRslt);
    bRslt = expTree.EvaluateExpression();
    EXPECT_TRUE(bRslt);
    pExpressionRslt = expTree.GetResult();
    cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->nResultLong << "\n";
    EXPECT_EQ(3, pExpressionRslt->nResultLong);

    /////////////

    cout << "\n---------------------------------------\n";
    bRslt = expTree.SetInfixExpression("2.2-2");
    EXPECT_TRUE(bRslt);
    bRslt = expTree.EvaluateExpression();
    EXPECT_TRUE(bRslt);
    pExpressionRslt = expTree.GetResult();
    cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->nResultFloat << "\n";
    EXPECT_TRUE(float_compare(0.2f, pExpressionRslt->nResultFloat));
    //EXPECT_DOUBLE_EQ(0.2, pExpressionRslt->nResultFloat);

    cout << "\n---------------------------------------\n";
    bRslt = expTree.SetInfixExpression("3-2.2");
    EXPECT_TRUE(bRslt);
    bRslt = expTree.EvaluateExpression();
    EXPECT_TRUE(bRslt);
    pExpressionRslt = expTree.GetResult();
    cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->nResultFloat << "\n";
    EXPECT_TRUE(float_compare(0.8f, pExpressionRslt->nResultFloat));
    //EXPECT_DOUBLE_EQ(0.8, pExpressionRslt->nResultFloat);

    cout << "\n---------------------------------------\n";
    bRslt = expTree.SetInfixExpression("3.2-2.1");
    EXPECT_TRUE(bRslt);
    bRslt = expTree.EvaluateExpression();
    EXPECT_TRUE(bRslt);
    pExpressionRslt = expTree.GetResult();
    cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->nResultFloat << "\n";
    EXPECT_TRUE(float_compare(1.1f, pExpressionRslt->nResultFloat));
    //EXPECT_DOUBLE_EQ(1.1, pExpressionRslt->nResultFloat);

    cout << "\n---------------------------------------\n";
    bRslt = expTree.SetInfixExpression("3-2");
    EXPECT_TRUE(bRslt);
    bRslt = expTree.EvaluateExpression();
    EXPECT_TRUE(bRslt);
    pExpressionRslt = expTree.GetResult();
    cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->nResultLong << "\n";
    EXPECT_EQ(1, pExpressionRslt->nResultLong);


}

///////////////////////////////////////////////////////////////////////////////
TEST(ExpressionTest, Functions)
{
    ExpressionTree expTree;
    bool bRslt = false;
    expression_result* pExpressionRslt;

    cout << "\n---------------------------------------\n";
    bRslt = expTree.SetInfixExpression(" StrCat3('A','B', StrCat3('C','D','E'))");
    EXPECT_TRUE(bRslt);
    bRslt = expTree.EvaluateExpression();
    EXPECT_TRUE(bRslt);
    pExpressionRslt = expTree.GetResult();
    cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->strResult << "\n";
    EXPECT_STREQ("ABCDE", pExpressionRslt->strResult);

    cout << "\n---------------------------------------\n";
    bRslt = expTree.SetInfixExpression("'1' + StrCat3('A','B', StrCat3('C','D','E')) = '1ABCDE'");
    //EXPECT_EQ(5, expTree.GetDepth());    
    EXPECT_TRUE(bRslt);
    bRslt = expTree.EvaluateExpression();
    EXPECT_TRUE(bRslt);

    //wrong expression check!!!
    cout << "\n---------------------------------------\n";
    bRslt = expTree.SetInfixExpression("StrCat3('1',StrCat3('2','3',StrCat3('4','5','6')))");
    EXPECT_FALSE(bRslt);


    cout << "\n---------------------------------------\n";
    bRslt = expTree.SetInfixExpression("StrCat3('1',StrCat3('2','3',StrCat3('4','5','6')),'7')");
    EXPECT_TRUE(bRslt);
    bRslt = expTree.EvaluateExpression();
    EXPECT_TRUE(bRslt);
    pExpressionRslt = expTree.GetResult();
    cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->strResult << "\n";
    EXPECT_STREQ("1234567", pExpressionRslt->strResult);

    cout << "\n---------------------------------------\n";
    bRslt = expTree.SetInfixExpression("StrCat3('1',StrCat3('2','3',StrCat3('4','5',StrCat3('6','7',StrCat3('8','9','10')))),'X')");
    EXPECT_TRUE(bRslt);
    bRslt = expTree.EvaluateExpression();
    EXPECT_TRUE(bRslt);
    pExpressionRslt = expTree.GetResult();
    cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->strResult << "\n";
    EXPECT_STREQ("12345678910X", pExpressionRslt->strResult);

    cout << "\n---------------------------------------\n";
    bRslt = expTree.SetInfixExpression("SumInt3(1,SumInt3(3,4,SumInt3(5,6,SumInt3(7,8,SumInt(9,10)))),2)");
    EXPECT_TRUE(bRslt);
    bRslt = expTree.EvaluateExpression();
    EXPECT_TRUE(bRslt);
    pExpressionRslt = expTree.GetResult();

    cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->nResultLong << "\n";
    EXPECT_EQ(55, pExpressionRslt->nResultLong);
    cout << "\n---------------------------------------\n";
    bRslt = expTree.SetInfixExpression("SumInt3(1,2,SumInt3(3,4,SumInt3(5,6,SumInt3(7,8,SumInt(9,10)))))");
    EXPECT_TRUE(bRslt);
    bRslt = expTree.EvaluateExpression();
    EXPECT_TRUE(bRslt);
    pExpressionRslt = expTree.GetResult();
    cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->nResultLong << "\n";
    EXPECT_EQ(55, pExpressionRslt->nResultLong);

    cout << "\n---------------------------------------\n";
    bRslt =
        expTree.SetInfixExpression("SumInt(1, SumInt(2, SumInt(3, SumInt(4, SumInt(5, SumInt(6, SumInt(7,SumInt(8,SumInt(9,10)))))))))");
    EXPECT_TRUE(bRslt);
    bRslt = expTree.EvaluateExpression();
    EXPECT_TRUE(bRslt);
    pExpressionRslt = expTree.GetResult();
    cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->nResultLong << "\n";
    EXPECT_EQ(55, pExpressionRslt->nResultLong);

    cout << "\n---------------------------------------\n";
    bRslt = expTree.SetInfixExpression("-1.0 / 2.0");
    EXPECT_TRUE(bRslt);
    bRslt = expTree.EvaluateExpression();
    EXPECT_TRUE(bRslt);
    pExpressionRslt = expTree.GetResult();
    cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->nResultFloat << "\n";
    EXPECT_EQ(-0.5, pExpressionRslt->nResultFloat);

    cout << "\n---------------------------------------\n";
    bRslt = expTree.SetInfixExpression("-1 + 2");
    EXPECT_TRUE(bRslt);
    bRslt = expTree.EvaluateExpression();
    EXPECT_TRUE(bRslt);
    pExpressionRslt = expTree.GetResult();
    cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->nResultLong << "\n";
    EXPECT_EQ(1, pExpressionRslt->nResultLong);

    cout << "\n---------------------------------------\n";
    bRslt = expTree.SetInfixExpression("1+ +2");
    EXPECT_TRUE(bRslt);
    bRslt = expTree.EvaluateExpression();
    EXPECT_TRUE(bRslt);
    pExpressionRslt = expTree.GetResult();
    cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->nResultLong << "\n";
    EXPECT_EQ(3, pExpressionRslt->nResultLong);

    cout << "\n---------------------------------------\n";
    bRslt = expTree.SetInfixExpression("StrLen( StrCat('AA', 'BB'))");
    EXPECT_TRUE(bRslt);
    bRslt = expTree.EvaluateExpression();
    EXPECT_TRUE(bRslt);
    pExpressionRslt = expTree.GetResult();
    cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->nResultLong << "\n";
    EXPECT_EQ(4, pExpressionRslt->nResultLong);

    cout << "\n---------------------------------------\n";
    bRslt = expTree.SetInfixExpression("StrCat3('1','2','3')");
    EXPECT_TRUE(bRslt);
    bRslt = expTree.EvaluateExpression();
    EXPECT_TRUE(bRslt);
    pExpressionRslt = expTree.GetResult();
    cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->strResult << "\n";
    EXPECT_STREQ("123", pExpressionRslt->strResult);

    cout << "\n---------------------------------------\n";
    bRslt = expTree.SetInfixExpression("1+2");
    EXPECT_TRUE(bRslt);
    bRslt = expTree.EvaluateExpression();
    EXPECT_TRUE(bRslt);
    pExpressionRslt = expTree.GetResult();
    cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->nResultLong << "\n";
    EXPECT_EQ(3, pExpressionRslt->nResultLong);


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
    cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->nResultLong << "\n";
    EXPECT_EQ(13, pExpressionRslt->nResultLong);

    cout << "\n---------------------------------------\n";
    bRslt = expTree.SetInfixExpression("SumInt4(1+1, 1+1, 1+1, 1+1)");
    EXPECT_TRUE(bRslt);
    bRslt = expTree.EvaluateExpression();
    EXPECT_TRUE(bRslt);
    pExpressionRslt = expTree.GetResult();
    cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->nResultLong << "\n";
    EXPECT_EQ(8, pExpressionRslt->nResultLong);

    cout << "\n---------------------------------------\n";
    bRslt = expTree.SetInfixExpression("StrCat3('1','2',StrCat3('a','b','c') )");
    EXPECT_TRUE(bRslt);
    bRslt = expTree.EvaluateExpression();
    EXPECT_TRUE(bRslt);
    pExpressionRslt = expTree.GetResult();
    cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->strResult << "\n";
    EXPECT_STREQ("12abc", pExpressionRslt->strResult);

    cout << "\n---------------------------------------\n";
    bRslt = expTree.SetInfixExpression("SumInt4(1+1, 1+1, 1+1, 1+1)");
    EXPECT_TRUE(bRslt);
    bRslt = expTree.EvaluateExpression();
    EXPECT_TRUE(bRslt);
    pExpressionRslt = expTree.GetResult();
    cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->nResultLong << "\n";
    EXPECT_EQ(8, pExpressionRslt->nResultLong);


    cout << "\n---------------------------------------\n";
    bRslt = expTree.SetInfixExpression("SumInt3(1+2, 3+4,5+6)");
    EXPECT_TRUE(bRslt);
    bRslt = expTree.EvaluateExpression();
    EXPECT_TRUE(bRslt);
    pExpressionRslt = expTree.GetResult();
    cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->nResultLong << "\n";
    EXPECT_EQ(21, pExpressionRslt->nResultLong);

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
    cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->strResult << "\n";
    EXPECT_STREQ("123", pExpressionRslt->strResult);

    cout << "\n---------------------------------------\n";
    bRslt = expTree.SetInfixExpression("SumInt(1+2, 2)");
    EXPECT_TRUE(bRslt);
    bRslt = expTree.EvaluateExpression();
    EXPECT_TRUE(bRslt);
    pExpressionRslt = expTree.GetResult();
    cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->nResultLong << "\n";
    EXPECT_EQ(5, pExpressionRslt->nResultLong);

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
    cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->nResultLong << "\n";
    EXPECT_EQ(100, pExpressionRslt->nResultLong);

    cout << "\n---------------------------------------\n";
    bRslt = expTree.SetInfixExpression("SumInt( SumInt( 1, 1), 2)");
    EXPECT_TRUE(bRslt);
    bRslt = expTree.EvaluateExpression();
    EXPECT_TRUE(bRslt);
    pExpressionRslt = expTree.GetResult();
    cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->nResultLong << "\n";
    EXPECT_EQ(4, pExpressionRslt->nResultLong);

    cout << "\n---------------------------------------\n";
    bRslt = expTree.SetInfixExpression("SumInt( 1, 2)");
    EXPECT_TRUE(bRslt);
    bRslt = expTree.EvaluateExpression();
    EXPECT_TRUE(bRslt);
    pExpressionRslt = expTree.GetResult();
    cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->nResultLong << "\n";
    EXPECT_EQ(3, pExpressionRslt->nResultLong);

    cout << "\n---------------------------------------\n";
    bRslt = expTree.SetInfixExpression("SumInt( (1), (2))");
    EXPECT_TRUE(bRslt);
    bRslt = expTree.EvaluateExpression();
    EXPECT_TRUE(bRslt);
    pExpressionRslt = expTree.GetResult();
    cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->nResultLong << "\n";
    EXPECT_EQ(3, pExpressionRslt->nResultLong);

    cout << "\n---------------------------------------\n";
    bRslt = expTree.SetInfixExpression("(( SumInt(0,1) ))");
    EXPECT_TRUE(bRslt);
    bRslt = expTree.EvaluateExpression();
    EXPECT_TRUE(bRslt);
    pExpressionRslt = expTree.GetResult();
    cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->nResultLong << "\n";
    EXPECT_EQ(1, pExpressionRslt->nResultLong);

    cout << "\n---------------------------------------\n";
    bRslt = expTree.SetInfixExpression("SumInt( (SumInt(0,1)),SumInt(1,1) )");
    EXPECT_TRUE(bRslt);
    bRslt = expTree.EvaluateExpression();
    EXPECT_TRUE(bRslt);
    pExpressionRslt = expTree.GetResult();
    cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->nResultLong << "\n";
    EXPECT_EQ(3, pExpressionRslt->nResultLong);

    cout << "\n---------------------------------------\n";
    bRslt = expTree.SetInfixExpression("StrCat('a','b')");
    EXPECT_TRUE(bRslt);
    bRslt = expTree.EvaluateExpression();
    EXPECT_TRUE(bRslt);
    pExpressionRslt = expTree.GetResult();
    cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->strResult << "\n";
    EXPECT_STREQ("ab", pExpressionRslt->strResult);

    cout << "\n---------------------------------------\n";
    bRslt = expTree.SetInfixExpression("StrCat('a','b')+StrCat('c','d')");
    EXPECT_TRUE(bRslt);
    bRslt = expTree.EvaluateExpression();
    EXPECT_TRUE(bRslt);
    pExpressionRslt = expTree.GetResult();
    cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->strResult << "\n";
    EXPECT_STREQ("abcd", pExpressionRslt->strResult);

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
        cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->nResultLong << "\n";
        EXPECT_EQ(123, pExpressionRslt->nResultLong);
    }

    cout << "\n---------------------------------------\n";
    bRslt = expTree.SetInfixExpression("AsciiToInt('123')+StrLen('abc') ");
    EXPECT_TRUE(bRslt);
    if (bRslt)
    {
        bRslt = expTree.EvaluateExpression();
        pExpressionRslt = expTree.GetResult();
        cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->nResultLong << "\n";
        EXPECT_EQ(126, pExpressionRslt->nResultLong);
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
        cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->nResultLong << "\n";
        EXPECT_EQ(5, pExpressionRslt->nResultLong);
    }


    cout << "\n---------------------------------------\n";
    bRslt = expTree.SetInfixExpression("StrLen( StrCat( StrCat('AA', 'BB'), 'XX' ) )+2");
    EXPECT_TRUE(bRslt);
    bRslt = expTree.EvaluateExpression();
    EXPECT_TRUE(bRslt);
    pExpressionRslt = expTree.GetResult();
    cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->nResultLong << "\n";
    EXPECT_EQ(8, pExpressionRslt->nResultLong);
    // Postfix is:   'AA' 'BB' StrCat StrLen 2 +



    cout << "\n---------------------------------------\n";
    bRslt = expTree.SetInfixExpression("StrLen( 'AA')");
    EXPECT_TRUE(bRslt);
    bRslt = expTree.EvaluateExpression();
    EXPECT_TRUE(bRslt);
    pExpressionRslt = expTree.GetResult();
    cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->nResultLong << "\n";
    EXPECT_EQ(2, pExpressionRslt->nResultLong);

    cout << "\n---------------------------------------\n";
    bRslt = expTree.SetInfixExpression("StrLen( StrCat('AA', 'BB'))");
    EXPECT_TRUE(bRslt);
    bRslt = expTree.EvaluateExpression();
    EXPECT_TRUE(bRslt);
    pExpressionRslt = expTree.GetResult();
    cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->nResultLong << "\n";
    EXPECT_EQ(4, pExpressionRslt->nResultLong);


    cout << "\n---------------------------------------\n";
    bRslt = expTree.SetInfixExpression("StrLen( StrCat('A', 'B')+StrCat('C', 'D'))");
    EXPECT_TRUE(bRslt);
    bRslt = expTree.EvaluateExpression();
    EXPECT_TRUE(bRslt);
    pExpressionRslt = expTree.GetResult();
    cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->nResultLong << "\n";
    EXPECT_EQ(4, pExpressionRslt->nResultLong);


    cout << "\n---------------------------------------\n";
    bRslt = expTree.SetInfixExpression("StrCat3('1','2', '3')");
    EXPECT_TRUE(bRslt);
    bRslt = expTree.EvaluateExpression();
    EXPECT_TRUE(bRslt);
    pExpressionRslt = expTree.GetResult();
    cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->strResult << "\n";
    EXPECT_STREQ("123", pExpressionRslt->strResult);

    cout << "\n---------------------------------------\n";
    bRslt = expTree.SetInfixExpression("StrCat4('1','2', '3','4')");
    EXPECT_TRUE(bRslt);
    bRslt = expTree.EvaluateExpression();
    EXPECT_TRUE(bRslt);
    pExpressionRslt = expTree.GetResult();
    cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->strResult << "\n";
    EXPECT_STREQ("1234", pExpressionRslt->strResult);

    cout << "\n---------------------------------------\n";
    bRslt = expTree.SetInfixExpression("StrCat('a','b')+StrCat4('1','2', '3','4')");
    EXPECT_TRUE(bRslt);
    bRslt = expTree.EvaluateExpression();
    EXPECT_TRUE(bRslt);
    pExpressionRslt = expTree.GetResult();
    cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->strResult << "\n";
    EXPECT_STREQ("ab1234", pExpressionRslt->strResult);

    cout << "\n---------------------------------------\n";
    bRslt = expTree.SetInfixExpression("GetInt100()");
    EXPECT_TRUE(bRslt);
    bRslt = expTree.EvaluateExpression();
    EXPECT_TRUE(bRslt);
    pExpressionRslt = expTree.GetResult();
    cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->nResultLong << "\n";
    EXPECT_EQ(100, pExpressionRslt->nResultLong);

    cout << "\n---------------------------------------\n";
    bRslt = expTree.SetInfixExpression("((1+2)*(4-2))+SumInt(1,2)");
    EXPECT_TRUE(bRslt);
    bRslt = expTree.EvaluateExpression();
    EXPECT_TRUE(bRslt);
    pExpressionRslt = expTree.GetResult();
    cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->nResultLong << "\n";
    EXPECT_EQ(9, pExpressionRslt->nResultLong);

    cout << "\n---------------------------------------\n";
    bRslt = expTree.SetInfixExpression("AsciiToInt( '1')");
    EXPECT_TRUE(bRslt);
    bRslt = expTree.EvaluateExpression();
    EXPECT_TRUE(bRslt);
    pExpressionRslt = expTree.GetResult();
    cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->nResultLong << "\n";
    EXPECT_EQ(1, pExpressionRslt->nResultLong);

    cout << "\n---------------------------------------\n";
    bRslt = expTree.SetInfixExpression("10 - AsciiToInt( StrCat('1','0')+'0')");
    EXPECT_TRUE(bRslt);
    bRslt = expTree.EvaluateExpression();
    EXPECT_TRUE(bRslt);
    pExpressionRslt = expTree.GetResult();
    cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->nResultLong << "\n";
    EXPECT_EQ(-90, pExpressionRslt->nResultLong);


    cout << "\n---------------------------------------\n";
    bRslt = expTree.SetInfixExpression("StrLen('abc')");
    EXPECT_TRUE(bRslt);
    bRslt = expTree.EvaluateExpression();
    EXPECT_TRUE(bRslt);
    pExpressionRslt = expTree.GetResult();
    cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->nResultLong << "\n";
    EXPECT_EQ(3, pExpressionRslt->nResultLong);

    cout << "\n---------------------------------------\n";
    bRslt = expTree.SetInfixExpression("StrLen( 'abs')");
    EXPECT_TRUE(bRslt);
    bRslt = expTree.EvaluateExpression();
    EXPECT_TRUE(bRslt);
    pExpressionRslt = expTree.GetResult();
    cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->nResultLong << "\n";
    EXPECT_EQ(3, pExpressionRslt->nResultLong);

    cout << "\n---------------------------------------\n";
    bRslt = expTree.SetInfixExpression("StrLen( 'abs 2' )");
    EXPECT_TRUE(bRslt);
    bRslt = expTree.EvaluateExpression();
    EXPECT_TRUE(bRslt);
    pExpressionRslt = expTree.GetResult();
    cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->nResultLong << "\n";
    EXPECT_EQ(5, pExpressionRslt->nResultLong);


    cout << "\n---------------------------------------\n";
    bRslt = expTree.SetInfixExpression("StrLen( 'abs 2' )+StrLen( 'aaa' )");
    EXPECT_TRUE(bRslt);
    bRslt = expTree.EvaluateExpression();
    EXPECT_TRUE(bRslt);
    pExpressionRslt = expTree.GetResult();
    cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->nResultLong << "\n";
    EXPECT_EQ(8, pExpressionRslt->nResultLong);

    cout << "\n---------------------------------------\n";
    bRslt = expTree.SetInfixExpression("StrLen('a')+StrLen('b')+StrLen('c')+StrLen('d')");
    EXPECT_TRUE(bRslt);
    bRslt = expTree.EvaluateExpression();
    EXPECT_TRUE(bRslt);
    pExpressionRslt = expTree.GetResult();
    cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->nResultLong << "\n";
    EXPECT_EQ(4, pExpressionRslt->nResultLong);

    cout << "\n---------------------------------------\n";
    bRslt = expTree.SetInfixExpression("StrLen('aa')+StrLen('bb')*StrLen('ccc')");
    EXPECT_TRUE(bRslt);
    bRslt = expTree.EvaluateExpression();
    EXPECT_TRUE(bRslt);
    pExpressionRslt = expTree.GetResult();
    cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->nResultLong << "\n";
    EXPECT_EQ(8, pExpressionRslt->nResultLong);



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
    cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->nResultLong << "\n";
    EXPECT_EQ(7, pExpressionRslt->nResultLong);

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
    cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->nResultLong << "\n";
    EXPECT_EQ(6, pExpressionRslt->nResultLong);

    cout << "\n---------------------------------------\n";
    bRslt = expTree.SetInfixExpression("SumInt( 1, StrLen( StrCat('A', 'B')) )");
    EXPECT_TRUE(bRslt);
    bRslt = expTree.EvaluateExpression();
    EXPECT_TRUE(bRslt);
    pExpressionRslt = expTree.GetResult();
    cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->nResultLong << "\n";
    EXPECT_EQ(3, pExpressionRslt->nResultLong);

    cout << "\n---------------------------------------\n";
    bRslt = expTree.SetInfixExpression("SumInt( 1, SumInt( 1, StrLen( StrCat('A', 'B')) ))");
    EXPECT_TRUE(bRslt);
    bRslt = expTree.EvaluateExpression();
    EXPECT_TRUE(bRslt);
    pExpressionRslt = expTree.GetResult();
    cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->nResultLong << "\n";
    EXPECT_EQ(4, pExpressionRslt->nResultLong);

    cout << "\n---------------------------------------\n";
    bRslt = expTree.SetInfixExpression("100 - SumInt( 1, SumInt( 1, StrLen( StrCat('A', 'B')) ))");
    EXPECT_TRUE(bRslt);
    bRslt = expTree.EvaluateExpression();
    EXPECT_TRUE(bRslt);
    pExpressionRslt = expTree.GetResult();
    cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->nResultLong << "\n";
    EXPECT_EQ(96, pExpressionRslt->nResultLong);

    cout << "\n---------------------------------------\n";
    bRslt = expTree.SetInfixExpression("100 - AsciiToInt( '100')");
    EXPECT_TRUE(bRslt);
    bRslt = expTree.EvaluateExpression();
    EXPECT_TRUE(bRslt);
    pExpressionRslt = expTree.GetResult();
    cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->nResultLong << "\n";
    EXPECT_EQ(0, pExpressionRslt->nResultLong);

    cout << "\n---------------------------------------\n";
    bRslt = expTree.SetInfixExpression("GetStrSum3(10,'20',30)");
    EXPECT_TRUE(bRslt);
    bRslt = expTree.EvaluateExpression();
    EXPECT_TRUE(bRslt);
    pExpressionRslt = expTree.GetResult();
    cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->strResult << "\n";
    EXPECT_STREQ("60", pExpressionRslt->strResult);

    cout << "\n---------------------------------------\n";
    bRslt = expTree.SetInfixExpression("GetStrSum(10,'20')");
    EXPECT_TRUE(bRslt);
    bRslt = expTree.EvaluateExpression();
    EXPECT_TRUE(bRslt);
    pExpressionRslt = expTree.GetResult();
    cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->strResult << "\n";
    EXPECT_STREQ("30", pExpressionRslt->strResult);


    cout << "\n---------------------------------------\n";
    bRslt = expTree.SetInfixExpression("GetStrSumAndPlus100(10,20)");
    EXPECT_TRUE(bRslt);
    bRslt = expTree.EvaluateExpression();
    EXPECT_TRUE(bRslt);
    pExpressionRslt = expTree.GetResult();
    cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->strResult << "\n";
    EXPECT_STREQ("130", pExpressionRslt->strResult);

    cout << "\n---------------------------------------\n";
    bRslt = expTree.SetInfixExpression("GetStrPlus100(10)");
    EXPECT_TRUE(bRslt);
    bRslt = expTree.EvaluateExpression();
    EXPECT_TRUE(bRslt);
    pExpressionRslt = expTree.GetResult();
    cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->strResult << "\n";

}


///////////////////////////////////////////////////////////////////////////////
TEST(ExpressionTest, Literal)
{
    ExpressionTree expTree;
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
    ExpressionTree expTree;
    expression_result* pExpressionRslt;
    bool bRslt = false;

    cout << "\n---------------------------------------\n";
    bRslt = expTree.SetInfixExpression("100 + GetInt100()");
    EXPECT_TRUE(bRslt);
    bRslt = expTree.EvaluateExpression();
    EXPECT_TRUE(bRslt);
    pExpressionRslt = expTree.GetResult();
    cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->nResultLong << "\n";
    EXPECT_EQ(200, pExpressionRslt->nResultLong);

    // 54 - (5 - 9) =

    //(1+2)*3-(4-5)*6/7
    cout << "\n---------------------------------------\n";
    bRslt = expTree.SetInfixExpression("(1+2)*3-(4-5)*7/7");
    EXPECT_TRUE(bRslt);
    bRslt = expTree.EvaluateExpression();
    EXPECT_TRUE(bRslt);
    pExpressionRslt = expTree.GetResult();
    cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->nResultLong << "\n";
    EXPECT_EQ(10, pExpressionRslt->nResultLong);

    cout << "\n---------------------------------------\n";
    bRslt = expTree.SetInfixExpression("54 - 5 - 9");
    EXPECT_TRUE(bRslt);
    bRslt = expTree.EvaluateExpression();
    EXPECT_TRUE(bRslt);
    pExpressionRslt = expTree.GetResult();
    cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->nResultLong << "\n";
    EXPECT_EQ(40, pExpressionRslt->nResultLong);
    
    
    cout << "\n---------------------------------------\n";
    bRslt = expTree.SetInfixExpression("2+1*3/3-1");
    EXPECT_TRUE(bRslt);
    bRslt = expTree.EvaluateExpression();
    EXPECT_TRUE(bRslt);
    pExpressionRslt = expTree.GetResult();
    cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->nResultLong << "\n";
    EXPECT_EQ(2, pExpressionRslt->nResultLong);

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
    cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->nResultLong << "\n";
    EXPECT_EQ(2, pExpressionRslt->nResultLong);

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
    bRslt = expTree.SetInfixExpression("1+2+3");
    EXPECT_TRUE(bRslt);
    bRslt = expTree.EvaluateExpression();
    EXPECT_TRUE(bRslt);
    pExpressionRslt = expTree.GetResult();
    cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->nResultLong << "\n";
    EXPECT_EQ(6, pExpressionRslt->nResultLong);


    cout << "\n---------------------------------------\n";
    bRslt = expTree.SetInfixExpression("1+2*3");
    EXPECT_TRUE(bRslt);
    bRslt = expTree.EvaluateExpression();
    EXPECT_TRUE(bRslt);
    pExpressionRslt = expTree.GetResult();
    cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->nResultLong << "\n";
    EXPECT_EQ(7, pExpressionRslt->nResultLong);

    cout << "\n---------------------------------------\n";
    bRslt = expTree.SetInfixExpression("(1+2)*3");
    EXPECT_TRUE(bRslt);
    bRslt = expTree.EvaluateExpression();
    EXPECT_TRUE(bRslt);
    pExpressionRslt = expTree.GetResult();
    cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->nResultLong << "\n";
    EXPECT_EQ(9, pExpressionRslt->nResultLong);

    cout << "\n---------------------------------------\n";
    bRslt = expTree.SetInfixExpression("1*2+3");
    EXPECT_TRUE(bRslt);
    bRslt = expTree.EvaluateExpression();
    EXPECT_TRUE(bRslt);
    pExpressionRslt = expTree.GetResult();
    cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->nResultLong << "\n";
    EXPECT_EQ(5, pExpressionRslt->nResultLong);


    cout << "\n---------------------------------------\n";
    bRslt = expTree.SetInfixExpression("1+4/2");
    EXPECT_TRUE(bRslt);
    bRslt = expTree.EvaluateExpression();
    EXPECT_TRUE(bRslt);
    pExpressionRslt = expTree.GetResult();
    cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->nResultLong << "\n";
    EXPECT_EQ(3, pExpressionRslt->nResultLong);

    cout << "\n---------------------------------------\n";
    bRslt = expTree.SetInfixExpression("(1+(2-3))*4");
    EXPECT_TRUE(bRslt);
    bRslt = expTree.EvaluateExpression();
    EXPECT_TRUE(bRslt);
    pExpressionRslt = expTree.GetResult();
    cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->nResultLong << "\n";
    EXPECT_EQ(0, pExpressionRslt->nResultLong);

    cout << "\n---------------------------------------\n";
    bRslt = expTree.SetInfixExpression("10+20+30");
    EXPECT_TRUE(bRslt);
    bRslt = expTree.EvaluateExpression();
    EXPECT_TRUE(bRslt);
    pExpressionRslt = expTree.GetResult();
    cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->nResultLong << "\n";
    EXPECT_EQ(60, pExpressionRslt->nResultLong);

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
    cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->nResultLong << "\n";
    EXPECT_EQ(100, pExpressionRslt->nResultLong);


    cout << "\n---------------------------------------\n";
    bRslt = expTree.SetInfixExpression("SumInt(StrLen('a'), GetInt100())");
    EXPECT_TRUE(bRslt);
    bRslt = expTree.EvaluateExpression();
    EXPECT_TRUE(bRslt);
    pExpressionRslt = expTree.GetResult();
    cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->nResultLong << "\n";
    EXPECT_EQ(101, pExpressionRslt->nResultLong);
    //EXPECT_GT(2, 0);

    cout << "\n---------------------------------------\n";
    bRslt = expTree.SetInfixExpression("1+2+3+4+5+6+7+8+9+10+11+12+13+14+15+16+17+18+19+20");
    EXPECT_TRUE(bRslt);
    bRslt = expTree.EvaluateExpression();
    EXPECT_TRUE(bRslt);
    pExpressionRslt = expTree.GetResult();
    cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->nResultLong << "\n";
    EXPECT_EQ(210, pExpressionRslt->nResultLong);

}

///////////////////////////////////////////////////////////////////////////////
TEST(ExpressionTest, negative_positive)
{
    ExpressionTree expTree;
    expression_result* pExpressionRslt;
    bool bRslt = false;

    cout << "\n---------------------------------------\n";
    bRslt = expTree.SetInfixExpression("1 * -2 ");
    EXPECT_TRUE(bRslt);
    if (bRslt)
    {
        bRslt = expTree.EvaluateExpression();
        EXPECT_TRUE(bRslt);
        pExpressionRslt = expTree.GetResult();
        cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->nResultLong << "\n";
        EXPECT_EQ(-2, pExpressionRslt->nResultLong);
    }

    cout << "\n---------------------------------------\n";
    bRslt = expTree.SetInfixExpression("1 * (-2) ");
    EXPECT_TRUE(bRslt);
    if (bRslt)
    {
        bRslt = expTree.EvaluateExpression();
        EXPECT_TRUE(bRslt);
        pExpressionRslt = expTree.GetResult();
        cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->nResultLong << "\n";
        EXPECT_EQ(-2, pExpressionRslt->nResultLong);
    }

    cout << "\n---------------------------------------\n";
    bRslt = expTree.SetInfixExpression("1*-2 ");
    EXPECT_TRUE(bRslt);
    if (bRslt)
    {
        bRslt = expTree.EvaluateExpression();
        EXPECT_TRUE(bRslt);
        pExpressionRslt = expTree.GetResult();
        cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->nResultLong << "\n";
        EXPECT_EQ(-2, pExpressionRslt->nResultLong);
    }

    cout << "\n---------------------------------------\n";
    bRslt = expTree.SetInfixExpression("1 - -1 ");
    EXPECT_TRUE(bRslt);
    if (bRslt)
    {
        bRslt = expTree.EvaluateExpression();
        EXPECT_TRUE(bRslt);
        pExpressionRslt = expTree.GetResult();
        cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->nResultLong << "\n";
        EXPECT_EQ(2, pExpressionRslt->nResultLong);
    }

    cout << "\n---------------------------------------\n";
    bRslt = expTree.SetInfixExpression("1 - (-1) ");
    EXPECT_TRUE(bRslt);
    if (bRslt)
    {
        bRslt = expTree.EvaluateExpression();
        EXPECT_TRUE(bRslt);
        pExpressionRslt = expTree.GetResult();
        cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->nResultLong << "\n";
        EXPECT_EQ(2, pExpressionRslt->nResultLong);
    }

    cout << "\n---------------------------------------\n";
    bRslt = expTree.SetInfixExpression("1 - -1 + 2 * -1");
    EXPECT_TRUE(bRslt);
    if (bRslt)
    {
        bRslt = expTree.EvaluateExpression();
        EXPECT_TRUE(bRslt);
        pExpressionRslt = expTree.GetResult();
        cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->nResultLong << "\n";
        EXPECT_EQ(0, pExpressionRslt->nResultLong);
    }

}

///////////////////////////////////////////////////////////////////////////////
TEST(ExpressionTest, AND_OR)
{
    ExpressionTree expTree;
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

///////////////////////////////////////////////////////////////////////////////
TEST(ExpressionTest, TEST_RULE)
{
    ExpressionTree expTree;
    bool bRslt = false;
    expression_result * pExpressionRslt;

    cout << "\n---------------------------------------\n";
    bRslt = expTree.SetInfixExpression("(:$ph1 = '01071786907' | :$ph2 = '1071786907') & :$ph3 = '127.0.0.1' & SumInt(:$ph4, :$ph5) != 100");
    EXPECT_TRUE(bRslt);

    if ( bRslt )
    {
        expTree.SetStringValueOfPlaceHolder ( 1, "'01071786907'" );//XXX TODO remove ' '
        expTree.SetStringValueOfPlaceHolder ( 2, "'1071786907X'" );
        expTree.SetStringValueOfPlaceHolder ( 3, "'127.0.0.1'" );
        expTree.SetNumberLongValueOfPlaceHolder ( 4, 79 );
        expTree.SetNumberLongValueOfPlaceHolder ( 5, 20 );

        bRslt = expTree.EvaluateExpression ( );
        EXPECT_TRUE ( bRslt );
        pExpressionRslt = expTree.GetResult ( );
        cout << "expTree.EvaluateExpression returns :" << pExpressionRslt->strResult << "\n";
    }   
}

