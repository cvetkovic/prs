#pragma once

#include <iostream>
#include <armadillo>

using namespace std;
using namespace arma;

class GordonNewellSolver
{
private:
	const int K;
	const int probabilityMatrixSize;

	mat CreateProbabilityMatrix();

public:
	GordonNewellSolver(int K) : K(K), probabilityMatrixSize(4 + K) {}

	mat GetXMatrix();
};