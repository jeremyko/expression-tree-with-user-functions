
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
    "NUMBER"
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
    //vecPostFix
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
    if (pOpRsltLeft->nType == pOpRsltRight->nType )
    {
        return true;
    }

    if (pOpRsltLeft->nType == NODE_USER_FUNCTIION && pOpRsltRight->nType == NODE_USER_FUNCTIION)
    {
        //both user function
        if (pOpRsltLeft->userFuncInfo.nReturnType != pOpRsltRight->userFuncInfo.nReturnType)
        {
            return false;
        }
    }
    else if (pOpRsltLeft->nType != NODE_USER_FUNCTIION && pOpRsltRight->nType != NODE_USER_FUNCTIION)
    {
        //cout << "*** Error data type is different!! ***\n";
        //cout << "- Left: nType=" << NodeTypeDesc[pOpRsltLeft->nType] << " / nDetailedType=" << DetailedTypeDesc[pOpRsltLeft->nDetailedType] << "\n";
        //cout << "- Right:nType=" << NodeTypeDesc[pOpRsltRight->nType] << " / nDetailedType=" << DetailedTypeDesc[pOpRsltRight->nDetailedType] << "\n";
        return false;
    }
    else if (pOpRsltLeft->nType == NODE_USER_FUNCTIION)
    {
        bool bError = true;
        if (pOpRsltLeft->userFuncInfo.nReturnType == FUNC_RTN_STR && pOpRsltRight->nType == NODE_LITERAL)
        {
            bError = false;
        }
        else if (pOpRsltLeft->userFuncInfo.nReturnType == FUNC_RTN_NUM && pOpRsltRight->nType == NODE_NUMBER)
        {
            bError = false;
        }
        if (bError)
        {
            //cout << "*** Error data type is different!! ***\n";
            //cout << "- Left: nType=" << NodeTypeDesc[pOpRsltLeft->nType] << " / nDetailedType=" << DetailedTypeDesc[pOpRsltLeft->nDetailedType] << "\n";
            //cout << "- Right:nType=" << NodeTypeDesc[pOpRsltRight->nType] << " / nDetailedType=" << DetailedTypeDesc[pOpRsltRight->nDetailedType] << "\n";
            return false;
        }
    }
    else if (pOpRsltRight->nType == NODE_USER_FUNCTIION)
    {
        bool bError = true;
        if (pOpRsltRight->userFuncInfo.nReturnType == FUNC_RTN_STR && pOpRsltLeft->nType == NODE_LITERAL)
        {
            bError = false;
        }
        else if (pOpRsltRight->userFuncInfo.nReturnType == FUNC_RTN_NUM && pOpRsltLeft->nType == NODE_NUMBER)
        {
            bError = false;
        }
        if (bError)
        {
            //cout << "*** Error data type is different!! ***\n";
            //cout << "- Left: nType=" << NodeTypeDesc[pOpRsltLeft->nType] << " / nDetailedType=" << DetailedTypeDesc[pOpRsltLeft->nDetailedType] << "\n";
            //cout << "- Right:nType=" << NodeTypeDesc[pOpRsltRight->nType] << " / nDetailedType=" << DetailedTypeDesc[pOpRsltRight->nDetailedType] << "\n";
            return false;
        }
    }

    return true;
}

