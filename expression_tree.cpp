
#include "expression_tree.h"
#include <memory.h>
#include <string.h>

const char* NodeTypeDesc[] = {
    "NODE_UNKNOWN",    
    "NODE_LITERAL",
    "NODE_NUMBER",
    "NODE_COMPARE_OPERATOR",
    "NODE_CALCULATE_OPERATOR",
    "NODE_DELIMITER",
    "NODE_USER_FUNCTIION",
    "NODE_PLACE_HOLDER", 
    "NODE_EMPTY"
};

const char* DetailedTypeDesc[] = {
    "OPERATOR_PLUS",
    "OPERATOR_MINUS",
    "OPERATOR_TIMES",
    "OPERATOR_DEVIDE",
    "COMPARE_AND",
    "COMPARE_OR",
    "COMPARE_EQUALS",
    "COMPARE_NOT_EQUALS",
    "COMPARE_GREATER_THAN",
    "COMPARE_GREATER_EQUALS",
    "COMPARE_LESS_THAN",
    "COMPARE_LESS_EQUALS",
    "LEFT_PARENTHESIS",
    "RIGHT_PARENTHESIS",
    "PAUSE",
    "USER_FUNCTIION",
    "LITERAL",        
    "NUMBER_INT", 
    "NUMBER_LONG",  
    "NUMBER_FLOAT", 
    "BOOL_TYPE",   
    "PLACE_HOLDER" 
};

///////////////////////////////////////////////////////////////////////////////
// Priority of Operator
const int MAX_OP_PRIORITY_TABLE_CNT = 12;

typedef struct _struct_OpPriority
{
    int nOpType;
    int nPriority;
} type_struct_OpPriority;

type_struct_OpPriority OpPriorityTable[MAX_OP_PRIORITY_TABLE_CNT] =
{
    { COMPARE_AND,              1 },    // '&'
    { COMPARE_OR,               1 },    // '|'
    { COMPARE_EQUALS,           2 },    // '='
    { COMPARE_NOT_EQUALS,       2 },    // '<>' '!='
    { COMPARE_GREATER_THAN,     2 },    // '>'
    { COMPARE_GREATER_EQUALS,   2 },    // '>='
    { COMPARE_LESS_THAN,        2 },    // '<'
    { COMPARE_LESS_EQUALS,      2 },    // '<='    

    { OPERATOR_PLUS,            3 },    // +
    { OPERATOR_MINUS,           3 },    // -   
    { OPERATOR_TIMES,           4 },    // *
    { OPERATOR_DEVIDE,          4 }     // /     
};

///////////////////////////////////////////////////////////////////////////////
ExpressionTree::ExpressionTree() 
{
    memStatus = 0;
    root_node = NULL;        
    nDepthOfTree = 0;           
}

///////////////////////////////////////////////////////////////////////////////
ExpressionTree::~ExpressionTree()
{   
    DeleteExpressionTree(root_node);    
    cout << "memStatus: " << memStatus << "\n";
}


///////////////////////////////////////////////////////////////////////////////
bool ExpressionTree::IsUserFunction(ItemTokenInfo* pNodeInfo)
{    
    if (pNodeInfo->nType == NODE_USER_FUNCTIION)    
    {        
        return true;
    }
        
    return false;
}

///////////////////////////////////////////////////////////////////////////////
bool ExpressionTree::IsOperator(ItemTokenInfo* pNodeInfo)
{
    if (pNodeInfo->nType == NODE_COMPARE_OPERATOR || pNodeInfo->nType == NODE_CALCULATE_OPERATOR)
    {        
        return true;
    }    

    return false;
}

///////////////////////////////////////////////////////////////////////////////
bool ExpressionTree::IsOperand(ItemTokenInfo* pNodeInfo)
{
    if (!IsOperator(pNodeInfo) &&
        pNodeInfo->nDetailedType != LEFT_PARENTHESIS &&
        pNodeInfo->nDetailedType != RIGHT_PARENTHESIS &&
        pNodeInfo->nDetailedType != COMMA
        )
    {        
        return true;
    }
    
    return false;
}

