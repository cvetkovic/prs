#pragma once

#include <iostream>
#include <armadillo>

using namespace std;
using namespace arma;

class Analyzer
{
private:
	const int K;
	const int probabilityMatrixSize;

	const double cpu = 1000. / 5;
	const double sys_disk1 = 1000. / 20;
	const double sys_disk2_3 = 1000. / 15;
	const double usr_disk = 1000. / 20;

	mat CreateProbabilityMatrix();
	mat GetGs(const int n, const int k, const mat& x);

public:
	Analyzer(int K) : K(K), probabilityMatrixSize(4 + K) {}

	mat GetXMatrix();
	mat GetBuzenOutput(const mat& x_matrix, const int n, double& T);
};