///////////////////////////////////////////////////////////////////////////////
void ExpressionTree::EvaluateStringCondition(
    expression_node *root,
    expression_node* pRsltLeft,
    expression_node* pRsltRight    )
{
    switch (root->nDetailedType)
    {
    case OPERATOR_PLUS:
        strcat(root->strVal, pRsltLeft->strVal);
        strcat(root->strVal, pRsltRight->strVal);
        break;
    case COMPARE_EQUALS:
        if (strcmp(pRsltLeft->strVal, pRsltRight->strVal) == 0)  { root->opReslut = true; }
        else { root->opReslut = false; }
        break;

    case COMPARE_NOT_EQUALS:
        if (strcmp(pRsltLeft->strVal, pRsltRight->strVal) != 0) { root->opReslut = true; }
        else { root->opReslut = false; }
        break;

    case COMPARE_GREATER_THAN:
        if (strcmp(pRsltLeft->strVal, pRsltRight->strVal) > 0) { root->opReslut = true; }
        else { root->opReslut = false; }
        break;

    case COMPARE_GREATER_EQUALS:
        if (strcmp(pRsltLeft->strVal, pRsltRight->strVal) >= 0) { root->opReslut = true; }
        else { root->opReslut = false; }
        break;

    case COMPARE_LESS_THAN:
        if (strcmp(pRsltLeft->strVal, pRsltRight->strVal) < 0) { root->opReslut = true; }
        else { root->opReslut = false; }
        break;

    case COMPARE_LESS_EQUALS:
        if (strcmp(pRsltLeft->strVal, pRsltRight->strVal) <= 0) { root->opReslut = true; }
        else { root->opReslut = false; }
        break;
    case COMPARE_AND:
        if (pRsltLeft->opReslut && pRsltRight->opReslut) { root->opReslut = true; }
        else{ root->opReslut = false; }
        break;
    case COMPARE_OR:
        if (pRsltLeft->opReslut || pRsltRight->opReslut) { root->opReslut = true; }
        else{ root->opReslut = false; }
        break;
    }

    if (root->nDetailedType == OPERATOR_PLUS)
    {
        root->opReslut = true;
    }
}

