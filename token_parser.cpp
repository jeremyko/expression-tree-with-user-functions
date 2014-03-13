
#include <string.h>
#include "token_parser.h"
#include "user_functions.h"
#include <memory.h>
#include<iostream>

using namespace std;

///////////////////////////////////////////////////////////////////////////////
CTokenParser::CTokenParser()
{
    strExpression = NULL;
    nFuncDepth = 0;
    bIsAvailableNegativePositiveNumberNowForThePreviousType = false;
    
    bThisIsNegativeNumber = false;
    bThisIsPositiveNumber = false;
}

///////////////////////////////////////////////////////////////////////////////
CTokenParser::~CTokenParser()
{

}

///////////////////////////////////////////////////////////////////////////////
// Token 분석할 식을 받음
void CTokenParser::PutExpression( char *exp)
{
    nFuncDepth = 0;
    bIsAvailableNegativePositiveNumberNowForThePreviousType = false;
    
    bThisIsNegativeNumber = false;
    bThisIsPositiveNumber = false;
	strExpression = exp;
}

///////////////////////////////////////////////////////////////////////////////
bool CTokenParser::GetToken(ItemTokenInfo* pResult)
{
	int            nTknType;
	int            nLength = 0;
        
    pResult->nType = NODE_UNKNOWN;
    pResult->nDetailedType = NODE_UNKNOWN;

    //cout<<"GetToken ["<<strExpression<<"]"<<endl;

	while (1)
	{
        if (
            !bIsAvailableNegativePositiveNumberNowForThePreviousType && 
            ( *strExpression == CHAR_MINUS || *strExpression == CHAR_PLUS) 
           )
        {            
            // Numeric Plus or Minus Operation is impossible ...
            //cout << "GetToken : #1 [" << strExpression << "]" << endl; 
            char* pTempPos = strExpression;
            pTempPos++; //skip minus, plus
            nLength = GetNumberCount(pTempPos);
            if (nLength > 0)
            {                
                pResult->nType = NODE_NUMBER;                
                bIsAvailableNegativePositiveNumberNowForThePreviousType = true; ///======= //음수/양수 부호의 숫자 이전 토큰이, 수식계산 가능한지 여부

                if (*strExpression == CHAR_MINUS )
                {
                    cout << "GetToken : this is the start of a negative value [" << strExpression << "]" << endl;
                    bThisIsNegativeNumber = true;
                }
                else
                {
                    cout << "GetToken : this is the start of a positive value [" << strExpression << "]" << endl;
                    bThisIsPositiveNumber = true;
                }
                
                strExpression++; //skip minus, plus
                
            }
            else
            {
                //'ABC' + 'CDE' , '12'='1'+'2'                
                bThisIsNegativeNumber = false;
                bThisIsPositiveNumber = false;
            }
            //1 - -1 =2, 1 - +1 =0
        }
        else
        {            
            bThisIsNegativeNumber = false;
            bThisIsPositiveNumber = false;
        }

        nTknType = GetTokenType(*strExpression);

        if (nTknType == CHAR_LITERAL)
        {
            nLength = GetIdentifierCount(strExpression);

            char szTemp[100];
            memset(&szTemp, 0x00, sizeof(szTemp));
            strncpy(szTemp, strExpression, nLength);
            IT_MAP_USER_FUNC itFound;
            if (funcMap.IsThisUserFunction(szTemp, itFound))
            {
                pResult->nType = NODE_USER_FUNCTIION;
                pResult->nDetailedType = USER_FUNCTIION;

                SFuncDefInfo functionToken;
                //memset(&functionToken, 0x00, sizeof(functionToken));
                memcpy(&pResult->userFuncInfo, &itFound->second, sizeof(pResult->userFuncInfo));
                memcpy(&functionToken, &itFound->second, sizeof(functionToken));
                pResult->bThisTokenBelongsToTheUserFunctions = true;

                processingFunctionStack.push(functionToken);
                nFuncDepth++;
            }
            else
            {
                pResult->nType = NODE_LITERAL;
                pResult->nDetailedType = LITERAL;
            }
            bIsAvailableNegativePositiveNumberNowForThePreviousType = false; //Literal형에 대해 이후, 음수/양수 부호의 수식계산 불가
        }
        else if (nTknType == CHAR_NUMBER)
        {
            nLength = GetNumberCount(strExpression);
            pResult->nType = NODE_NUMBER;                        
            bIsAvailableNegativePositiveNumberNowForThePreviousType = true; ///=======
        }
        else
        {
            switch (*strExpression)
            {
            case CHAR_SINGLE_QUOTATION:
                nLength = GetLiteralCount(strExpression);
                if (nLength == -1)
                {
                    cout << "NOT End Quotation Mark" << endl;
                    pResult->nType = TKN_ERROR;
                    nLength = 1;
                    pResult->nLength = 0;
                    //break;
                    return false;
                }
                pResult->nType = NODE_LITERAL;
                pResult->nDetailedType = LITERAL;
                bIsAvailableNegativePositiveNumberNowForThePreviousType = false;
                break;

            case CHAR_LEFT_PARENTHESIS:                   // '('
                nLength = 1;
                pResult->nType = NODE_DELIMITER;
                pResult->nDetailedType = LEFT_PARENTHESIS;
                if (nFuncDepth)
                {
                    pResult->bThisTokenBelongsToTheUserFunctions = true;
                }
                bIsAvailableNegativePositiveNumberNowForThePreviousType = false;
                break;

            case CHAR_RIGHT_PARENTHESIS:                  // ')'
                nLength = 1;
                pResult->nType = NODE_DELIMITER;
                pResult->nDetailedType = RIGHT_PARENTHESIS;
                if (nFuncDepth)
                {
                    pResult->bThisTokenBelongsToTheUserFunctions = true;
                    memcpy(&pResult->userFuncInfo, &processingFunctionStack.top(), sizeof(pResult->userFuncInfo));

                    processingFunctionStack.pop();
                    nFuncDepth--;
                }
                bIsAvailableNegativePositiveNumberNowForThePreviousType = true; //function's ) !!!! ///=======
                break;
                            
            case CHAR_COMMA:                
                bIsAvailableNegativePositiveNumberNowForThePreviousType = false;                
                
                //함수내의 , : 만약 함수내에 연산자가 포함된 경우 처리. : SumInt(1+1,2) --> 1 1 + 2 SumInt가 되게끔.
                //------------------------------
                nLength = 1;
                pResult->nType = NODE_DELIMITER;
                pResult->nDetailedType = COMMA;                
                //------------------------------
                break;
                
            case CHAR_PLUS:                               // '+'
                nLength = 1;
                pResult->nType = NODE_CALCULATE_OPERATOR;
                pResult->nDetailedType = OPERATOR_PLUS;
                bIsAvailableNegativePositiveNumberNowForThePreviousType = false; // 1 + +1 ==> 2nd + means positive. not plus op
                break;

            case CHAR_MINUS:                              // '-'
                nLength = 1;
                pResult->nType = NODE_CALCULATE_OPERATOR;
                pResult->nDetailedType = OPERATOR_MINUS;
                bIsAvailableNegativePositiveNumberNowForThePreviousType = false;
                break;

            case CHAR_TIMES:                              // '*'
                nLength = 1;
                pResult->nType = NODE_CALCULATE_OPERATOR;
                pResult->nDetailedType = OPERATOR_TIMES;
                bIsAvailableNegativePositiveNumberNowForThePreviousType = false;
                break;

            case CHAR_DEVIDE:                             // '/'
                nLength = 1;
                pResult->nType = NODE_CALCULATE_OPERATOR;
                pResult->nDetailedType = OPERATOR_DEVIDE;
                bIsAvailableNegativePositiveNumberNowForThePreviousType = false;
                break;

            case CHAR_EQUALS:                             // '='
                nLength = 1;
                pResult->nType = NODE_COMPARE_OPERATOR;
                pResult->nDetailedType = COMPARE_EQUALS;
                bIsAvailableNegativePositiveNumberNowForThePreviousType = true;  ///=======
                break;

            case CHAR_LESS_THAN:                          // '<'
                if (*(strExpression + 1) == CHAR_EQUALS)    // '<='
                {
                    nLength = 2;
                    pResult->nDetailedType = COMPARE_LESS_EQUALS;
                }
                else if (*(strExpression + 1) == CHAR_GREATER_THAN)// '<>'
                {
                    nLength = 2;
                    pResult->nDetailedType = COMPARE_NOT_EQUALS;
                }
                else                                      // '<'
                {
                    nLength = 1;
                    pResult->nDetailedType = COMPARE_LESS_THAN;
                }
                pResult->nType = NODE_COMPARE_OPERATOR;
                bIsAvailableNegativePositiveNumberNowForThePreviousType = true; ///=======
                break;

            case CHAR_GREATER_THAN:			              // '>'
                if (*(strExpression + 1) == CHAR_EQUALS)    // '>='
                {
                    nLength = 2;
                    pResult->nDetailedType = COMPARE_GREATER_EQUALS;
                }
                else                                      // '>'
                {
                    nLength = 1;
                    pResult->nDetailedType = COMPARE_GREATER_THAN;
                }
                pResult->nType = NODE_COMPARE_OPERATOR;
                bIsAvailableNegativePositiveNumberNowForThePreviousType = true; ///=======
                break;

            case CHAR_NOT:                                  // '!'
                if (*(strExpression + 1) == CHAR_EQUALS)    // '!='
                {
                    nLength = 2;
                    pResult->nType = NODE_COMPARE_OPERATOR;
                    pResult->nDetailedType = COMPARE_NOT_EQUALS;
                }
                else
                {                                         //no only ! 
                    nLength = 1;
                    pResult->nType = TKN_ERROR;
                }
                bIsAvailableNegativePositiveNumberNowForThePreviousType = true; ///=======
                break;

            case CHAR_AND:                                // '&'
                if (*(strExpression + 1) == CHAR_AND)       // '&&'
                    nLength = 2;
                else
                    nLength = 1;
                pResult->nType = NODE_COMPARE_OPERATOR;
                pResult->nDetailedType = COMPARE_AND;
                bIsAvailableNegativePositiveNumberNowForThePreviousType = false;
                break;

            case CHAR_OR:                                 // '|'
                if (*(strExpression + 1) == CHAR_OR)        // '||'
                    nLength = 2;
                else
                    nLength = 1;
                pResult->nType = NODE_COMPARE_OPERATOR;
                pResult->nDetailedType = COMPARE_OR;
                bIsAvailableNegativePositiveNumberNowForThePreviousType = false;
                break;

            case CHAR_SPACE:
                strExpression++;
                continue;

            case CHAR_SEMICOLON:  // ;
            case NULL:            // end of expression
                nLength = 0;
                pResult->nLength = nLength;
                bIsAvailableNegativePositiveNumberNowForThePreviousType = false;
                return true;

            default:   // incorrect value
                nLength = 1;
                pResult->nType = TKN_ERROR;
                bIsAvailableNegativePositiveNumberNowForThePreviousType = false;
                cout << "[error]strExpression is " << strExpression << endl;
                break;

            }// end of switch            
        }

        if (nFuncDepth)
        {
            pResult->bThisTokenBelongsToTheUserFunctions = true;
            memcpy(&pResult->userFuncInfo, &processingFunctionStack.top(), sizeof(pResult->userFuncInfo));
        }
        
		if (TKN_ERROR == pResult->nType)
		{
            pResult->nLength = 0;
			return false;
		}
                        
        pResult->nLength = nLength;
        
        if (bThisIsNegativeNumber || bThisIsPositiveNumber)
        {
            if (bThisIsNegativeNumber )
            {
                strcat(pResult->strValue, "-");
            }
            else
            {
                strcat(pResult->strValue, "+");
            }            
            strncpy(pResult->strValue+1, strExpression, nLength);
            *(pResult->strValue + nLength+1) = 0;

            if (NULL == strchr(pResult->strValue, '.'))
            {
                pResult->nDetailedType = NUMBER_LONG;
            }
            else
            {
                pResult->nDetailedType = NUMBER_FLOAT;
            }
        }
        else
        {
            strncpy(pResult->strValue, strExpression, nLength);
            *(pResult->strValue + nLength) = 0;

            if (pResult->nType == NODE_NUMBER)
            {
                if (NULL == strchr(pResult->strValue, '.'))
                {
                    pResult->nDetailedType = NUMBER_LONG;
                }
                else
                {
                    pResult->nDetailedType = NUMBER_FLOAT;
                }                
            }            
        }
               
        strExpression += nLength;
		
		nLength = 0;
		return true;

	}// end of while

	return false;
}


