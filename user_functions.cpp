#include "user_functions.h"
#include "expression_tree.h"
#include <stdarg.h>

#include<iostream>
using namespace std;

//--------------------------------------------------------------------------
CUserFunctions::CUserFunctions()
{
    memset(&szReturn, 0x00, sizeof(szReturn));
    Initialize();
}

//--------------------------------------------------------------------------
CUserFunctions::~CUserFunctions()
{

}

void CUserFunctions::InsertMap(int nRtnType, int nRtnTypeDetailed, const char* strFuncName, int nArgCnt, ...)
{
    SFuncDefInfo funcDef;    
    strcpy(funcDef.strFuncName, strFuncName );
    funcDef.nReturnType = nRtnType;
    funcDef.nDetailedReturnType = nRtnTypeDetailed;
    funcDef.ntotalInputArgCnt = nArgCnt;
    
    va_list arguments;            
    va_start(arguments, nArgCnt);
    for (int x = 0; x < nArgCnt; x++)
    {
        int nVar = va_arg(arguments, int);
        funcDef.nFuncArgsTypes[x] = nVar;
    }
    va_end(arguments);  
    mapFuncDefinitions.insert(VT_MAP_USER_FUNC(string(strFuncName), funcDef));
}

bool CUserFunctions::Initialize()
{
    //TODO set map (from db)
    InsertMap(FUNC_RTN_NUM, FUNC_RTN_DETAILED_LONG, "SumInt", 2, FUNC_ARG_NUM, FUNC_ARG_NUM);
    InsertMap(FUNC_RTN_NUM, FUNC_RTN_DETAILED_LONG, "SumInt3", 3, FUNC_ARG_NUM, FUNC_ARG_NUM, FUNC_ARG_NUM);
    InsertMap(FUNC_RTN_NUM, FUNC_RTN_DETAILED_LONG, "SumInt4", 4, FUNC_ARG_NUM, FUNC_ARG_NUM, FUNC_ARG_NUM, FUNC_ARG_NUM);
    InsertMap(FUNC_RTN_NUM, FUNC_RTN_DETAILED_LONG, "StrLen", 1, FUNC_ARG_STR);
    InsertMap(FUNC_RTN_NUM, FUNC_RTN_DETAILED_LONG, "AsciiToInt", 1, FUNC_ARG_STR);
    InsertMap(FUNC_RTN_STR, FUNC_RTN_DETAILED_STR,"StrCat", 2, FUNC_ARG_STR, FUNC_ARG_STR);

    InsertMap(FUNC_RTN_STR, FUNC_RTN_DETAILED_STR,"StrCat3", 3, FUNC_ARG_STR, FUNC_ARG_STR, FUNC_ARG_STR); //test
    InsertMap(FUNC_RTN_STR, FUNC_RTN_DETAILED_STR,"StrCat4", 4, FUNC_ARG_STR, FUNC_ARG_STR, FUNC_ARG_STR, FUNC_ARG_STR);

    InsertMap(FUNC_RTN_NUM, FUNC_RTN_DETAILED_LONG, "GetInt100", 0, FUNC_ARG_NONE);
    InsertMap(FUNC_RTN_STR, FUNC_RTN_DETAILED_STR,"GetStrPlus100", 1, FUNC_ARG_NUM);
    InsertMap(FUNC_RTN_STR, FUNC_RTN_DETAILED_STR,"GetStrSumAndPlus100", 2, FUNC_ARG_NUM, FUNC_ARG_NUM);
    InsertMap(FUNC_RTN_STR, FUNC_RTN_DETAILED_STR, "GetStrSum", 2, FUNC_ARG_NUM, FUNC_ARG_STR);
    InsertMap(FUNC_RTN_STR, FUNC_RTN_DETAILED_STR, "GetStrSum3", 3, FUNC_ARG_NUM, FUNC_ARG_STR, FUNC_ARG_NUM);
    InsertMap(FUNC_RTN_BOOL, FUNC_RTN_DETAILED_BOOL, "GetBool", 1, FUNC_ARG_BOOL);
    InsertMap(FUNC_RTN_BOOL, FUNC_RTN_DETAILED_BOOL,"testBool", 3, FUNC_ARG_NUM, FUNC_ARG_STR, FUNC_ARG_BOOL);
    InsertMap(FUNC_RTN_BOOL, FUNC_RTN_DETAILED_BOOL,"testBool2", 2, FUNC_ARG_NUM, FUNC_ARG_BOOL);
    
    
        
    return true;
}


