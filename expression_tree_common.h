#ifndef	_COMMON_HEADER_H_
#define	_COMMON_HEADER_H_

#include <map>
#include <vector>
#include <iostream>

using namespace std;

enum EnumFuncReturnType
{
    FUNC_RTN_NUM = 1,
    FUNC_RTN_STR,
    FUNC_RTN_BOOL,
    MAX_FUNC_RTN_TYPE
};

enum EnumFuncNumReturnDetailedType
{
    /*
    NUMBER_INT,            //17
    NUMBER_LONG,           //18
    NUMBER_FLOAT,          //19
    */

    //FUNC_RTN_DETAILED_INT = 17, // XXX token_parser.h 와 동일해야하는 문제
    FUNC_RTN_DETAILED_LONG = 18,
    FUNC_RTN_DETAILED_FLOAT,
    FUNC_RTN_DETAILED_STR,
    FUNC_RTN_DETAILED_BOOL,
    MAX_FUNC_RTN_DETAILED_TYPE
};

enum EnumFuncArgsType
{
    FUNC_ARG_NONE = 0,
    FUNC_ARG_NUM,
    FUNC_ARG_INT,
    FUNC_ARG_LONG_INT,
    FUNC_ARG_STR,
    FUNC_ARG_BOOL,
    MAX_FUNC_ARG_TYPE
};

typedef struct _USER_FUNC_ARG
{
    int nArgType;
    union _variable_container_
    {
        bool    _bool;        
        long    _long;
        float   _float;
        char    _string[1024];
    } variable;

} USER_FUNC_ARG;


typedef struct _SFuncDefInfo
{
    char strFuncName[50];
    int nReturnType;
    int nDetailedReturnType;
    int ntotalInputArgCnt;
    int nFuncArgsTypes[30]; // FUNC_ARG_NUM, FUNC_ARG_STR
        
    _SFuncDefInfo()
    {        
        memset(&strFuncName, 0x00, sizeof(strFuncName));
        nReturnType = -1;
        nDetailedReturnType = -1;
        ntotalInputArgCnt = -1;
        memset(&nFuncArgsTypes, -1, sizeof(nFuncArgsTypes));
    }

} SFuncDefInfo;

typedef map<string, SFuncDefInfo> MAP_USER_FUNC;
typedef MAP_USER_FUNC::iterator            IT_MAP_USER_FUNC;
typedef MAP_USER_FUNC::value_type          VT_MAP_USER_FUNC;
typedef pair<IT_MAP_USER_FUNC, bool>       PA_MAP_USER_FUNC;


///////////////////////////////////////////////////////////////////////////////
class PlaceHolderValue //20140314
{
public:
    PlaceHolderValue()
    {
        nLongValue  = -1;
        nFloatValue = -1.0;
        memset(stringVal, 0x00, sizeof(stringVal));
    }
protected:
    int     nPlaceHolderDataType; //NODE_NUMBER, NODE_LITERAL    
    int     nPlaceHolderDetailedDataType; // NUMBER_INT, NUMBER_LONG, NUMBER_FLOAT  
    long    nLongValue;
    float   nFloatValue;
    char    stringVal[1024];

public:
    void SetDataType(int nDataType, int nDetailedDataType)
    {
        nPlaceHolderDataType = nDataType;
        nPlaceHolderDetailedDataType = nDetailedDataType;
    }

    int GetDataType() { return nPlaceHolderDataType;  }
    int GetDetailedDataType() { return nPlaceHolderDetailedDataType; }

    void SetStringData(const char* strData)
    {
        strncpy(stringVal, strData, sizeof(stringVal));
    }

    char* GetStringData()
    {
        return stringVal;
    }

    void SetNumberLongData(long  nVal)
    {
        nLongValue = nVal;
    }

    long GetNumberLongData()
    {
        return nLongValue ;
    }

    void SetNumberFloatData(float  nVal)
    {
        nFloatValue = nVal;
    }

    float GetNumberFloatData()
    {
        return nFloatValue;
    }
};

///////////////////////////////////////////////////////////////////////////////
typedef struct _expression_result_
{
    int    nResultType;
    int    nResultDetailedType;
    
    long   nResultLong;
    float  nResultFloat;    
    bool   bResult;
    char   strResult[1024];

    _expression_result_()
    {
        nResultType = -1;
        nResultDetailedType = -1;
        nResultLong = -1;
        nResultFloat = -1.0;
        bResult = false;        
        memset(&strResult, 0x00, sizeof(strResult));
    }
} expression_result;

///////////////////////////////////////////////////////////////////////////////
struct expression_node
{
    //--------------------------------------
    //input or meta or function, operator info
    int    nType;
    int    nDetailedType;
    long   nLongValue;
    float  nFloatValue;                
    char   strVal[1024];    
    SFuncDefInfo userFuncInfo;//filled only if this is a function
    //--------------------------------------
    
    //--------------------------------------
    //result of operator, user function
    //initial value is same as input
    expression_result expressionResult; 
    //--------------------------------------
        
    expression_node *left;
    expression_node *right;
        
    //dynamic array for a function with more than 2 arguments.
    //if function has 4 args, left has arg1, right has arg2, 
    //right->siblingForMore2funcArgs has arg3 and 
    //right->siblingForMore2funcArgs->siblingForMore2funcArgs has arg4.
    expression_node* rightSiblingForMore2funcArgs;

    expression_node()
    {
        memset(&userFuncInfo, 0x00, sizeof(userFuncInfo));       
        nType = -1;
        nDetailedType = -1;        
        nLongValue = -1;
        nFloatValue = -1.0;
        memset(&strVal, 0x00, sizeof(strVal));

        rightSiblingForMore2funcArgs = NULL;
        left = NULL;
        right = NULL;
    }
};



#endif
