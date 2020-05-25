#include "GordonNewell.h"

mat GordonNewellSolver::CreateProbabilityMatrix()
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

    matrix.print("P:");

    return matrix;
}

mat GordonNewellSolver::GetXMatrix()
{
    mat P = CreateProbabilityMatrix();
    mat PTransposed = P.t();

    PTransposed.print("P^T:");

    mat identity = eye<mat>(probabilityMatrixSize, probabilityMatrixSize);
    mat leftSide = PTransposed - identity;
    leftSide.print("Left side:");

    mat DVector = null(leftSide);

    DVector.print("Solved D vector:");

    const double cpu = 1000. / 5;
    const double sys_disk1 = 1000. / 20;
    const double sys_disk2_3 = 1000. / 15;
    const double usr_disk = 1000. / 20;

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

    XVector.print("X vector:");

    mat normalizedX = XVector / XVector(0, 0);

    normalizedX.print("Normalized X vector with X0:");

    return normalizedX;
}
