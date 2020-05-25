#include <iostream>
#include <armadillo>

#include "GordonNewell.h"

using namespace std;
using namespace arma;

int main(int argc, char** argv)
{
    //for (int k = 2; k <= 8; k++) 
    //{
        GordonNewellSolver gn(2);
        mat result = gn.GetXMatrix();
    //}




    return 0;
}