///////////////////////////////////////////////////////////////////////////////
// Token 종류를 구분하기 위해 맨 앞 character를 확인한다
int CTokenParser::GetTokenType(char chexp)
{   
	if (((chexp >= 'A') && (chexp <= 'z')) || chexp == '_' )
	{
		return CHAR_LITERAL;
	}
    else if ( ((chexp >= 0x30) && (chexp <= 0x39)) || chexp == '.' )
	{
		return CHAR_NUMBER;
	}
    else if (chexp == 0x20 )
    {
        return CHAR_SPACE;
    }
	else
	{
		return false;
	}
}

///////////////////////////////////////////////////////////////////////////////
int CTokenParser::GetIdentifierCount( char *strexp)
{
	int  nRet;
	int  nCount = 0;
	char *imsiPtr;

	imsiPtr = strexp;

	while (1)
	{
		nRet = GetTokenType(*imsiPtr);
		if (nRet == CHAR_LITERAL || nRet == CHAR_NUMBER)
		{
			imsiPtr++;
			nCount++;
		}
		else
		{
			return nCount;
		}
	}// end of while
}

///////////////////////////////////////////////////////////////////////////////
int CTokenParser::GetNumberCount( char *strexp)
{
	int nCount = 0;
	char *imsiPtr;
	imsiPtr = strexp;

	while (1)
	{
		if (GetTokenType(*imsiPtr) == CHAR_NUMBER)
		{
			imsiPtr++;
			nCount++;
		}
        else if (GetTokenType(*imsiPtr) == CHAR_SPACE)
        {
            imsiPtr++;
            continue;
        }
        else
        {
            return nCount;
        }			
	}
}