///////////////////////////////////////////////////////////////////////////////
int ExpressionTree::GetOperatortPriority(ItemTokenInfo* pNodeInfo)
{
    for (int i = 0; i < MAX_OP_PRIORITY_TABLE_CNT; i++)
    {
        if (pNodeInfo->nDetailedType == OpPriorityTable[i].nOpType)
        {
            return OpPriorityTable[i].nPriority;
        }
    }
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
int ExpressionTree::GetOperatorPrecedence(ItemTokenInfo* pNodeInfo1, ItemTokenInfo* pNodeInfo2)
{    
    int nPriority1 = 0;
    int nPriority2 = 0;

    nPriority1 = GetOperatortPriority(pNodeInfo1);
    nPriority2 = GetOperatortPriority(pNodeInfo2);

    if (nPriority1 > nPriority2)
    {
        return -1;
    }
    else if (nPriority1 < nPriority2)
    {
        return 1;
    }

    return 0;
}


///////////////////////////////////////////////////////////////////////////////
bool ExpressionTree::SetInfixExpression(const char* inFix)
{       
    cout << "Infix:" << inFix << "\n";        
    vector<ItemTokenInfo>().swap(vecPostfixResult);
    //vecPostfixResult.shrink_to_fit();
                        
    CTokenParser parser;
    parser.PutExpression((char*)inFix);

    stack<ItemTokenInfo> opStack;
    string postFixString = "";    
    bool bErrorOccurred = false;

    while (1)
    {
        ItemTokenInfo token;
        memset(&token, 0x00, sizeof(token));
        if (!parser.GetToken(&token))
        {
            bErrorOccurred = true;
            break;
        }

        if (0 == token.nLength)
        {
            break;
        }

        #ifdef _DEBUG_MORE_
        cout << __LINE__ << ": token.nLength=" << token.nLength  << " token.nType=" << token.nType
             << " token.strTokenValue= " << token.strValue  <<  "\n";
        #endif
        
        if (IsUserFunction(&token))
        {
            opStack.push(token); //no precedence between functions 
        }
        else if (IsOperand(&token))
        {
            postFixString += " ";
            postFixString += token.strValue;
            vecPostfixResult.push_back(token);            
        }
        else if (IsOperator(&token))
        {
            while ( !opStack.empty() &&
                     LEFT_PARENTHESIS != opStack.top().nDetailedType && 
                     GetOperatorPrecedence(&opStack.top(), &token) <= 0 
                  )
            {                
                postFixString += " ";
                postFixString += opStack.top().strValue;
                vecPostfixResult.push_back(opStack.top());
                opStack.pop();                
            }
            opStack.push(token);            
        }
        else if (COMMA == token.nDetailedType && token.bThisTokenBelongsToTheUserFunctions ) // SumInt(1+1, 2)
        {
            while (!opStack.empty() &&
                    LEFT_PARENTHESIS != opStack.top().nDetailedType                     
                  )
            {
                postFixString += " ";
                postFixString += opStack.top().strValue;
                vecPostfixResult.push_back(opStack.top());
                opStack.pop();
            }           
        }
        else if (LEFT_PARENTHESIS == token.nDetailedType)
        {            
            opStack.push(token);
        }
        else if (RIGHT_PARENTHESIS == token.nDetailedType) 
        {
            //pop till starting '('
            while (!opStack.empty())
            {                
                if (LEFT_PARENTHESIS == opStack.top().nDetailedType)
                {
                    if (token.bThisTokenBelongsToTheUserFunctions)
                    {
                        //pop till starting function name 
                        opStack.pop(); 
                        
                        if (opStack.top().nDetailedType == LEFT_PARENTHESIS)
                        {
                            // (SumInt(1,1))
                            opStack.pop();
                        }
                        else
                        {
                            postFixString += " ";
                            postFixString += opStack.top().strValue; //function name
                            vecPostfixResult.push_back(opStack.top());
                            opStack.pop();
                        }
                        
                        break; 
                    }
                    else
                    {
                        opStack.pop();                         
                        break; 
                    }                    
                }
                postFixString += " ";
                postFixString += opStack.top().strValue;
                vecPostfixResult.push_back(opStack.top());
                                
                opStack.pop();
            }
        } //RIGHT_PARENTHESIS
    }

    if (bErrorOccurred)
    {
        return false;
    }

    while (!opStack.empty())
    {
        postFixString += " ";
        postFixString += opStack.top().strValue;

        vecPostfixResult.push_back(opStack.top());

        opStack.pop();        
    }

    cout << "Postfix is: " << postFixString << endl;
    
    return BuildExpressionTree();    
}


///////////////////////////////////////////////////////////////////////////////
expression_node* ExpressionTree::CreateTreeNode()
{
    expression_node* node = new expression_node;   

    treeNodeMemRepository.push(node);
    ++memStatus;
    return node;
}

///////////////////////////////////////////////////////////////////////////////
bool ExpressionTree::CheckCompareError(expression_node* pOpRsltLeft, expression_node* pOpRsltRight)
{       
    if ( pOpRsltLeft->expressionResult.nResultType == pOpRsltRight->expressionResult.nResultType )
    {
        return true;
    }
    else if ( pOpRsltLeft->expressionResult.nResultType != pOpRsltRight->expressionResult.nResultType )
    {
        return false;
    }
    else
    {
        if ( pOpRsltLeft->expressionResult.nResultDetailedType != pOpRsltRight->expressionResult.nResultDetailedType )
        {
            return false;
        }

        return true;
    }
}

///////////////////////////////////////////////////////////////////////////////
void ExpressionTree::EvaluateStringCondition(   
    expression_node *root, 
    expression_node* pRsltLeft,
    expression_node* pRsltRight    )
{
    //cout << "(" <<__func__<<":"<<__LINE__ << ") debug" << "\n"; //debug
    root->expressionResult.bResult = false;

    switch (root->nDetailedType)
    {
    case OPERATOR_PLUS:
        strcat(root->expressionResult.strResult , pRsltLeft->expressionResult.strResult);
        strcat ( root->expressionResult.strResult, pRsltRight->expressionResult.strResult );
        break;
	case COMPARE_EQUALS:
		if ( strcmp ( pRsltLeft->expressionResult.strResult, pRsltRight->expressionResult.strResult ) == 0 )  //20140408
		{
			root->expressionResult.bResult = true;
		}
	    /*
	    //-------------------------------------------------------------
        // 20140408 : % 
        if ( pRsltLeft->expressionResult.strResult [ 0 ] != CHAR_PERCENT &&
             pRsltLeft->expressionResult.strResult [ strlen ( pRsltLeft->expressionResult.strResult ) - 1 ] != CHAR_PERCENT  &&
             pRsltRight->expressionResult.strResult [ 0 ] != CHAR_PERCENT &&
             pRsltRight->expressionResult.strResult [ strlen ( pRsltRight->expressionResult.strResult ) - 1 ] != CHAR_PERCENT
             )
        {
            if ( strcmp ( pRsltLeft->expressionResult.strResult, pRsltRight->expressionResult.strResult ) == 0 )  //20140408
            {
                root->expressionResult.bResult = true;
            }
        }
        else            
        {
            if ( pRsltLeft->expressionResult.strResult [ 0 ] == CHAR_PERCENT && 
                 pRsltLeft->expressionResult.strResult [ strlen ( pRsltLeft->expressionResult.strResult ) -1 ] != CHAR_PERCENT )
            {
                // '%99'='AA99'
                char* pTemp = strstr ( pRsltRight->expressionResult.strResult, &pRsltLeft->expressionResult.strResult [ 1 ] );
                if ( pTemp != 0x00 && ! strcmp ( &pRsltLeft->expressionResult.strResult [ 1 ], pTemp ) )
                {
                    root->expressionResult.bResult = true;
                }
            }
            else if ( pRsltLeft->expressionResult.strResult [ 0 ] == CHAR_PERCENT &&
                      pRsltLeft->expressionResult.strResult [ strlen ( pRsltLeft->expressionResult.strResult ) - 1 ] == CHAR_PERCENT )
            {
                // '%678%'='12345678A'
                char szTemp [ 1024 ];
                memset ( &szTemp, 0x00, sizeof( szTemp ) );
                strncpy ( szTemp, &pRsltLeft->expressionResult.strResult[1], strlen ( pRsltLeft->expressionResult.strResult ) - 2 );
                char* pTemp = strstr ( pRsltRight->expressionResult.strResult, szTemp );

                if ( pTemp != 0x00  )
                {
                    root->expressionResult.bResult = true;
                }                
            }
            else if ( pRsltRight->expressionResult.strResult [ 0 ] == CHAR_PERCENT &&
                      pRsltRight->expressionResult.strResult [ strlen ( pRsltRight->expressionResult.strResult ) - 1 ] != CHAR_PERCENT )
            {
                // 'AA99' = '%99'                
                char* pTemp = strstr ( pRsltLeft->expressionResult.strResult, &pRsltRight->expressionResult.strResult [ 1 ] );
                if ( pTemp != 0x00 && !strcmp ( &pRsltRight->expressionResult.strResult [ 1 ], pTemp ) )
                {
                    root->expressionResult.bResult = true;
                }
            }
            else if ( pRsltRight->expressionResult.strResult [ 0 ] == CHAR_PERCENT &&
                      pRsltRight->expressionResult.strResult [ strlen ( pRsltRight->expressionResult.strResult ) - 1 ] == CHAR_PERCENT )
            {
                // '12345678A' = '%678%'
                char szTemp [ 1024 ];
                memset ( &szTemp, 0x00, sizeof( szTemp ) );
                strncpy ( szTemp, &pRsltRight->expressionResult.strResult [ 1 ], strlen ( pRsltRight->expressionResult.strResult ) - 2 );
                char* pTemp = strstr ( pRsltLeft->expressionResult.strResult, szTemp );

                if ( pTemp != 0x00 )
                {
                    root->expressionResult.bResult = true;
                }
            }
        }        
        //-------------------------------------------------------------
		*/
        break;

    case COMPARE_NOT_EQUALS:
        if ( strcmp ( pRsltLeft->expressionResult.strResult, pRsltRight->expressionResult.strResult ) != 0 )
        {
            root->expressionResult.bResult = true;
        }        
        break;

    case COMPARE_GREATER_THAN:
        if ( strcmp ( pRsltLeft->expressionResult.strResult, pRsltRight->expressionResult.strResult ) > 0 )
        {
            root->expressionResult.bResult = true;
        }        
        break;

    case COMPARE_GREATER_EQUALS:
        if ( strcmp ( pRsltLeft->expressionResult.strResult, pRsltRight->expressionResult.strResult ) >= 0 )
        {
            root->expressionResult.bResult = true;
        }
        break;

    case COMPARE_LESS_THAN:
        if ( strcmp ( pRsltLeft->expressionResult.strResult, pRsltRight->expressionResult.strResult ) < 0 )
        {
            root->expressionResult.bResult = true;
        }
        break;

    case COMPARE_LESS_EQUALS:
        if ( strcmp ( pRsltLeft->expressionResult.strResult, pRsltRight->expressionResult.strResult ) <= 0 )
        {
            root->expressionResult.bResult = true;
        }
        break;
    }

    if (root->nType == NODE_CALCULATE_OPERATOR)
    {        
        root->expressionResult.bResult = true;
        root->expressionResult.nResultType = NODE_LITERAL;
        root->expressionResult.nResultDetailedType = LITERAL;
    }     
    else if ( root->nType == NODE_COMPARE_OPERATOR )
    {
        root->expressionResult.nResultType = NODE_BOOL;
        root->expressionResult.nResultDetailedType = BOOL_TYPE;
    }
}

///////////////////////////////////////////////////////////////////////////////
void ExpressionTree::EvaluateBoolCondition (
    expression_node *root,
    expression_node* pRsltLeft,
    expression_node* pRsltRight )
{
    root->expressionResult.bResult = false;

    switch ( root->nDetailedType )
    {
    case COMPARE_AND:
        if ( pRsltLeft->expressionResult.bResult && pRsltRight->expressionResult.bResult )
        {
            root->expressionResult.bResult = true;
        }
        break;
    case COMPARE_OR:
        if ( pRsltLeft->expressionResult.bResult || pRsltRight->expressionResult.bResult )
        {
            root->expressionResult.bResult = true;
        }
        break;
    }

    root->expressionResult.nResultType = NODE_BOOL;
    root->expressionResult.nResultDetailedType = BOOL_TYPE;
}


///////////////////////////////////////////////////////////////////////////////
void ExpressionTree::EvaluateNumericCondition(  
    expression_node *root,
    expression_node* pRsltLeft,
    expression_node* pRsltRight  )
{       
            
    cout << "(" <<__func__<<":"<<__LINE__ << ") debug" << "\n"; //debug
    int  nCase = 0;
    long  nLongLeft = 0;
    long  nLongRight = 0;
    float nFloatLeft = 0;
    float nFloatRight = 0;

    nCase = nCase & 0xffff;
   
    if ( pRsltLeft->expressionResult.nResultDetailedType == NUMBER_LONG )
    {
        nLongLeft = pRsltLeft->expressionResult.nResultLong;
        nCase = nCase | CASE_L_LONG;
    }
    else if ( pRsltLeft->expressionResult.nResultDetailedType == NUMBER_FLOAT )
    {
        nFloatLeft = pRsltLeft->expressionResult.nResultFloat;
        nCase = nCase | CASE_L_FLOAT;
    }

    if ( pRsltRight->expressionResult.nResultDetailedType == NUMBER_LONG )
    {
        nLongRight = pRsltRight->expressionResult.nResultLong;
        nCase = nCase | CASE_R_LONG;
    }
    else if ( pRsltRight->expressionResult.nResultDetailedType == NUMBER_FLOAT )
    {
        nFloatRight = pRsltRight->expressionResult.nResultFloat;
        nCase = nCase | CASE_R_FLOAT;
    }

    //cout << "nCase : " << nCase <<  "\n";
    //cout << "nLongLeft : " << nLongLeft << "/ nLongRight:" << nLongRight << "\n";
    //cout << "nFloatLeft: " << nFloatLeft << "/ nFloatRight:" << nFloatRight << "\n";
        
    if ( nCase == CASE_L_LONG_R_LONG )
    {
        root->expressionResult.nResultType = NODE_NUMBER;
        root->expressionResult.nResultDetailedType = NUMBER_LONG;
    }
    else if ( nCase == CASE_L_LONG_R_FLOAT || nCase == CASE_L_FLOAT_R_LONG || nCase == CASE_L_FLOAT_R_FLOAT )
    {
        root->expressionResult.nResultType = NODE_NUMBER;
        root->expressionResult.nResultDetailedType = NUMBER_FLOAT;
    }
    
    root->expressionResult.bResult = false;

    switch ( root->nDetailedType )
    {
    case OPERATOR_PLUS:
        if ( nCase == CASE_L_LONG_R_LONG )
        {
            root->expressionResult.nResultLong = nLongLeft + nLongRight;
        }
        else if ( nCase == CASE_L_LONG_R_FLOAT )
        {
            root->expressionResult.nResultFloat = nLongLeft + nFloatRight;
        }
        else if ( nCase == CASE_L_FLOAT_R_LONG )
        {
            root->expressionResult.nResultFloat = nFloatLeft + nLongRight;
        }
        else if ( nCase == CASE_L_FLOAT_R_FLOAT )
        {
            root->expressionResult.nResultFloat = nFloatLeft + nFloatRight;
        }
        break;
    case OPERATOR_MINUS:
        if ( nCase == CASE_L_LONG_R_LONG )
        {
            root->expressionResult.nResultLong = nLongLeft - nLongRight;
        }
        else if ( nCase == CASE_L_LONG_R_FLOAT )
        {
            root->expressionResult.nResultFloat = nLongLeft - nFloatRight;
        }
        else if ( nCase == CASE_L_FLOAT_R_LONG )
        {
            root->expressionResult.nResultFloat = nFloatLeft - nLongRight;
        }
        else if ( nCase == CASE_L_FLOAT_R_FLOAT )
        {
            root->expressionResult.nResultFloat = nFloatLeft - nFloatRight;
        }
        break;
    case OPERATOR_TIMES:
        if ( nCase == CASE_L_LONG_R_LONG )
        {
            root->expressionResult.nResultLong = nLongLeft * nLongRight;
        }
        else if ( nCase == CASE_L_LONG_R_FLOAT )
        {
            root->expressionResult.nResultFloat = nLongLeft * nFloatRight;
        }
        else if ( nCase == CASE_L_FLOAT_R_LONG )
        {
            root->expressionResult.nResultFloat = nFloatLeft * nLongRight;
        }
        else if ( nCase == CASE_L_FLOAT_R_FLOAT )
        {
            root->expressionResult.nResultFloat = nFloatLeft * nFloatRight;
        }
        break;
    case OPERATOR_DEVIDE:
        if ( nCase == CASE_L_LONG_R_LONG )
        {
            root->expressionResult.nResultLong = nLongLeft / nLongRight;
        }
        else if ( nCase == CASE_L_LONG_R_FLOAT )
        {
            root->expressionResult.nResultFloat = nLongLeft / nFloatRight;
        }
        else if ( nCase == CASE_L_FLOAT_R_LONG )
        {
            root->expressionResult.nResultFloat = nFloatLeft / nLongRight;
        }
        else if ( nCase == CASE_L_FLOAT_R_FLOAT )
        {
            root->expressionResult.nResultFloat = nFloatLeft / nFloatRight;
        }
        break;
    case COMPARE_EQUALS:
        if ( nCase == CASE_L_LONG_R_LONG )
        {
            if ( nLongLeft == nLongRight )
            {
                root->expressionResult.bResult = true;
            }            
        }
        else if ( nCase == CASE_L_LONG_R_FLOAT )
        {
            if ( nLongLeft == nFloatRight )
            {
                root->expressionResult.bResult = true;
            }            
        }
        else if ( nCase == CASE_L_FLOAT_R_LONG )
        {
            if ( nFloatLeft == nLongRight )
            {
                root->expressionResult.bResult = true;
            }
        }
        else if ( nCase == CASE_L_FLOAT_R_FLOAT )
        {
            if ( nFloatLeft == nFloatRight )
            {
                root->expressionResult.bResult = true;
            }
        }
        break;
    case COMPARE_NOT_EQUALS:
        if ( nCase == CASE_L_LONG_R_LONG )
        {
            if ( nLongLeft != nLongRight )
            {
                root->expressionResult.bResult = true;
            }
        }
        else if ( nCase == CASE_L_LONG_R_FLOAT )
        {
            if ( nLongLeft != nFloatRight )
            {
                root->expressionResult.bResult = true;
            }
        }
        else if ( nCase == CASE_L_FLOAT_R_LONG )
        {
            if ( nFloatLeft != nLongRight )
            {
                root->expressionResult.bResult = true;
            }
        }
        else if ( nCase == CASE_L_FLOAT_R_FLOAT )
        {
            if ( nFloatLeft != nFloatRight )
            {
                root->expressionResult.bResult = true;
            }
        }
        break;
    case COMPARE_GREATER_THAN:
        if ( nCase == CASE_L_LONG_R_LONG )
        {
            if ( nLongLeft > nLongRight )
            {
                root->expressionResult.bResult = true;
            }
        }
        else if ( nCase == CASE_L_LONG_R_FLOAT )
        {
            if ( nLongLeft > nFloatRight )
            {
                root->expressionResult.bResult = true;
            }
        }
        else if ( nCase == CASE_L_FLOAT_R_LONG )
        {
            if ( nFloatLeft > nLongRight )
            {
                root->expressionResult.bResult = true;
            }
        }
        else if ( nCase == CASE_L_FLOAT_R_FLOAT )
        {
            if ( nFloatLeft > nFloatRight )
            {
                root->expressionResult.bResult = true;
            }
        }
        break;
    case COMPARE_GREATER_EQUALS:
        if ( nCase == CASE_L_LONG_R_LONG )
        {
            if ( nLongLeft >= nLongRight )
            {
                root->expressionResult.bResult = true;
            }
        }
        else if ( nCase == CASE_L_LONG_R_FLOAT )
        {
            if ( nLongLeft >= nFloatRight )
            {
                root->expressionResult.bResult = true;
            }
        }
        else if ( nCase == CASE_L_FLOAT_R_LONG )
        {
            if ( nFloatLeft >= nLongRight )
            {
                root->expressionResult.bResult = true;
            }
        }
        else if ( nCase == CASE_L_FLOAT_R_FLOAT )
        {
            if ( nFloatLeft >= nFloatRight )
            {
                root->expressionResult.bResult = true;
            }
        }
        break;
    case COMPARE_LESS_THAN:
        if ( nCase == CASE_L_LONG_R_LONG )
        {
            if ( nLongLeft < nLongRight )
            {
                root->expressionResult.bResult = true;
            }
        }
        else if ( nCase == CASE_L_LONG_R_FLOAT )
        {
            if ( nLongLeft < nFloatRight )
            {
                root->expressionResult.bResult = true;
            }
        }
        else if ( nCase == CASE_L_FLOAT_R_LONG )
        {
            if ( nFloatLeft < nLongRight )
            {
                root->expressionResult.bResult = true;
            }
        }
        else if ( nCase == CASE_L_FLOAT_R_FLOAT )
        {
            if ( nFloatLeft < nFloatRight )
            {
                root->expressionResult.bResult = true;
            }
        }
        break;
    case COMPARE_LESS_EQUALS:
        if ( nCase == CASE_L_LONG_R_LONG )
        {
            if ( nLongLeft <= nLongRight )
            {
                root->expressionResult.bResult = true;
            }
        }
        else if ( nCase == CASE_L_LONG_R_FLOAT )
        {
            if ( nLongLeft <= nFloatRight )
            {
                root->expressionResult.bResult = true;
            }
        }
        else if ( nCase == CASE_L_FLOAT_R_LONG )
        {
            if ( nFloatLeft <= nLongRight )
            {
                root->expressionResult.bResult = true;
            }
        }
        else if ( nCase == CASE_L_FLOAT_R_FLOAT )
        {
            if ( nFloatLeft <= nFloatRight )
            {
                root->expressionResult.bResult = true;
            }
        }
        break;       
    }
    
    if ( root->nType == NODE_COMPARE_OPERATOR )
    {
        root->expressionResult.nResultType = NODE_BOOL;
        root->expressionResult.nResultDetailedType = BOOL_TYPE;
    }
    else if ( root->nType == NODE_CALCULATE_OPERATOR )
    {
        root->expressionResult.bResult = true;
    }   

}


///////////////////////////////////////////////////////////////////////////////
bool ExpressionTree::SetRealPlaceHolderValue( expression_node* root)
{    
    if (root == NULL)
    {
        return true;
    }

    if (root->nType != NODE_PLACE_HOLDER)
    {
        return true;
    }

    bool bRtn = SetRealPlaceHolderValue(root->rightSiblingForMore2funcArgs);
    if (!bRtn)
    {
        return false;
    }

    char szTemp[100];
    memset(&szTemp, 0x00, sizeof(szTemp));
    strncpy(szTemp, root->strVal + 4, strlen(root->strVal) - 4);
    int nIndex = atoi(szTemp) - 1;
    if (nIndex < 0)
    {
        root->expressionResult.bResult = false;        
        return false;
    }

    //$ph1 / :$ph2            
    if ( placeHolderArray [ nIndex ].GetDataType ( ) == NODE_LITERAL )
    {   
        strcpy ( root->expressionResult.strResult, placeHolderArray [ nIndex ].GetStringData ( ) );
    }
    else if ( placeHolderArray [ nIndex ].GetDataType ( ) == NODE_NUMBER )
    {
        if ( placeHolderArray [ nIndex ].GetDetailedDataType ( ) == NUMBER_LONG )
        {
            root->expressionResult.nResultLong = placeHolderArray [ nIndex ].GetNumberLongData ( );
        }
        else if ( placeHolderArray [ nIndex ].GetDetailedDataType ( ) == NUMBER_FLOAT )
        {
            root->expressionResult.nResultFloat = placeHolderArray [ nIndex ].GetNumberFloatData ( );
        }
    }

    root->expressionResult.nResultType = placeHolderArray [ nIndex ].GetDataType ( );
    root->expressionResult.nResultDetailedType = placeHolderArray [ nIndex ].GetDetailedDataType ( );
    
    return true;  
}

///////////////////////////////////////////////////////////////////////////////
void ExpressionTree::EvaluateLogic(expression_node *root, expression_node* pRsltLeft, expression_node* pRsltRight)
{
    if (root->nType == NODE_USER_FUNCTIION) //////////////////////////
    {  
        //20140314//////////////////////////////////////////
        if (pRsltLeft->nType == NODE_PLACE_HOLDER)  //20140314 
        {
            SetRealPlaceHolderValue(pRsltLeft);
        }
        if(pRsltRight->nType == NODE_PLACE_HOLDER)
        {
            SetRealPlaceHolderValue( pRsltRight);
        }
        ///////////////////////////////////////////////////

        if (userFuncs.InvokeUserFunction(root,  pRsltLeft,  pRsltRight ))
        {   
            //TODO remove converting !!!
            if (root->userFuncInfo.nReturnType == FUNC_RTN_STR)
            {
                root->expressionResult.nResultType = NODE_LITERAL;
                root->expressionResult.nResultDetailedType = LITERAL;
                
                root->expressionResult.bResult = true; 
            }
            else if (root->userFuncInfo.nReturnType == FUNC_RTN_NUM)
            {
                root->expressionResult.nResultType = NODE_NUMBER;
                root->expressionResult.nResultDetailedType = root->userFuncInfo.nDetailedReturnType;
                
                root->expressionResult.bResult = true; 
            }
            else if (root->userFuncInfo.nReturnType == FUNC_RTN_BOOL)
            {
                root->expressionResult.nResultType = NODE_BOOL;
                root->expressionResult.nResultDetailedType = BOOL_TYPE;                
            }
        }
        else
        {
            root->expressionResult.bResult = false;
        }
        
    } // if (root->nType == NODE_USER_FUNCTIION)
    else
    {
        if (pRsltLeft->nType != NODE_PLACE_HOLDER && pRsltRight->nType != NODE_PLACE_HOLDER) //20140314 
        {
            if (!CheckCompareError(pRsltLeft, pRsltRight))
            {
                cout << "Error : CheckCompareError" << "\n";            
                root->expressionResult.bResult = false;
                return;
            }
        }

        //////////////////////////////////////////////////////////////////////////////////////
        if ( pRsltLeft->nType == NODE_PLACE_HOLDER || pRsltRight->nType == NODE_PLACE_HOLDER ) //20140314 
        {
            if ( pRsltLeft->nType == NODE_PLACE_HOLDER )  //20140314 
            {
                SetRealPlaceHolderValue ( pRsltLeft );
            }
            if ( pRsltRight->nType == NODE_PLACE_HOLDER )
            {
                SetRealPlaceHolderValue ( pRsltRight );
            }

            if ( pRsltLeft->expressionResult.nResultType == NODE_NUMBER && pRsltRight->expressionResult.nResultType == NODE_NUMBER )
            {
                EvaluateNumericCondition ( root, pRsltLeft, pRsltRight );
                
            }
            else if ( pRsltLeft->expressionResult.nResultType == NODE_LITERAL && pRsltRight->expressionResult.nResultType == NODE_LITERAL )
            {
                EvaluateStringCondition ( root, pRsltLeft, pRsltRight );
            }

        }
        //////////////////////////////////////////////////////
        else
        {
            if ( pRsltLeft->expressionResult.nResultType == NODE_LITERAL && pRsltRight->expressionResult.nResultType == NODE_LITERAL )
            {
                //if expression is '12'='1'+'2' , strip off both side's single quotations..
                if ( pRsltLeft->strVal [ 0 ] == CHAR_SINGLE_QUOTATION &&
                     pRsltLeft->strVal [ strlen ( pRsltLeft->strVal ) - 1 ] == CHAR_SINGLE_QUOTATION )
                {
                    userFuncs.GetStringSingleQuotationsBothSidesRemoved ( pRsltLeft->strVal );
                    userFuncs.GetStringSingleQuotationsBothSidesRemoved ( pRsltLeft->expressionResult.strResult );
                }
                if ( pRsltRight->strVal [ 0 ] == CHAR_SINGLE_QUOTATION &&
                     pRsltRight->strVal [ strlen ( pRsltRight->strVal ) - 1 ] == CHAR_SINGLE_QUOTATION )
                {
                    userFuncs.GetStringSingleQuotationsBothSidesRemoved ( pRsltRight->strVal );
                    userFuncs.GetStringSingleQuotationsBothSidesRemoved ( pRsltRight->expressionResult.strResult );
                }

                EvaluateStringCondition ( root, pRsltLeft, pRsltRight );
            }
            else if ( pRsltLeft->expressionResult.nResultType == NODE_NUMBER && pRsltRight->expressionResult.nResultType == NODE_NUMBER )
            {
                EvaluateNumericCondition ( root, pRsltLeft, pRsltRight );
            }
            else if ( pRsltLeft->expressionResult.nResultType == NODE_BOOL && pRsltRight->expressionResult.nResultType == NODE_BOOL )
            {
                EvaluateBoolCondition ( root, pRsltLeft, pRsltRight );
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
int ExpressionTree::GetDepth()
{
    int nDepth =  GetDepth(root_node);
    return nDepth;
}

///////////////////////////////////////////////////////////////////////////////
int ExpressionTree::GetDepth(expression_node* root)
{   
    /* TODO
    int nLeftDepth, nRightDepth, nRightDepth2, nRightDepthReal;

    if (root == NULL)
    {
        return 0;
    }
    else
    {
        nLeftDepth = GetDepth(root->left);
        
        nRightDepth2 = GetDepth(root->rightSiblingForMore2funcArgs);
        if (root->right)
        {
            nRightDepth2 += GetDepth(root->right->rightSiblingForMore2funcArgs);
        }

        nRightDepth = GetDepth(root->right);

        nRightDepthReal = nRightDepth > nRightDepth2 ? nRightDepth : nRightDepth2;
        
        if (nLeftDepth > nRightDepthReal)
        {
            return nLeftDepth + 1;
        }
        else
        {
            return nRightDepthReal + 1;
        }
    }
    */

    return 0;
}

///////////////////////////////////////////////////////////////////////////////
void ExpressionTree::EvaluateConditionNonRecursive(expression_node *root)
{
    stack<expression_node*> s;

    while (root != NULL || !s.empty())
    {
        if (root != NULL) //left node push
        {
            s.push(root);
            root = root->left;
            continue;
        }

        while (!s.empty() && s.top()->right == root) //right node pop
        {
            root = s.top();
            s.pop();
           
            if (root->left && root->right)
            {  
                EvaluateConditionNonRecursive(root->right->rightSiblingForMore2funcArgs);
                EvaluateConditionNonRecursive(root->rightSiblingForMore2funcArgs);
               
                EvaluateLogic(root, root->left, root->right );
                
            }           
        }

        root = s.empty() ? NULL : s.top()->right;
    }
}

///////////////////////////////////////////////////////////////////////////////
bool ExpressionTree::EvaluateExpression()
{        
    EvaluateConditionNonRecursive(root_node); 
                
    return root_node->expressionResult.bResult;
}

///////////////////////////////////////////////////////////////////////////////
expression_result* ExpressionTree::GetResult()
{    
    return & root_node->expressionResult; 
}

///////////////////////////////////////////////////////////////////////////////
bool ExpressionTree::SetNumberFloatValueOfPlaceHolder(int nPos, float nVal)   //using placeholder 20140314
{
    if ( nPos - 1 >= MAX_PLACE_HOLDER  )
    {
        return false;
    }
    placeHolderArray [ nPos - 1 ].SetDataType ( NODE_NUMBER, NUMBER_FLOAT );
    placeHolderArray [ nPos - 1 ].SetNumberFloatData ( nVal );
    
    return true;
}


///////////////////////////////////////////////////////////////////////////////
bool ExpressionTree::SetNumberLongValueOfPlaceHolder(int nPos, long nVal)   //using placeholder 20140314
{
    if ( nPos - 1 >= MAX_PLACE_HOLDER  )
    {
        return false;
    }

    placeHolderArray [ nPos - 1 ].SetDataType ( NODE_NUMBER, NUMBER_LONG );
    placeHolderArray [ nPos - 1 ].SetNumberLongData ( nVal );

    return true;
}


///////////////////////////////////////////////////////////////////////////////
bool ExpressionTree::SetStringValueOfPlaceHolder(int nPos, const char* strVal) //using placeholder
{
    if ( nPos - 1 >= MAX_PLACE_HOLDER  )
    {
        return false;
    }

    placeHolderArray [ nPos - 1 ].SetDataType(NODE_LITERAL, LITERAL);
    placeHolderArray [ nPos - 1 ].SetStringData ( strVal );
    
    return true;
}

///////////////////////////////////////////////////////////////////////////////
bool ExpressionTree::BuildExpressionTree()
{    
    nDepthOfTree = 0;    
    DeleteExpressionTree(root_node);

    stack<expression_node*> treeNodeStack;
    stack<expression_node*> funcArgsNodeStack;

    for (vector<ItemTokenInfo>::iterator it = vecPostfixResult.begin(); it != vecPostfixResult.end(); ++it)
    {   
        //-------------------------------------------------------
        if (IsUserFunction(&(*it)))
        {            
            int nArgCnt = it->userFuncInfo.ntotalInputArgCnt;
                                    
            if (nArgCnt == 0)
            {
                expression_node* node = CreateTreeNode();
                expression_node* nodeEmptyR = CreateTreeNode(); //empty node
                expression_node* nodeEmptyL = CreateTreeNode(); //empty node   
                nodeEmptyR->nType = NODE_EMPTY;
                nodeEmptyL->nType = NODE_EMPTY;
                                
                node->right = nodeEmptyR;
                node->left = nodeEmptyL;
                
                node->nType = it->nType;
                node->nDetailedType = it->nDetailedType;
                node->expressionResult.nResultType = it->nType;
                node->expressionResult.nResultDetailedType = it->nDetailedType;

                memcpy(&node->userFuncInfo, &it->userFuncInfo, sizeof(node->userFuncInfo));
                treeNodeStack.push(node);
            }
            else if (nArgCnt == 1)
            {
                int nArgType = it->userFuncInfo.nFuncArgsTypes[0];
                if (treeNodeStack.empty()) //20140314
                {                    
                    cout << "ERROR : Wrong Expression!!\n";
                    return false;
                }
                expression_node* nodeStack = treeNodeStack.top(); 
                                
                if (FUNC_ARG_STR == nArgType)
                {
                    userFuncs.GetStringSingleQuotationsBothSidesRemoved(nodeStack->strVal);
                    userFuncs.GetStringSingleQuotationsBothSidesRemoved ( nodeStack->expressionResult.strResult  );
                }
                
                treeNodeStack.pop();

                expression_node* nodeEmpty = CreateTreeNode(); //empty node   
                nodeEmpty->nType = NODE_EMPTY;
                expression_node* node = CreateTreeNode();                
                node->left = nodeStack ;
                node->right = nodeEmpty;
                
                node->nType = it->nType;
                node->nDetailedType = it->nDetailedType;
                node->expressionResult.nResultType = it->nType;
                node->expressionResult.nResultDetailedType = it->nDetailedType;
                
                memcpy(&node->userFuncInfo, &it->userFuncInfo, sizeof(node->userFuncInfo));
                treeNodeStack.push(node);
            }
            else if (nArgCnt == 2)
            {                
                int nArgType = it->userFuncInfo.nFuncArgsTypes[1];  // 1
                if (treeNodeStack.empty()) //20140314
                {
                    //Error!!!
                    cout << "ERROR : Wrong Expression!!\n";
                    return false;
                }
                expression_node* nodeArgR = treeNodeStack.top();
                
                if (FUNC_ARG_STR == nArgType)
                {
                    userFuncs.GetStringSingleQuotationsBothSidesRemoved(nodeArgR->strVal);
                    userFuncs.GetStringSingleQuotationsBothSidesRemoved ( nodeArgR->expressionResult.strResult );
                }
                treeNodeStack.pop();

                nArgType = it->userFuncInfo.nFuncArgsTypes[0];  
                if (treeNodeStack.empty()) //20140314
                {
                    //Error!!!
                    cout << "ERROR : Wrong Expression!!\n";
                    return false;
                }
                expression_node* nodeArgL = treeNodeStack.top();                
                if (FUNC_ARG_STR == nArgType)
                {
                    userFuncs.GetStringSingleQuotationsBothSidesRemoved(nodeArgL->strVal);
                    userFuncs.GetStringSingleQuotationsBothSidesRemoved ( nodeArgL->expressionResult.strResult );
                }                
                treeNodeStack.pop();
                
                expression_node* node = CreateTreeNode();
                node->right = nodeArgR; //스택에서 먼저 나온것이 오른쪽. 
                node->left = nodeArgL;
                
                node->nType = it->nType;
                node->nDetailedType = it->nDetailedType;
                node->expressionResult.nResultType = it->nType;
                node->expressionResult.nResultDetailedType = it->nDetailedType;

                memcpy(&node->userFuncInfo, &it->userFuncInfo, sizeof(node->userFuncInfo));
                treeNodeStack.push(node);
            }
            else if (nArgCnt > 2)
            {                                   
                expression_node* node = CreateTreeNode();   
                expression_node* nodeRight = NULL;
                                                
                for (int i = 0; i < nArgCnt; i++)
                {
                    int nArgType = it->userFuncInfo.nFuncArgsTypes[nArgCnt -i -1]; //args type XXX 
                    
                    //'1','2','3' 경우 '3','2',1' 순서로 pop
                    //         StrCat3 
                    //   '1'             '2','3'

                    if (i == nArgCnt - 1) // last pop -> left
                    {                        
                        if (treeNodeStack.empty()) //20140314
                        {
                            //Error!!!
                            cout << "ERROR : Wrong Expression!!\n";
                            return false;
                        }
                        expression_node* nodeLeft = treeNodeStack.top();
                        treeNodeStack.pop();                        
                        if (FUNC_ARG_STR == nArgType)
                        {
                            userFuncs.GetStringSingleQuotationsBothSidesRemoved(nodeLeft->strVal);
                            userFuncs.GetStringSingleQuotationsBothSidesRemoved ( nodeLeft->expressionResult.strResult );
                        }
                        node->left = nodeLeft;
                    }
                    else if (i == nArgCnt - 2) // right
                    {                    
                        if (treeNodeStack.empty()) //20140314
                        {
                            //Error!!!
                            cout << "ERROR : Wrong Expression!!\n";
                            return false;
                        }
                        nodeRight = treeNodeStack.top();
                        treeNodeStack.pop();                        
                        if (FUNC_ARG_STR == nArgType)
                        {
                            userFuncs.GetStringSingleQuotationsBothSidesRemoved(nodeRight->strVal);
                            userFuncs.GetStringSingleQuotationsBothSidesRemoved ( nodeRight->expressionResult.strResult );
                        }
                    }
                    else
                    {
                        if (treeNodeStack.empty()) //20140314
                        {
                            //Error!!!
                            cout << "ERROR : Wrong Expression!!\n";
                            return false;
                        }
                        expression_node* nodeForMore2Args = treeNodeStack.top();
                        treeNodeStack.pop();                        
                        if (FUNC_ARG_STR == nArgType)
                        {
                            userFuncs.GetStringSingleQuotationsBothSidesRemoved(nodeForMore2Args->strVal);
                            userFuncs.GetStringSingleQuotationsBothSidesRemoved ( nodeForMore2Args->expressionResult.strResult );
                        }
                        funcArgsNodeStack.push(nodeForMore2Args);
                    }
                   
                }//for
                                
                expression_node* nodePosForFuncArgs = NULL;
                while (!funcArgsNodeStack.empty() ) 
                {
                    if (nodePosForFuncArgs == NULL)
                    {
                        nodePosForFuncArgs = nodeRight->rightSiblingForMore2funcArgs = funcArgsNodeStack.top();
                    }
                    else
                    {
                        nodePosForFuncArgs->rightSiblingForMore2funcArgs = CreateTreeNode();
                        nodePosForFuncArgs->rightSiblingForMore2funcArgs = funcArgsNodeStack.top();
                        nodePosForFuncArgs = nodePosForFuncArgs->rightSiblingForMore2funcArgs;
                    }

                    funcArgsNodeStack.pop();
                }
                                
                node->right = nodeRight;
                
                node->nType = it->nType;
                node->nDetailedType = it->nDetailedType;
                node->expressionResult.nResultType = it->nType;
                node->expressionResult.nResultDetailedType = it->nDetailedType;

                memcpy(&node->userFuncInfo, &it->userFuncInfo, sizeof(node->userFuncInfo));
                treeNodeStack.push(node);
                
            }   
        }
        //-------------------------------------------------------
        else if (IsOperand(&(*it)))
        {
            expression_node* node = CreateTreeNode();
            
            if (NODE_NUMBER == it->nType)
            {                
                if (NUMBER_LONG == it->nDetailedType)
                {
                    node-> nLongValue = atol(it->strValue);
                    node->expressionResult.nResultLong = node->nLongValue;
                }
                else if (NUMBER_FLOAT == it->nDetailedType)
                {
                    node-> nFloatValue = (float)atof(it->strValue);
                    node->expressionResult.nResultFloat = node->nFloatValue;
                }
                else
                {
                    cout << "Error \n";                    
                }                
            }
            else 
            {
                memcpy(&node->strVal, it->strValue, sizeof(node->strVal));
                memcpy ( &node->expressionResult.strResult, it->strValue, sizeof( node->expressionResult.strResult ) );
            }
            
            node->nType = it->nType;
            node->nDetailedType = it->nDetailedType;
            node->expressionResult.nResultType = it->nType;
            node->expressionResult.nResultDetailedType = it->nDetailedType;
                        
            treeNodeStack.push(node);
        }
        //-------------------------------------------------------
        else if (IsOperator(&(*it)))
        {
            if (treeNodeStack.empty()) //2-14-314
            {                
                return false;
            }
            
            expression_node* node1 = treeNodeStack.top();
            treeNodeStack.pop();

            if (treeNodeStack.empty()) //2-14-314
            {                
                return false;
            }

            expression_node* node2 = treeNodeStack.top();
            treeNodeStack.pop();

            expression_node* node = CreateTreeNode();
            node->right = node1;  
            node->left = node2;

            node->nType = it->nType;
            node->nDetailedType = it->nDetailedType;
            node->expressionResult.nResultType = it->nType;
            node->expressionResult.nResultDetailedType = it->nDetailedType;

            treeNodeStack.push(node);
        }
        else
        {
            cout << "-- Not Operator/Operand" << "\n";
        }
    }

    if (treeNodeStack.empty()) //20140314
    {        
        cout << "ERROR : Wrong Expression!!\n";
        return false;
    }
    root_node = treeNodeStack.top();
    treeNodeStack.pop();

    return true;
}


///////////////////////////////////////////////////////////////////////////////
void ExpressionTree::DeleteExpressionTree(expression_node* root)
{       
    /*
    if (root == NULL)
    {
        return;
    }
  
    if (root->right && root->right->rightSiblingForMore2funcArgs)
    {
        DeleteExpressionTree(root->right->rightSiblingForMore2funcArgs);
    }
        
    if (root->left)
    {
        DeleteExpressionTree(root->left);
    }
        
    if (root->right)
    {
        DeleteExpressionTree(root->right);
    }   
       
    //cout << "Delete: nType=" << NodeTypeDesc[root->nType] << " / nDetailedType=" << DetailedTypeDesc[root->nDetailedType] << "/" << root->expressionResult.strResult << "/" << root->expressionResult.nResultLong << "\n";
    delete root;
    root = NULL; 
    */

    while (!treeNodeMemRepository.empty() )
    {
        expression_node* pDelete = treeNodeMemRepository.top();
        treeNodeMemRepository.pop();

        delete pDelete;
        pDelete = NULL;

        --memStatus;
    }
    
}
