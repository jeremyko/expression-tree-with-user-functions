#ifndef	_COMMON_HEADER_H_
#define	_COMMON_HEADER_H_

#include <map>
#include <vector>
#include <iostream>
#include <memory.h>


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

typedef vector<USER_FUNC_ARG> VEC_USER_FUNC_ARGS;
typedef vector<USER_FUNC_ARG>::iterator IT_VEC_USER_FUNC_ARGS;

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

typedef vector<SFuncDefInfo> VEC_USER_FUNC;
typedef VEC_USER_FUNC::iterator            IT_VEC_USER_FUNC;
typedef map<string, SFuncDefInfo> MAP_USER_FUNC;
typedef MAP_USER_FUNC::iterator            IT_MAP_USER_FUNC;
typedef MAP_USER_FUNC::value_type          VT_MAP_USER_FUNC;
typedef pair<IT_MAP_USER_FUNC, bool>       PA_MAP_USER_FUNC;



///////////////////////////////////////////////////////////////////////////////
struct expression_node
{
    int nType;
    int nDetailedType;
    
    union _variable_container_
    {        
        //int   nIntValue;
        long  nLongValue;
        float nFloatValue;        
    } variable;
    bool  opReslut;
    char  strVal[1024];
    
    SFuncDefInfo userFuncInfo;//filled only if this is a function
    
    expression_node* nextForMore2funcArgs;
    //dynamic array for a function with more than 2 arguments.
    //if function has 4 args, left has arg1, right has arg2, 
    //funcArgsNodes[0] has arg3 and funcArgsNodes[1] has arg4.

    expression_node *left;
    expression_node *right;

    expression_node()
    {
        memset(&userFuncInfo, 0x00, sizeof(userFuncInfo));       
        nType = -1;
        nDetailedType = -1;

        opReslut = false;        
        variable.nLongValue = -1;
        variable.nFloatValue = -1.0;
        memset(&strVal, 0x00, sizeof(strVal));

        nextForMore2funcArgs = NULL;
        left = NULL;
        right = NULL;
    }
};


#endif
