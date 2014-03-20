#ifndef	_TOKEN_PARSER_H_
#define	_TOKEN_PARSER_H_

#include "user_functions.h"
#include <stack>

typedef struct __ItemTokenInfo
{
	int     nType;         
	int     nDetailedType;      
	int     nLength;    
    char    strValue[1024];    
    bool    bThisTokenBelongsToTheUserFunctions;//true only if this token belongs to a function :    
    SFuncDefInfo userFuncInfo;
} ItemTokenInfo ;

//--------------------------------------------------------------------------
enum EnumErrorTokenType
{
    TKN_ERROR    = 0,       // Error Token -> Incorrect condition        
    TKN_UNKNOWN 
};

enum EnumNodeType
{
    NODE_UNKNOWN = 0,             
    NODE_LITERAL,            //1 String Value: ' or A-z or . or _
    NODE_NUMBER,             //2 Integer Value        
    NODE_BOOL,               //3 
    NODE_COMPARE_OPERATOR,   //4 Operator  =, <, >, !, &, | 		
    NODE_CALCULATE_OPERATOR, //5 Operator  +, -, *, /	
    NODE_DELIMITER,          //6 Delimiter (, ), ,(a pause)		        
    NODE_USER_FUNCTIION,     //7 functions in CUserFunctions
    NODE_PLACE_HOLDER,       //8 20140314 : 
    NODE_EMPTY ,             //9 empty args  --> 인자수 하나인 함수의 경우 child 트리 node 2개 조건을 만족하게 하기 위해 빈 노드를 가진다 XXX -> TODO 개선..
    MAX_NODE_TYPE
};

//TKN_END,               // 식이 끝났음
//TKN_QUESTION ,
//TKN_COLON ,   
//PAUSE,                  // ,
//TKN_NULL,

enum EnumNodeDetailedType
{    
    //TKN_CALCULAT_OPERATOR
    OPERATOR_PLUS,    //0 +
    OPERATOR_MINUS,   //1 - 
    OPERATOR_TIMES,   //2 *
    OPERATOR_DEVIDE,  //3 /

    //TKN_COMPARE_OPERATOR
    COMPARE_AND,           //4 '&'
    COMPARE_OR,            //5 '|'
    COMPARE_EQUALS,        //6 '='
    COMPARE_NOT_EQUALS,    //7 '<>' '!='
    COMPARE_GREATER_THAN,  //8 '>'
    COMPARE_GREATER_EQUALS,//9 '>='
    COMPARE_LESS_THAN,     //10 '<'
    COMPARE_LESS_EQUALS,   //11 '<='

    //TKN_DELIMITER
    LEFT_PARENTHESIS,      //12
    RIGHT_PARENTHESIS,     //13
    COMMA,                 //14
    USER_FUNCTIION,        //15
    LITERAL,               //16    
    NUMBER_INT,            //17
    NUMBER_LONG,           //18
    NUMBER_FLOAT,         //19    
    BOOL_TYPE,             //20
    PLACE_HOLDER           //21 20140314 
};

enum EnumTypeOfOneCharToken
{
    CHAR_LITERAL = 300, // Character is Literal
    CHAR_NUMBER         // Character is Number
};

//--------------------------------------------------------------------------
const char CHAR_SINGLE_QUOTATION    = '\'';
const char CHAR_LEFT_PARENTHESIS    = '(';
const char CHAR_RIGHT_PARENTHESIS   = ')';
const char CHAR_MINUS               = '-';
const char CHAR_TIMES               = '*';
const char CHAR_PLUS                = '+';
const char CHAR_DEVIDE              = '/';
const char CHAR_EQUALS              = '=';
const char CHAR_LESS_THAN           = '<';
const char CHAR_GREATER_THAN        = '>';
const char CHAR_NOT                 = '!';
const char CHAR_AND                 = '&';
const char CHAR_OR                  = '|';
const char CHAR_SPACE               = ' ';
const char CHAR_SEMICOLON           = ';';
const char CHAR_COLON               = ':';
const char CHAR_QUESTION            = '?';
const char CHAR_PERCENT             = '%';
const char CHAR_DOLLAR              = '$';
//const char CHAR_PAUSE               = ',';
const char CHAR_COMMA = ',';

// :$ph1, :$ph2, :$ph3...
#define PLACE_HOLDER_PREFIX  ":$ph" //using placeholder 20140314

//--------------------------------------------------------------------------
class CTokenParser
{
private:
	char* strExpression;	    
    int GetTokenType(char chexp);      	    
    int GetNumberCount( char* strexp);  	    
    int GetLiteralCount( char* strexp); 
    int nFuncDepth;
    stack<SFuncDefInfo> processingFunctionStack;   
    bool bIsAvailableNegativePositiveNumberNowForThePreviousType; //for checking negative number :     
    bool bThisIsNegativeNumber; 
    bool bThisIsPositiveNumber; 

protected:	
	int GetIdentifierCount( char* strexp);    
	int GetTokenType(char*);
    int GetPlaceHolderLength(char *strexp); //20140314
    CUserFunctions funcMap;    

public:
	CTokenParser();
	~CTokenParser();

	void PutExpression( char *exp);	
	bool GetToken(ItemTokenInfo* pResult);
};

#endif
