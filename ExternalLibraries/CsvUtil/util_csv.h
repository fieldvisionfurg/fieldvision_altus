/*
 * util_csv.h
 *
 *  Created on: Oct 25, 2009
 *      Author: mkuklik
 */

#ifndef UTIL_CSV_H_
#define UTIL_CSV_H_
#include <string>

using namespace std;

void writecsv(string fname, double *a, int N);
void writecsv(string fname, int *a, int N);

void readcsv( string fname, double *a, int N);
void readcsv( string fname, int *a, int N);

// 2D

void writecsv(string fname, double* a, int nRows, int nColms);

void readcsv( string fname, double* a,  int row, int col);

#endif /* UTIL_CSV_H_ */
