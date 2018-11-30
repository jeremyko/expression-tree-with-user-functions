
#include<iostream>
#include "expression_tree.h"
#include "user_functions.h"
#include "elapsed_time.hpp"

using namespace std;

///////////////////////////////////////////////////////////////////////////////
void perf_test ()
{
    ExpressionTree expTree;
    bool bRslt = false;

    cout << "\n---------------------------------------\n";

    bRslt = expTree.SetInfixExpression(":$ph1='01071786907'");
    ElapsedTime elapsed;
    if ( bRslt )
    {
        elapsed.SetStartTime();
        for(int i=0; i < 100000; i++)
        {
            expTree.SetStringValueOfPlaceHolder ( 1, "'01071786907'" );

            bRslt = expTree.EvaluateExpression ( );
        }
        std::cout << "elapsed: " << elapsed.SetEndTime(MILLI_SEC_RESOLUTION) << " mili secs\n";
    }   
}
///////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[])
{           
    perf_test();
    return 0; 
}