///////////////////////////////////////////////////////////////////////////////
int CTokenParser::GetLiteralCount( char *strexp)
{
	int nCount = 0;
	char *imsiPtr;

	imsiPtr = strexp;
	if (*imsiPtr == CHAR_SINGLE_QUOTATION)          // 시작 quotation
	{
		imsiPtr++;
		nCount++;
	}

	while (1)
	{
		if (*imsiPtr == 0x00)
			return -1;
		else if (*imsiPtr == CHAR_SINGLE_QUOTATION) // 끝 quotation
		{
			nCount++;
			return nCount;
		}
		else
		{
			imsiPtr++;
			nCount++;
		}
	}// end of while	
}

///////////////////////////////////////////////////////////////////////////////
int CTokenParser::GetTokenType(char * chexp)
{
	for (int i = 0; i < (int)strlen(chexp); i++)
	{
        if (GetTokenType(chexp[i]) == CHAR_LITERAL)
        {
            return NODE_LITERAL;
        }			
	}
	return NODE_NUMBER;
}


/* TEST
if (pResult->nType == NODE_USER_FUNCTIION)
{
//set nLength to the last end of the function --> last ')' 함수전체를 얻음
int nFunctionDepth = 0;
for (int nCount = 0; nCount < (int)strlen(strExpression); nCount++)
{
if (strExpression[nCount] == CHAR_LEFT_PARENTHESIS)
{
nFunctionDepth++;
}
else if (strExpression[nCount] == CHAR_RIGHT_PARENTHESIS)
{
nFunctionDepth--;
if (nFunctionDepth == 0)
{
nLength = nCount + 1;
strncpy(pResult->strValue, strExpression, nLength);
pResult->nLength = nLength;
strExpression += nLength;
break;
}
}
}// end of for

if (nFunctionDepth != 0)
{
cout << "[error] strExpression : Wrong usage of user function " << strExpression << endl;
pResult->nLength = 0;
return false;
}
}
else
*/
