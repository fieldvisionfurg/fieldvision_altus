/*
 * util_bin.h
 *
 *  Created on: Oct 25, 2009
 *      Author: mkuklik
 */

#ifndef UTIL_BIN_H_
#define UTIL_BIN_H_
#include <string>

using namespace std;

void writebin(string fname, double *a, int N);
void readbin(string fname, double *a, int nElms);
void writebin(string fname, int *a, int N);
void readbin(string fname, int *a, int nElms);
void writebin(string fname, char *a, int N);
void readbin(string fname, char *a, int nElms);


#endif /* UTIL_BIN_H_ */