///////////////////////////////////////////////////////////////////////////////
void ExpressionTree::EvaluateNumericCondition(
    expression_node *root,
    expression_node* pRsltLeft,
    expression_node* pRsltRight  )
{
    switch (root->nDetailedType)
    {
    case OPERATOR_PLUS:
        if (pRsltLeft->nDetailedType == NUMBER_LONG && pRsltRight->nDetailedType == NUMBER_LONG)
        {
            root->variable.nLongValue = pRsltLeft->variable.nLongValue + pRsltRight->variable.nLongValue;
        }
        else if (pRsltLeft->nDetailedType == NUMBER_FLOAT && pRsltRight->nDetailedType == NUMBER_FLOAT)
        {
            root->variable.nFloatValue = pRsltLeft->variable.nFloatValue + pRsltRight->variable.nFloatValue;
        }
        else if (pRsltLeft->nDetailedType == NUMBER_FLOAT && pRsltRight->nDetailedType == NUMBER_LONG)
        {
            root->variable.nFloatValue = pRsltLeft->variable.nFloatValue + pRsltRight->variable.nLongValue;
        }
        else if (pRsltLeft->nDetailedType == NUMBER_LONG && pRsltRight->nDetailedType == NUMBER_FLOAT)
        {
            root->variable.nFloatValue = pRsltLeft->variable.nLongValue + pRsltRight->variable.nFloatValue;
        }

        break;
    case OPERATOR_MINUS:
        if (pRsltLeft->nDetailedType == NUMBER_LONG && pRsltRight->nDetailedType == NUMBER_LONG)
        {
            root->variable.nLongValue = pRsltLeft->variable.nLongValue - pRsltRight->variable.nLongValue;
        }
        else if (pRsltLeft->nDetailedType == NUMBER_FLOAT && pRsltRight->nDetailedType == NUMBER_FLOAT)
        {
            root->variable.nFloatValue = pRsltLeft->variable.nFloatValue - pRsltRight->variable.nFloatValue;
        }
        else if (pRsltLeft->nDetailedType == NUMBER_FLOAT && pRsltRight->nDetailedType == NUMBER_LONG)
        {
            root->variable.nFloatValue = pRsltLeft->variable.nFloatValue - pRsltRight->variable.nLongValue;
        }
        else if (pRsltLeft->nDetailedType == NUMBER_LONG && pRsltRight->nDetailedType == NUMBER_FLOAT)
        {
            root->variable.nFloatValue = pRsltLeft->variable.nLongValue - pRsltRight->variable.nFloatValue;
        }

        break;

    case OPERATOR_TIMES:
        if (pRsltLeft->nDetailedType == NUMBER_LONG && pRsltRight->nDetailedType == NUMBER_LONG)
        {
            root->variable.nLongValue = pRsltLeft->variable.nLongValue * pRsltRight->variable.nLongValue;
        }
        else if (pRsltLeft->nDetailedType == NUMBER_FLOAT && pRsltRight->nDetailedType == NUMBER_FLOAT)
        {
            root->variable.nFloatValue = pRsltLeft->variable.nFloatValue * pRsltRight->variable.nFloatValue;
        }
        else if (pRsltLeft->nDetailedType == NUMBER_FLOAT && pRsltRight->nDetailedType == NUMBER_LONG)
        {
            root->variable.nFloatValue = pRsltLeft->variable.nFloatValue * pRsltRight->variable.nLongValue;
        }
        else if (pRsltLeft->nDetailedType == NUMBER_LONG && pRsltRight->nDetailedType == NUMBER_FLOAT)
        {
            root->variable.nFloatValue = pRsltLeft->variable.nLongValue * pRsltRight->variable.nFloatValue;
        }

        break;

    case OPERATOR_DEVIDE:
        if (pRsltLeft->nDetailedType == NUMBER_LONG && pRsltRight->nDetailedType == NUMBER_LONG)
        {
            root->variable.nLongValue = pRsltLeft->variable.nLongValue / pRsltRight->variable.nLongValue;
        }
        else if (pRsltLeft->nDetailedType == NUMBER_FLOAT && pRsltRight->nDetailedType == NUMBER_FLOAT)
        {
            root->variable.nFloatValue = pRsltLeft->variable.nFloatValue / pRsltRight->variable.nFloatValue;
        }
        else if (pRsltLeft->nDetailedType == NUMBER_FLOAT && pRsltRight->nDetailedType == NUMBER_LONG)
        {
            root->variable.nFloatValue = pRsltLeft->variable.nFloatValue / pRsltRight->variable.nLongValue;
        }
        else if (pRsltLeft->nDetailedType == NUMBER_LONG && pRsltRight->nDetailedType == NUMBER_FLOAT)
        {
            root->variable.nFloatValue = pRsltLeft->variable.nLongValue / pRsltRight->variable.nFloatValue;
        }

        break;

    case COMPARE_EQUALS:
        if (pRsltLeft->nDetailedType == NUMBER_LONG && pRsltRight->nDetailedType == NUMBER_LONG)
        {
            if (pRsltLeft->variable.nLongValue == pRsltRight->variable.nLongValue)  { root->opReslut = true; }
            else { root->opReslut = false; }
        }
        else if (pRsltLeft->nDetailedType == NUMBER_FLOAT && pRsltRight->nDetailedType == NUMBER_FLOAT)
        {
            if (pRsltLeft->variable.nFloatValue == pRsltRight->variable.nFloatValue)  { root->opReslut = true; }
            else { root->opReslut = false; }
        }
        else if (pRsltLeft->nDetailedType == NUMBER_FLOAT && pRsltRight->nDetailedType == NUMBER_LONG)
        {
            if (pRsltLeft->variable.nFloatValue == pRsltRight->variable.nLongValue)  { root->opReslut = true; }
            else { root->opReslut = false; }
        }
        else if (pRsltLeft->nDetailedType == NUMBER_LONG && pRsltRight->nDetailedType == NUMBER_FLOAT)
        {
            if (pRsltLeft->variable.nLongValue == pRsltRight->variable.nFloatValue)  { root->opReslut = true; }
            else { root->opReslut = false; }
        }

        break;

    case COMPARE_NOT_EQUALS:
        if (pRsltLeft->nDetailedType == NUMBER_LONG && pRsltRight->nDetailedType == NUMBER_LONG)
        {
            if (pRsltLeft->variable.nLongValue != pRsltRight->variable.nLongValue)  { root->opReslut = true; }
            else { root->opReslut = false; }
        }
        else if (pRsltLeft->nDetailedType == NUMBER_FLOAT && pRsltRight->nDetailedType == NUMBER_FLOAT)
        {
            if (pRsltLeft->variable.nFloatValue != pRsltRight->variable.nFloatValue)  { root->opReslut = true; }
            else { root->opReslut = false; }
        }
        else if (pRsltLeft->nDetailedType == NUMBER_FLOAT && pRsltRight->nDetailedType == NUMBER_LONG)
        {
            if (pRsltLeft->variable.nFloatValue != pRsltRight->variable.nLongValue)  { root->opReslut = true; }
            else { root->opReslut = false; }
        }
        else if (pRsltLeft->nDetailedType == NUMBER_LONG && pRsltRight->nDetailedType == NUMBER_FLOAT)
        {
            if (pRsltLeft->variable.nLongValue != pRsltRight->variable.nFloatValue)  { root->opReslut = true; }
            else { root->opReslut = false; }
        }
        break;

    case COMPARE_GREATER_THAN:
        if (pRsltLeft->nDetailedType == NUMBER_LONG && pRsltRight->nDetailedType == NUMBER_LONG)
        {
            if (pRsltLeft->variable.nLongValue > pRsltRight->variable.nLongValue)  { root->opReslut = true; }
            else { root->opReslut = false; }
        }
        else if (pRsltLeft->nDetailedType == NUMBER_FLOAT && pRsltRight->nDetailedType == NUMBER_FLOAT)
        {
            if (pRsltLeft->variable.nFloatValue > pRsltRight->variable.nFloatValue)  { root->opReslut = true; }
            else { root->opReslut = false; }
        }
        else if (pRsltLeft->nDetailedType == NUMBER_FLOAT && pRsltRight->nDetailedType == NUMBER_LONG)
        {
            if (pRsltLeft->variable.nFloatValue > pRsltRight->variable.nLongValue)  { root->opReslut = true; }
            else { root->opReslut = false; }
        }
        else if (pRsltLeft->nDetailedType == NUMBER_LONG && pRsltRight->nDetailedType == NUMBER_FLOAT)
        {
            if (pRsltLeft->variable.nLongValue > pRsltRight->variable.nFloatValue)  { root->opReslut = true; }
            else { root->opReslut = false; }
        }
        break;

    case COMPARE_GREATER_EQUALS:
        if (pRsltLeft->nDetailedType == NUMBER_LONG && pRsltRight->nDetailedType == NUMBER_LONG)
        {
            if (pRsltLeft->variable.nLongValue >= pRsltRight->variable.nLongValue)  { root->opReslut = true; }
            else { root->opReslut = false; }
        }
        else if (pRsltLeft->nDetailedType == NUMBER_FLOAT && pRsltRight->nDetailedType == NUMBER_FLOAT)
        {
            if (pRsltLeft->variable.nFloatValue >= pRsltRight->variable.nFloatValue)  { root->opReslut = true; }
            else { root->opReslut = false; }
        }
        else if (pRsltLeft->nDetailedType == NUMBER_FLOAT && pRsltRight->nDetailedType == NUMBER_LONG)
        {
            if (pRsltLeft->variable.nFloatValue >= pRsltRight->variable.nLongValue)  { root->opReslut = true; }
            else { root->opReslut = false; }
        }
        else if (pRsltLeft->nDetailedType == NUMBER_LONG && pRsltRight->nDetailedType == NUMBER_FLOAT)
        {
            if (pRsltLeft->variable.nLongValue >= pRsltRight->variable.nFloatValue)  { root->opReslut = true; }
            else { root->opReslut = false; }
        }
        break;

    case COMPARE_LESS_THAN:
        if (pRsltLeft->nDetailedType == NUMBER_LONG && pRsltRight->nDetailedType == NUMBER_LONG)
        {
            if (pRsltLeft->variable.nLongValue < pRsltRight->variable.nLongValue)  { root->opReslut = true; }
            else { root->opReslut = false; }
        }
        else if (pRsltLeft->nDetailedType == NUMBER_FLOAT && pRsltRight->nDetailedType == NUMBER_FLOAT)
        {
            if (pRsltLeft->variable.nFloatValue < pRsltRight->variable.nFloatValue)  { root->opReslut = true; }
            else { root->opReslut = false; }
        }
        else if (pRsltLeft->nDetailedType == NUMBER_FLOAT && pRsltRight->nDetailedType == NUMBER_LONG)
        {
            if (pRsltLeft->variable.nFloatValue < pRsltRight->variable.nLongValue)  { root->opReslut = true; }
            else { root->opReslut = false; }
        }
        else if (pRsltLeft->nDetailedType == NUMBER_LONG && pRsltRight->nDetailedType == NUMBER_FLOAT)
        {
            if (pRsltLeft->variable.nLongValue < pRsltRight->variable.nFloatValue)  { root->opReslut = true; }
            else { root->opReslut = false; }
        }
        break;

    case COMPARE_LESS_EQUALS:
        if (pRsltLeft->nDetailedType == NUMBER_LONG && pRsltRight->nDetailedType == NUMBER_LONG)
        {
            if (pRsltLeft->variable.nLongValue <= pRsltRight->variable.nLongValue)  { root->opReslut = true; }
            else { root->opReslut = false; }
        }
        else if (pRsltLeft->nDetailedType == NUMBER_FLOAT && pRsltRight->nDetailedType == NUMBER_FLOAT)
        {
            if (pRsltLeft->variable.nFloatValue <= pRsltRight->variable.nFloatValue)  { root->opReslut = true; }
            else { root->opReslut = false; }
        }
        else if (pRsltLeft->nDetailedType == NUMBER_FLOAT && pRsltRight->nDetailedType == NUMBER_LONG)
        {
            if (pRsltLeft->variable.nFloatValue <= pRsltRight->variable.nLongValue)  { root->opReslut = true; }
            else { root->opReslut = false; }
        }
        else if (pRsltLeft->nDetailedType == NUMBER_LONG && pRsltRight->nDetailedType == NUMBER_FLOAT)
        {
            if (pRsltLeft->variable.nLongValue <= pRsltRight->variable.nFloatValue)  { root->opReslut = true; }
            else { root->opReslut = false; }
        }
        break;
    case COMPARE_AND:
        if (pRsltLeft->opReslut && pRsltRight->opReslut) { root->opReslut = true; }
        else{ root->opReslut = false; }
        break;
    case COMPARE_OR:
        if (pRsltLeft->opReslut || pRsltRight->opReslut) { root->opReslut = true; }
        else{ root->opReslut = false; }
        break;
    }

    if (root->nDetailedType == OPERATOR_PLUS ||
        root->nDetailedType == OPERATOR_MINUS ||
        root->nDetailedType == OPERATOR_TIMES ||
        root->nDetailedType == OPERATOR_DEVIDE
       )
    {
        root->opReslut = true;
    }
}