bool CUserFunctions::IsThisUserFunction(const char* strFuncName, IT_MAP_USER_FUNC& itFound)
{
    itFound = mapFuncDefinitions.find(strFuncName);
    if (itFound != mapFuncDefinitions.end())
    {
        return true;
    }
        
    return false;
}

//remove single quotations both sides
bool CUserFunctions::GetStringSingleQuotationsBothSidesRemoved(char* strExpression)
{    
    char *quotPtr = strchr(strExpression, '\'');
    if (quotPtr == NULL)
    {
        //cout << "no single quot :" << strExpression << endl;
        return false;
    }

    int position = quotPtr - strExpression;
    char attrValue[1024];
    memset(&attrValue, 0x00, sizeof(attrValue));
    strncpy(attrValue, strExpression + position + 1, strlen(strExpression) - position);

    quotPtr = strchr(attrValue, '\'');
    if (quotPtr == NULL)
    {
        //cout << "no single quot :" << strExpression << endl;
        return false;
    }    
    position = quotPtr - attrValue;    
    memset(strExpression, 0x00, sizeof(attrValue));
    strncpy(strExpression, attrValue, position);

    return true;
}


///////////////////////////////////////////////////////////////////////////////
bool CUserFunctions::InvokeUserFunction(expression_node* pRoot, 
                    expression_node* pFuncArgLeft, //(array)첫번째 함수인자 
                    expression_node* pFuncArgRight //(array)두번째부터                     
                    )
{
    //cout << "InvokeUserFunction " << endl;
    if (strcmp(pRoot->userFuncInfo.strFuncName, "StrCat") == 0) //2 arg
    {
        strcpy(pRoot->strVal, StrCat(pFuncArgLeft->strVal, pFuncArgRight->strVal));
    }
    else if (strcmp(pRoot->userFuncInfo.strFuncName, "StrCat3") == 0) //3 arg
    {        
        strcpy(pRoot->strVal, StrCat3(pFuncArgLeft->strVal,
            pFuncArgRight->strVal,
            pFuncArgRight->nextForMore2funcArgs->strVal));
    }
    else if (strcmp(pRoot->userFuncInfo.strFuncName, "StrCat4") == 0) //4 arg
    {                        
        strcpy(pRoot->strVal, StrCat4(
                            pFuncArgLeft->strVal,
                            pFuncArgRight->strVal,
                            pFuncArgRight->nextForMore2funcArgs->strVal,
                            pFuncArgRight->nextForMore2funcArgs->nextForMore2funcArgs->strVal 
                            ) 
              );
    }
    else if (strcmp(pRoot->userFuncInfo.strFuncName, "StrLen") == 0)
    {        
        pRoot->variable.nLongValue = StrLen(pFuncArgLeft->strVal);
    }

    else if (strcmp(pRoot->userFuncInfo.strFuncName, "SumInt") == 0)
    {
        pRoot->variable.nLongValue = SumInt(pFuncArgLeft->variable.nLongValue,
                                pFuncArgRight->variable.nLongValue);
    }
    else if (strcmp(pRoot->userFuncInfo.strFuncName, "SumInt3") == 0)
    {        
        pRoot->variable.nLongValue = SumInt3(
            pFuncArgLeft->variable.nLongValue, 
            pFuncArgRight->variable.nLongValue, 
            pFuncArgRight->nextForMore2funcArgs->variable.nLongValue);
    }
    else if (strcmp(pRoot->userFuncInfo.strFuncName, "SumInt4") == 0)
    {        
        pRoot->variable.nLongValue = SumInt4(pFuncArgLeft->variable.nLongValue,
            pFuncArgRight->variable.nLongValue,
            pFuncArgRight->nextForMore2funcArgs->variable.nLongValue,
            pFuncArgRight->nextForMore2funcArgs->nextForMore2funcArgs->variable.nLongValue
            );
    }
    else if (strcmp(pRoot->userFuncInfo.strFuncName, "AsciiToInt") == 0)
    {
        pRoot->variable.nLongValue = AsciiToInt(pFuncArgLeft->strVal);
    }
    else if (strcmp(pRoot->userFuncInfo.strFuncName, "GetInt100") == 0)
    {
        pRoot->variable.nLongValue = GetInt100();
    }
    else if (strcmp(pRoot->userFuncInfo.strFuncName, "GetStrPlus100") == 0)
    {
        strcpy(pRoot->strVal, GetStrPlus100(pFuncArgLeft->variable.nLongValue));
    }
    else if (strcmp(pRoot->userFuncInfo.strFuncName, "GetStrSumAndPlus100") == 0)
    {
        strcpy(pRoot->strVal, GetStrSumAndPlus100(pFuncArgLeft->variable.nLongValue,
            pFuncArgRight->variable.nLongValue));
    }
    else if (strcmp(pRoot->userFuncInfo.strFuncName, "GetStrSum") == 0)
    {
        strcpy(pRoot->strVal, GetStrSum(pFuncArgLeft->variable.nLongValue,
            pFuncArgRight->strVal));
    }
    else if (strcmp(pRoot->userFuncInfo.strFuncName, "GetStrSum3") == 0)
    {        
        strcpy(pRoot->strVal, GetStrSum3(pFuncArgLeft->variable.nLongValue,
            pFuncArgRight->strVal, pFuncArgRight->nextForMore2funcArgs->variable.nLongValue ));
    }
    else if (strcmp(pRoot->userFuncInfo.strFuncName, "GetBool") == 0)
    {        
        pRoot->opReslut = GetBool(pFuncArgLeft->opReslut);
    }
    else if (strcmp(pRoot->userFuncInfo.strFuncName, "testBool") == 0)
    {        
        pRoot->opReslut = testBool(pFuncArgLeft->variable.nLongValue, pFuncArgRight->strVal, pFuncArgRight->nextForMore2funcArgs->opReslut);
    }
    else if (strcmp(pRoot->userFuncInfo.strFuncName, "testBool2") == 0)
    {                
        pRoot->opReslut = testBool2(pFuncArgLeft->variable.nLongValue, pFuncArgRight->opReslut);
    }

    return true;
}

