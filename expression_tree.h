#ifndef	_EXPRESSION_TREE_H_
#define	_EXPRESSION_TREE_H_



#include<iostream>
#include<stack>
#include<vector>
#include <string>
using namespace std;

#include <memory.h>
#include "token_parser.h"
#include "user_functions.h"

#include "expression_tree_common.h"


///////////////////////////////////////////////////////////////////////////////
class ExpressionTree
{
public:
    ExpressionTree();
    ~ExpressionTree();
    
    bool SetInfixExpression(const char* inFix);
    bool EvaluateExpression();    
    expression_node* GetResult();
    int GetDepth();
            
protected:
    int nDepthOfTree;
    int  memStatus;
    int GetDepth( expression_node* root );
    bool IsOperator(ItemTokenInfo* pNodeInfo);
    bool IsOperand(ItemTokenInfo* pNodeInfo);
    bool IsUserFunction(ItemTokenInfo* pNodeInfo);
    int  GetOperatortPriority(ItemTokenInfo* pNodeInfo);
    int  GetOperatorPrecedence(ItemTokenInfo* pNodeInfo1, ItemTokenInfo* pNodeInfo2);
    expression_node* CreateTreeNode();
    bool BuildExpressionTree();
    void DeleteExpressionTree(expression_node* root);       
    bool CheckCompareError(expression_node* opRsltLeft, expression_node* opRsltRight);

    expression_node* root_node;        
    vector<ItemTokenInfo> vecPostfixResult;
    CUserFunctions userFuncs;
                
    void EvaluateNumericCondition(expression_node *root,
        expression_node* pRsltLeft,
        expression_node* pRsltRight );

    void EvaluateStringCondition(expression_node *root,
        expression_node* pRsltLeft,
        expression_node* pRsltRight );
        
    void EvaluateConditionNonRecursive(expression_node *root);
        
    void EvaluateLogic(expression_node *root, 
        expression_node* pRsltLeft, 
        expression_node* pRsltRight);

    stack<expression_node*> treeNodeMemRepository;
        
};

#endif
