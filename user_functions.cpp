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
        strcpy ( pRoot->expressionResult.strResult, StrCat ( pFuncArgLeft->expressionResult.strResult, 
                                                             pFuncArgRight->expressionResult.strResult ) );
    }
    else if (strcmp(pRoot->userFuncInfo.strFuncName, "StrCat3") == 0) //3 arg
    {        
        strcpy ( pRoot->expressionResult.strResult, StrCat3 ( pFuncArgLeft->expressionResult.strResult,
            pFuncArgRight->expressionResult.strResult,
            pFuncArgRight->rightSiblingForMore2funcArgs->expressionResult.strResult ) );
    }
    else if (strcmp(pRoot->userFuncInfo.strFuncName, "StrCat4") == 0) //4 arg
    {                        
        strcpy(pRoot->expressionResult.strResult, StrCat4(
            pFuncArgLeft->expressionResult.strResult,
            pFuncArgRight->expressionResult.strResult,
            pFuncArgRight->rightSiblingForMore2funcArgs->expressionResult.strResult,
            pFuncArgRight->rightSiblingForMore2funcArgs->rightSiblingForMore2funcArgs->expressionResult.strResult
                            ) 
              );
    }
    else if (strcmp(pRoot->userFuncInfo.strFuncName, "StrLen") == 0)
    {        
        pRoot->expressionResult.nResultLong = StrLen(pFuncArgLeft->expressionResult.strResult);
    }

    else if (strcmp(pRoot->userFuncInfo.strFuncName, "SumInt") == 0)
    {
        pRoot->expressionResult.nResultLong = SumInt(pFuncArgLeft->expressionResult.nResultLong,
                                                      pFuncArgRight->expressionResult.nResultLong );
    }
    else if (strcmp(pRoot->userFuncInfo.strFuncName, "SumInt3") == 0)
    {        
        pRoot->expressionResult.nResultLong = SumInt3(
            pFuncArgLeft->expressionResult.nResultLong,
            pFuncArgRight->expressionResult.nResultLong,
            pFuncArgRight->rightSiblingForMore2funcArgs->expressionResult.nResultLong );
    }
    else if (strcmp(pRoot->userFuncInfo.strFuncName, "SumInt4") == 0)
    {        
        pRoot->expressionResult.nResultLong = 
            SumInt4 ( pFuncArgLeft->expressionResult.nResultLong,
                      pFuncArgRight->expressionResult.nResultLong,
                      pFuncArgRight->rightSiblingForMore2funcArgs->expressionResult.nResultLong,
                      pFuncArgRight->rightSiblingForMore2funcArgs->rightSiblingForMore2funcArgs->expressionResult.nResultLong
            );
    }
    else if (strcmp(pRoot->userFuncInfo.strFuncName, "AsciiToInt") == 0)
    {
        pRoot->expressionResult.nResultLong = AsciiToInt ( pFuncArgLeft->expressionResult.strResult );
    }
    else if (strcmp(pRoot->userFuncInfo.strFuncName, "GetInt100") == 0)
    {
        pRoot->expressionResult.nResultLong = GetInt100();
    }
    else if (strcmp(pRoot->userFuncInfo.strFuncName, "GetStrPlus100") == 0)
    {
        strcpy ( pRoot->expressionResult.strResult, GetStrPlus100 ( pFuncArgLeft->expressionResult.nResultLong ) );
    }
    else if (strcmp(pRoot->userFuncInfo.strFuncName, "GetStrSumAndPlus100") == 0)
    {
        strcpy ( pRoot->expressionResult.strResult, GetStrSumAndPlus100 ( pFuncArgLeft->expressionResult.nResultLong,
            pFuncArgRight->expressionResult.nResultLong ) );
    }
    else if (strcmp(pRoot->userFuncInfo.strFuncName, "GetStrSum") == 0)
    {
        strcpy ( pRoot->expressionResult.strResult, GetStrSum ( pFuncArgLeft->expressionResult.nResultLong,
            pFuncArgRight->expressionResult.strResult ) );
    }
    else if (strcmp(pRoot->userFuncInfo.strFuncName, "GetStrSum3") == 0)
    {        
        strcpy ( pRoot->expressionResult.strResult, GetStrSum3 ( pFuncArgLeft->expressionResult.nResultLong,
            pFuncArgRight->expressionResult.strResult, pFuncArgRight->rightSiblingForMore2funcArgs->nLongValue ) );
    }
    else if (strcmp(pRoot->userFuncInfo.strFuncName, "GetBool") == 0)
    {        
        pRoot->expressionResult.bResult = GetBool(pFuncArgLeft->expressionResult.bResult);
    }
    else if (strcmp(pRoot->userFuncInfo.strFuncName, "testBool") == 0)
    {        
        pRoot->expressionResult.bResult = testBool ( pFuncArgLeft->expressionResult.nResultLong, 
                                                     pFuncArgRight->expressionResult.strResult, 
                                                     pFuncArgRight->rightSiblingForMore2funcArgs->expressionResult.bResult );
    }
    else if (strcmp(pRoot->userFuncInfo.strFuncName, "testBool2") == 0)
    {                
        pRoot->expressionResult.bResult = testBool2 ( pFuncArgLeft->expressionResult.nResultLong, 
                                                      pFuncArgRight->expressionResult.bResult );
    }

    return true;
}

///////////////////////////////////////////////////////////////////////////////
char*   CUserFunctions::GetStrSum3(int nVal1, char* szVal2, int nVal3)
{
    memset(&szReturn, 0x00, sizeof(szReturn));
    sprintf(szReturn, "%ld", nVal1 + atol(szVal2) + nVal3 );

    return szReturn;
}

///////////////////////////////////////////////////////////////////////////////
char*   CUserFunctions::GetStrSum(int nVal1, char* szVal2)
{
    memset(&szReturn, 0x00, sizeof(szReturn));
    sprintf(szReturn, "%ld", nVal1 + atol(szVal2) );

    return szReturn;
}

///////////////////////////////////////////////////////////////////////////////
char*  CUserFunctions::GetStrSumAndPlus100(int nVal1, int nVal2)
{
    memset(&szReturn, 0x00, sizeof(szReturn));
    sprintf(szReturn, "%d", nVal1 + nVal2 + 100);

    return szReturn;
}

///////////////////////////////////////////////////////////////////////////////
char*  CUserFunctions::GetStrPlus100(int nCnt)
{
    memset(&szReturn, 0x00, sizeof(szReturn));
    sprintf(szReturn, "%d", nCnt + 100);
    
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
    memset ( &szReturn, 0x00, sizeof( szReturn ) );
    strcat ( szReturn, str1 );
    strcat ( szReturn, str2 );

    return szReturn;

}

///////////////////////////////////////////////////////////////////////////////
char* CUserFunctions::StrCat3(char* str1, char* str2, char* str3)
{   
    memset ( &szReturn, 0x00, sizeof( szReturn ) );
    strcat ( szReturn, str1 );
    strcat ( szReturn, str2 );
    strcat ( szReturn, str3 );
    
    return szReturn ;
}

char* CUserFunctions::StrCat4(char* str1, char* str2, char* str3, char* str4)
{    
    memset ( &szReturn, 0x00, sizeof( szReturn ) );
    strcat ( szReturn, str1 );
    strcat ( szReturn, str2 );
    strcat ( szReturn, str3 );
    strcat ( szReturn, str4 );

    return szReturn;
}

///////////////////////////////////////////////////////////////////////////////
long CUserFunctions::AsciiToInt(const char* str)
{
    return atol(str);
}