///////////////////////////////////////////////////////////////////////////////
char*   CUserFunctions::GetStrSum3(long nVal1, char* szVal2, long nVal3)
{
    memset(&szReturn, 0x00, sizeof(szReturn));
    sprintf(szReturn, "%ld", nVal1 + atol(szVal2) + nVal3 );

    return szReturn;
}

///////////////////////////////////////////////////////////////////////////////
char*   CUserFunctions::GetStrSum(long nVal1, char* szVal2)
{
    memset(&szReturn, 0x00, sizeof(szReturn));
    sprintf(szReturn, "%ld", nVal1 + atol(szVal2) );

    return szReturn;
}

///////////////////////////////////////////////////////////////////////////////
char*  CUserFunctions::GetStrSumAndPlus100(long nVal1, long nVal2)
{
    memset(&szReturn, 0x00, sizeof(szReturn));
    sprintf(szReturn, "%ld", nVal1 + nVal2 + 100);

    return szReturn;
}

///////////////////////////////////////////////////////////////////////////////
char*  CUserFunctions::GetStrPlus100(long nCnt)
{
    memset(&szReturn, 0x00, sizeof(szReturn));
    sprintf(szReturn, "%ld", nCnt + 100);
    
    return szReturn;
}

///////////////////////////////////////////////////////////////////////////////
bool CUserFunctions::testBool(int nVal1, char* szVal2, bool boolFlag)
{
    if (boolFlag)
    {
        return true;
    }
    else
    {
        return false;
    }    
}

///////////////////////////////////////////////////////////////////////////////
bool CUserFunctions::testBool2(int nVal1,  bool boolFlag)
{
    if (boolFlag)
    {
        return true;
    }
    else
    {
        return false;
    }
}
///////////////////////////////////////////////////////////////////////////////
int CUserFunctions::StrLen(const char* str)
{
    return strlen(str);
}
///////////////////////////////////////////////////////////////////////////////
int CUserFunctions::SumInt(int n1, int n2)
{
    return n1 + n2;
}

///////////////////////////////////////////////////////////////////////////////
int CUserFunctions::SumInt3(int n1, int n2, int n3)
{
    return n1 + n2 + n3;
}
///////////////////////////////////////////////////////////////////////////////
int CUserFunctions::SumInt4(int n1, int n2, int n3, int n4)
{
    return n1 + n2 + n3 + n4;
}


///////////////////////////////////////////////////////////////////////////////
char* CUserFunctions::StrCat( char* str1,  char* str2)
{
    return strcat(str1,str2);
}

///////////////////////////////////////////////////////////////////////////////
char* CUserFunctions::StrCat3(char* str1, char* str2, char* str3)
{   
    strcat(str1, str2);
    strcat(str1, str3);
    return str1;
}

char* CUserFunctions::StrCat4(char* str1, char* str2, char* str3, char* str4)
{    
    strcat(str1, str2);
    strcat(str1, str3);
    strcat(str1, str4);
    return str1;
}

///////////////////////////////////////////////////////////////////////////////
long CUserFunctions::AsciiToInt(const char* str)
{
    return atol(str);
}
