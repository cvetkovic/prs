#include "GordonNewell.h"

mat Analyzer::CreateProbabilityMatrix()
{
    mat matrix(probabilityMatrixSize, probabilityMatrixSize);
    matrix.fill(0); // initialize matrix with all zeros

    matrix(0, 0) = 0.1;  // CPU -> CPU
    matrix(0, 1) = 0.1;  // CPU -> SYS_DISK1
    matrix(0, 2) = 0.1;  // CPU -> SYS_DISK2
    matrix(0, 3) = 0.1;  // CPU -> SYS_DISK3

    matrix(1, 0) = 0.4;  // SYS_DISK1 -> CPU
    matrix(2, 0) = 0.4;  // SYS_DISK2 -> CPU
    matrix(3, 0) = 0.4;  // SYS_DISK3 -> CPU

    const double toUserDisk = (1 - 0.4) / K;
    const double toCPU = 1;

    for (int i = 0; i < probabilityMatrixSize; i++) {
        for (int j = 4; i < 4 && j < probabilityMatrixSize; j++)
            matrix(i, j) = toUserDisk;

        if (i >= 4)
            matrix(i, 0) = toCPU;
    }

    //matrix.print("P:");

    return matrix;
}

mat Analyzer::GetGs(const int n, const int k, const mat& x)
{
    mat coefficient(n + 1, 1);

    // matrix initial content
    coefficient(0, 0) = 1;
    for (int i = 1; i <= n; i++)
        coefficient(i, 0) = 0;

    coefficient.print("Initial G:");

    // calculation
    for (int j = 1; j <= k; j++)
        for (int i = 1; i <= n; i++)
            coefficient(i, 0) += coefficient(i - 1, 0) * x(j - 1, 0);

    coefficient.print("Final G:");

    return coefficient;
}

mat Analyzer::GetXMatrix()
{
    mat P = CreateProbabilityMatrix();
    mat PTransposed = P.t();

    //PTransposed.print("P^T:");

    mat identity = eye<mat>(probabilityMatrixSize, probabilityMatrixSize);
    mat leftSide = PTransposed - identity;
    //leftSide.print("Left side:");

    mat DVector = null(leftSide);

    //DVector.print("Solved D vector:");

    mat XVector(probabilityMatrixSize, 1);
    for (int i = 0; i < probabilityMatrixSize; i++)
    {
        if (i == 0)
            XVector(0, 0) = DVector(0, 0) / cpu;
        else if (i == 1)
            XVector(1, 0) = DVector(1, 0) / sys_disk1;
        else if (i == 2 || i == 3)
            XVector(i, 0) = DVector(i, 0) / sys_disk2_3;
        else
            XVector(i, 0) = DVector(i, 0) / usr_disk;
    }

    //XVector.print("X vector:");

    mat normalizedX = XVector / XVector(0, 0);

    //normalizedX.print("Normalized X vector with X0:");

    return normalizedX;
}

mat Analyzer::GetBuzenOutput(const mat& x_matrix, const int n, double& T)
{
    mat G = GetGs(n, x_matrix.n_rows, x_matrix);
    mat result(x_matrix.n_rows, 4);
    result.fill(0);

    double ratio = G(n - 1, 0) / G(n, 0);
    for (int i = 0; i < x_matrix.n_rows; i++)
    {
        double mi;
        if (i == 0)
            mi = cpu;
        else if (i == 1)
            mi = sys_disk1;
        else if (i == 2 || i == 3)
            mi = sys_disk2_3;
        else
            mi = usr_disk;

        result(i, 0) = ratio * x_matrix(i, 0);       // Usage = x(i) * G(n - 1) / G(n)
        result(i, 1) = mi * ratio * x_matrix(i, 0);  // Throughput = mi * Usage
    }

    for (int i = 0; i < x_matrix.n_rows; i++)
    {
        double n_avg = 0;

        for (int m = 1; m <= n; m++)
        {
            n_avg += pow(x_matrix(i, 0), m) * G(n - m) / G(n);
        }

        result(i, 2) = n_avg;                           // n_avg = sum(x^m * G(n - m) / G(n))
        result(i, 3) = n_avg / result(i, 1);            // Ri = n_avg / Xi
    }

    result.print("0 - usage, 1 - throughput, 2 - average number of jobs, 3 - Response time:");

    T = n / (0.1 * result(0, 1));   // Tcpu = n / X_CPU

    return result;
}
