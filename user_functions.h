#ifndef	_USER_FUNCTIONS_H_
#define	_USER_FUNCTIONS_H_

#include <map>
#include <vector>
#include <iostream>
#include "expression_tree_common.h"

using namespace std;

class CUserFunctions
{
private:    
    MAP_USER_FUNC mapFuncDefinitions;
    bool    Initialize();
    void    InsertMap(int nRtnType, int nRtnTypeDetailed, const char* strFuncName, int nArgCnt, ...);
    int     StrLen(const char* str);
    int     SumInt(int n1, int n2);
    int     SumInt3(int n1, int n2, int n3);
    int     SumInt4(int n1, int n2, int n3, int n4);
    long    AsciiToInt(const char* str);
    char*   StrCat(char* str1, char* str2);
    bool    GetBool(bool bCondition) { return bCondition;  };

    //functions for test ----------------
    char*   StrCat3(char* str1, char* str2, char* str3);
    char*   StrCat4(char* str1, char* str2, char* str3, char* str4);
    int     GetInt100() {  return 100;  };
    char*   GetStrPlus100(long nCnt);
    char*   GetStrSumAndPlus100(long nVal1, long nVal2);
    char*   GetStrSum(long nVal1, char* szVal2);
    char*   GetStrSum3(long nVal1, char* szVal2, long nVal3);
    bool    testBool(int nVal1, char* szVal2, bool boolFlag);
    bool    testBool2(int nVal, bool boolFlag);
    //functions for test ----------------

protected:    
    char szReturn[1024];

public:
    CUserFunctions();
    ~CUserFunctions();
    
    bool GetStringSingleQuotationsBothSidesRemoved(char* strExpression);
    bool IsThisUserFunction(const char* strFuncName,  IT_MAP_USER_FUNC& itFound);    
       
    bool InvokeUserFunction(expression_node* pRoot, expression_node* pFuncArgLeft, expression_node* pFuncArgRight);
};
#endif

