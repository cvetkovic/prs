#include <iomanip>
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

void writeOutputBuzen(ofstream& ofstream, const mat& data, const int k, const int n, const double& T)
{
    ofstream << "////////////////////////////////////////" << endl;
    ofstream << "///////////" << " K := " << k << "; N := " << n << "; ///////////" << endl;
    ofstream << "////////////////////////////////////////" << endl;

    ofstream << endl;

    ofstream << "T = " << T << " [s]" << endl;

    ofstream << endl;
    ofstream << left
        << setw(15) << "Server #"
        << setw(15) << "U"
        << setw(15) << "X [1/s]"
        << setw(15) << "n_avg"
        << setw(15) << "R [s]"
        << endl;

    for (int i = 0; i < data.n_rows; i++)
    {
        ofstream << left << setw(15) << i;

        for (int j = 0; j < data.n_cols; j++)
            ofstream << left << setw(15) << data(i, j);
        
        ofstream << endl;
    }

    ofstream << endl;
}

int main(int argc, char** argv)
{
    ofstream outputStream;
    outputStream.open("potraznje_analiticki.txt");

    ofstream output10, output15, output20;
    output10.open("rezultati_analiticki_10.txt");
    output15.open("rezultati_analiticki_15.txt");
    output20.open("rezultati_analiticki_20.txt");

    for (int k = 2; k <= 8; k++) 
    {
        Analyzer gn(k);
        mat result = gn.GetXMatrix();

        writeOutput(outputStream, result, k);

        for (int mprg = 10; mprg <= 20; mprg += 5)
        {
            double T;
            mat data = gn.GetBuzenOutput(result, mprg, T);

            ofstream* oref;
            if (mprg == 10)
                oref = &output10;
            else if (mprg == 15)
                oref = &output15;
            else
                oref = &output20;
            
            writeOutputBuzen(*oref, data, k, mprg, T);
        }
    }

    outputStream.close();
    output10.close();
    output15.close();
    output20.close();

    return 0;
}