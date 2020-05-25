#include <iostream>
#include <armadillo>

#include "GordonNewell.h"

using namespace std;
using namespace arma;

void writeOutput(ofstream& ofstream, const mat& matrix, int K)
{
    ofstream << "////////////////////////////////////////" << endl;
    ofstream << "////////////////" << " K := " << K << " ////////////////" << endl;
    ofstream << "////////////////////////////////////////" << endl;

    ofstream << endl;

    for (int i = 0; i < matrix.n_rows; i++)
        ofstream << "X_" << i + 1 << " = " << matrix(i, 0) << endl;

    ofstream << endl;
}

int main(int argc, char** argv)
{
    ofstream outputStream;
    outputStream.open("potraznje_analiticki.txt");

    for (int k = 2; k <= 8; k++) 
    {
        GordonNewellSolver gn(k);
        mat result = gn.GetXMatrix();

        writeOutput(outputStream, result, k);
    }

    outputStream.close();



    return 0;
}