///////////////////////////////////////////////////////////////////////////////
void ExpressionTree::EvaluateLogic(expression_node *root, expression_node* pRsltLeft, expression_node* pRsltRight)
{
    if (root->nType == NODE_USER_FUNCTIION) //////////////////////////
    {
        if (pRsltLeft->strVal[0] == CHAR_SINGLE_QUOTATION &&
            pRsltLeft->strVal[strlen(pRsltLeft->strVal) - 1] == CHAR_SINGLE_QUOTATION)
        {
            userFuncs.GetStringSingleQuotationsBothSidesRemoved(pRsltLeft->strVal);
        }
        if (pRsltRight->strVal[0] == CHAR_SINGLE_QUOTATION &&
            pRsltRight->strVal[strlen(pRsltRight->strVal) - 1] == CHAR_SINGLE_QUOTATION)
        {
            userFuncs.GetStringSingleQuotationsBothSidesRemoved(pRsltRight->strVal);
        }

        if (userFuncs.InvokeUserFunction(root,  pRsltLeft,  pRsltRight ))
        {
            //TODO remove converting !!!
            if (root->userFuncInfo.nReturnType == FUNC_RTN_STR)
            {
                root->nType = NODE_LITERAL;
                root->nDetailedType = LITERAL;
                root->opReslut = true; //함수 호출 성공을 의미
            }
            else if (root->userFuncInfo.nReturnType == FUNC_RTN_NUM)
            {
                root->nType = NODE_NUMBER;
                root->nDetailedType = root->userFuncInfo.nDetailedReturnType;
                root->opReslut = true; //함수 호출 성공을 의미
            }
            else if (root->userFuncInfo.nReturnType == FUNC_RTN_BOOL)
            {
                root->nType = NODE_BOOL;
                root->nDetailedType = BOOL_TYPE;
            }
        }
        else
        {
            root->opReslut = false;
        }

    } // if (root->nType == NODE_USER_FUNCTIION)
    else
    {
        if (!CheckCompareError(pRsltLeft, pRsltRight))
        {
            //cout << "Error : CheckCompareError" << "\n";
            root->opReslut = false;
            return;
        }

        if (pRsltLeft->nType == NODE_LITERAL && pRsltRight->nType == NODE_LITERAL)
        {
            //if expression is '12'='1'+'2' , strip off both side's single quotations..
            if (pRsltLeft->strVal[0] == CHAR_SINGLE_QUOTATION &&
                pRsltLeft->strVal[strlen(pRsltLeft->strVal) - 1] == CHAR_SINGLE_QUOTATION)
            {
                userFuncs.GetStringSingleQuotationsBothSidesRemoved(pRsltLeft->strVal);
            }
            if (pRsltRight->strVal[0] == CHAR_SINGLE_QUOTATION &&
                pRsltRight->strVal[strlen(pRsltRight->strVal) - 1] == CHAR_SINGLE_QUOTATION)
            {
                userFuncs.GetStringSingleQuotationsBothSidesRemoved(pRsltRight->strVal);
            }

            //root->opReslut = true;
            EvaluateStringCondition(root, pRsltLeft, pRsltRight);

            root->nType = NODE_LITERAL;
            root->nDetailedType = pRsltLeft->nDetailedType;
        }
        else if (pRsltLeft->nType == NODE_NUMBER && pRsltRight->nType == NODE_NUMBER)
        {
            EvaluateNumericCondition(root, pRsltLeft, pRsltRight);
            root->nType = NODE_NUMBER;
            root->nDetailedType = pRsltLeft->nDetailedType;
        }
    }
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
                ///////////////////////////////////////////////////
                EvaluateConditionNonRecursive(root->right->rightSiblingForMore2funcArgs);
                EvaluateConditionNonRecursive(root->rightSiblingForMore2funcArgs);
                ///////////////////////////////////////////////////
                /*
                cout << "--root nType=" << NodeTypeDesc[root->nType] << " / nDetailedType=" << DetailedTypeDesc[root->nDetailedType] << "/" << root->strVal << "/" << root->variable.nLongValue << "\n";
                cout << "--L nType=" << NodeTypeDesc[root->left->nType] << " / nDetailedType=" << DetailedTypeDesc[root->left->nDetailedType]
                     << "/" << root->left->strVal << "/" << root->left->variable.nLongValue << "\n";
                cout << "--R nType=" << NodeTypeDesc[root->right->nType] << " / nDetailedType=" << DetailedTypeDesc[root->right->nDetailedType]
                    << "/" << root->right->strVal << "/" << root->right->variable.nLongValue << "\n";

                if (root->rightSiblingForMore2funcArgs)
                {
                    cout << "--rightSiblingForMore2funcArgs nType=" << NodeTypeDesc[root->rightSiblingForMore2funcArgs->nType] << " / nDetailedType="
                        << DetailedTypeDesc[root->rightSiblingForMore2funcArgs->nDetailedType]
                        << "/" << root->rightSiblingForMore2funcArgs->strVal << "/" << root->rightSiblingForMore2funcArgs->variable.nLongValue << "\n";
                }
                if (root->right->rightSiblingForMore2funcArgs)
                {
                    cout << "--rightSiblingForMore2funcArgs nType=" << NodeTypeDesc[root->right->rightSiblingForMore2funcArgs->nType] << " / nDetailedType="
                        << DetailedTypeDesc[root->right->rightSiblingForMore2funcArgs->nDetailedType]
                        << "/" << root->right->rightSiblingForMore2funcArgs->strVal << "/" << root->right->rightSiblingForMore2funcArgs->variable.nLongValue << "\n";
                }
                */
                EvaluateLogic(root, root->left, root->right );

            }
        }//while (!s.empty() && s.top()->right == root)

        root = s.empty() ? NULL : s.top()->right;
    }
}

