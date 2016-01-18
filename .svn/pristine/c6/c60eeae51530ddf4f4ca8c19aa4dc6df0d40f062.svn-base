//============================================================================
//  File with example for moving data from C to Matalb and vice versa
//============================================================================

#include <iostream>
#include "util_bin.h"
#include "util_csv.h"

using namespace std;

const int N1=4, N2=13, N3=3, N4=16;
const int N=N1*N2*N3*N4;

double A[N1][N2][N3][N4];  // static

double B[N];  // static, organized as A(i,j,k,l)=A[i*N2*N3*N4 + j*N3*N4 + k*N4 + l]
inline int iB(int i, int j, int k, int l){ return i*N2*N3*N4+j*N3*N4+k*N4+l; }


int main(void) {

	double *C = new double[N];

	for (int i=0; i<N1; i++)
		for (int j=0; j<N2; j++)
			for (int k=0; k<N3; k++)
				for (int l=0; l<N4; l++){
					A[i][j][k][l] = i*1000000 + j*10000 + k*100 + l;
					B[iB(i,j,k,l)] = 0;
					C[iB(i,j,k,l)] = i*1000000 + j*10000 + k*100 + l;
				}

	writebin("A.bin",&A[0][0][0][0], N);
	writecsv("A.csv",&A[0][0][0][0], N);
	writebin("C.bin",C,N);
	writecsv("C.csv",C,N);


	readbin("B.bin",B,N);
	int error=0;
	for (int i=0; i<N1; i++)
		for (int j=0; j<N2; j++)
			for (int k=0; k<N3; k++)
				for (int l=0; l<N4; l++){
					error += (B[iB(i,j,k,l)] != i*1000000 + j*10000 + k*100 + l);
				}

	if (error==0) printf("B is ok\n");
	else printf("B is corrupted\n");

	delete C;

	return 0;
}