///////////////////////////////////////////////////////////////////////////////
bool ExpressionTree::EvaluateExpression()
{
    EvaluateConditionNonRecursive(root_node);

    //cout << "bool reslut   : " << expr_rslt.opReslut << "\n";
    //cout << "numeric reslt : " << expr_rslt.value << "\n";
    //cout << "string result : " << expr_rslt.strVal << "\n";

    return root_node->opReslut;
}

///////////////////////////////////////////////////////////////////////////////
expression_node* ExpressionTree::GetResult()
{
    //return &expr_rslt;
    return root_node;
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
                memcpy(&node->userFuncInfo, &it->userFuncInfo, sizeof(node->userFuncInfo));
                treeNodeStack.push(node);
            }
            else if (nArgCnt == 1)
            {
                int nArgType = it->userFuncInfo.nFuncArgsTypes[0];
                if (treeNodeStack.empty()) //20140314
                {
                    //Error!!!
                    cout << "ERROR : Wrong Expression!!\n";
                    return false;
                }
                expression_node* nodeStack = treeNodeStack.top();

                if (FUNC_ARG_STR == nArgType)
                {
                    userFuncs.GetStringSingleQuotationsBothSidesRemoved(nodeStack->strVal);
                }

                treeNodeStack.pop();

                expression_node* nodeEmpty = CreateTreeNode(); //empty node
                nodeEmpty->nType = NODE_EMPTY;
                expression_node* node = CreateTreeNode();
                node->left = nodeStack ;
                node->right = nodeEmpty;
                node->nType = it->nType;
                node->nDetailedType = it->nDetailedType;
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
                }
                treeNodeStack.pop();

                expression_node* node = CreateTreeNode();
                node->right = nodeArgR; //스택에서 먼저 나온것이 오른쪽.
                node->left = nodeArgL;
                node->nType = it->nType;
                node->nDetailedType = it->nDetailedType;
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
                memcpy(&node->userFuncInfo, &it->userFuncInfo, sizeof(node->userFuncInfo));
                treeNodeStack.push(node);

            }
        }
        else if (IsOperand(&(*it)))
        {
            expression_node* node = CreateTreeNode();

            if (NODE_NUMBER == it->nType)
            {
                if (NUMBER_LONG == it->nDetailedType)
                {
                    node->variable. nLongValue = atol(it->strValue);
                }
                else if (NUMBER_FLOAT == it->nDetailedType)
                {
                    node->variable. nFloatValue = (float)atof(it->strValue);
                }
                else
                {
                    cout << "Error \n";
                }
            }
            else
            {
                memcpy(&node->strVal, it->strValue, sizeof(node->strVal));
            }
            node->nType = it->nType;
            node->nDetailedType = it->nDetailedType;
            treeNodeStack.push(node);
        }
        else if (IsOperator(&(*it)))
        {
            if (treeNodeStack.empty()) //20140314
            {
                //Error!!!
                cout << "ERROR : Wrong Expression!!\n";
                return false;
            }
            expression_node* node1 = treeNodeStack.top();
            treeNodeStack.pop();

            if (treeNodeStack.empty()) //20140314
            {
                //Error!!!
                cout << "ERROR : Wrong Expression!!\n";
                return false;
            }
            expression_node* node2 = treeNodeStack.top();
            treeNodeStack.pop();

            expression_node* node = CreateTreeNode();
            node->right = node1;
            node->left = node2;

            node->nType = it->nType;
            node->nDetailedType = it->nDetailedType;
            treeNodeStack.push(node);
        }
        else
        {
            cout << "-- Not Operator/Operand" << "\n";
        }
    }

    if (treeNodeStack.empty()) //20140314
    {
        //Error!!!
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
     while (!treeNodeMemRepository.empty() )
    {
        expression_node* pDelete = treeNodeMemRepository.top();
        treeNodeMemRepository.pop();

        delete pDelete;
        pDelete = NULL;

        --memStatus;
    }

}

