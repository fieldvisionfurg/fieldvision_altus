/*
 *  util_csv.h
 *
 *  Created by Michael Kuklik on 11/15/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 *
 *  File contains routines to save Blitz++ matrices in the CSV files.
 *  Matrices of all dimensions are saved column wise in order to
 *  make reshaping easy in Matlab. Note that routines aren't optimized for speed.
 *  Since matrices are stored row wise writing down column wise is slow.
 *  If you want to save data fast, use the binary routines
 *
 *  Usage:
 *  Routines work on Blitz arrays up to 7 dimensions
 *  and C one dimension array
 *
 *  1) to save a matrix A, either double or int
 *     writecsv("filename.csv", A);
 *
 *  2) to load a matrix A which dimensions are NxKxJ
 *
 *    readcsv("filename.csv", A, N,K,J);
 *
 *  3) to load a file in Matlab
 *    x = reshape(csvread('filename.csv'),N,K,J);
 */


#include "stdio.h"
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

using namespace std;


////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//				WRITE/READ CSV FILES TO/FROM C ARRAY
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////

void writecsv(string fname, double *a, int N){

	ofstream ofs;

	// Open file
	ofs.open(fname.c_str());   // read only
	if ( !ofs.is_open()) {
		cerr << "writecsv: Error opening file";
		exit(1);
	}

  // write data
	for ( int i=0; i<N-1; i++) {
		ofs.precision(15);
		ofs << a[i] << ",";
	}
	ofs << a[N-1];
	ofs.close();
}

void writecsv(string fname, int *a, int N){

	ofstream ofs;

	// Open file

	ofs.open(fname.c_str());   // read only
	if ( !ofs.is_open()) {
		cerr << "writecsv: Error opening file: "<< fname << endl;
		exit(1);
	}

	for ( int i=0; i<N-1; i++)
	{
		ofs.precision(15);
		ofs << a[i] << ",";
	}
	ofs << a[N-1];
	ofs.close();

}

void readcsv( string fname, double *a, int N){

	const char delim = ',';
	ifstream ifs;

	// Open file

	ifs.open(fname.c_str());   // read only
	if ( !ifs.is_open())
	{
		cerr << "readcsv: Error opening file " << fname << endl;
		exit(1);
	}

	// get length of file:
	ifs.seekg (-1, ios::end);
	int length = ifs.tellg();
	ifs.seekg (0, ios::beg);

    // Read matrix size

	char c;
	int nElms = 1;
	int nRows = 1;
	for (int i=1; i<length; i++){
		c = ifs.get();
		if (c == delim)
			nElms++;
		if (c == '\n'){
			nRows++;
			nElms++;
		}
	}

	ifs.clear();
	ifs.seekg (0, ios::beg);

	if (nElms % nRows != 0){
		cerr << "readcsv: file error, Rows*Columns != nElements\n";
		exit(1);
	}

	int nColms = nElms / nRows;

	if (nElms > N){
		cerr << "readcsv: file error, more number in file than the array's size\n";
		exit(1);
	}

	// Read elements

	char buff[255] = {0};

	int ii=0;
	for (int iRows = 0; iRows < nRows; iRows++) {
		for(int iColms = 0; iColms < nColms-1; iColms++) {
			ifs.getline(buff,255,delim);
			a[ii] = atof(buff);
			ii++;
		}
		ifs.getline(buff,255);
		a[ii] = atof(buff);
		ii++;
	}

	//
	ifs.close();


}

void readcsv( string fname, int *a, int N){

	const char delim = ',';
	ifstream ifs;

	// Open file

	ifs.open(fname.c_str());   // read only
	if ( !ifs.is_open())
	{
		cerr << "readcsv: Error opening file " << fname << endl;
		exit(1);
	}

	// get length of file:
	ifs.seekg (-1, ios::end);
	int length = ifs.tellg();
	ifs.seekg (0, ios::beg);

    // Read matrix size

	char c;
	int nElms = 1;
	int nRows = 1;
	for (int i=1; i<length; i++){
		c = ifs.get();
		if (c == delim)
			nElms++;
		if (c == '\n'){
			nRows++;
			nElms++;
		}
	}

	ifs.clear();
	ifs.seekg (0, ios::beg);

	if (nElms % nRows != 0){
		cerr << "readcsv: file error, Rows*Columns != nElements\n";
		exit(1);
	}

	int nColms = nElms / nRows;

	if (nElms > N){
		cerr << "readcsv: file error, more number in file than the array's size\n";
		exit(1);
	}

	// Read elements

	char buff[255] = {0};

	int ii=0;
	for (int iRows = 0; iRows < nRows; iRows++) {
		for(int iColms = 0; iColms < nColms-1; iColms++) {
			ifs.getline(buff,255,delim);
			a[ii] = atof(buff);
			ii++;
		}
		ifs.getline(buff,255);
		a[ii] = atoi(buff);
		ii++;
	}

	//
	ifs.close();
}

// 2D

void writecsv(string fname, double* a, int nRows, int nColms){

	ofstream ofs;

	// Open file

	ofs.open(fname.c_str());   // read only
	if ( !ofs.is_open()) {
		cerr << "writecsv: Error opening file: "<< fname << endl;
		exit(1);
	}

	for ( int i=0; i<nRows; i++) {
		for (int j=0; j<nColms-1; j++) {
			ofs.precision(15);
			ofs << a[i*nColms+j] << ",";
		}
		ofs.precision(15);
		if(i==(nRows-1))
			ofs << a[i*nColms + nColms-1];
		else
			ofs << a[i*nColms + nColms-1] << endl;
	}

	ofs.close();

}

void readcsv( string fname, double* a,  int row, int col){

	const char delim = ',';
	ifstream ifs;

	// Open file

	ifs.open(fname.c_str());   // read only
	if ( !ifs.is_open())
	{
		cerr << "readcsv: Error opening file " << fname << endl;
		exit(1);
	}

	// get length of file:
	ifs.seekg (-1, ios::end);
	int length = ifs.tellg();
	ifs.seekg (0, ios::beg);

    // Read matrix size

	char c;
	int nElms = 1;
	int nRows = 1;
	for (int i=1; i<length; i++){
		c = ifs.get();
		if (c == delim)
			nElms++;
		if (c == '\n'){
			nRows++;
			nElms++;
		}
	}

	
	ifs.clear();
	ifs.seekg (0, ios::beg);

	if (nElms % nRows != 0){
		cerr << "readcsv: file error, Rows*Columns != nElements\n";
		exit(1);
	}

	int nColms = nElms / nRows;

	if (row != nRows || col != nColms )
	{
		cerr << "readcsv: array resized to fit data from the file " << fname <<endl;
		exit(0);
	}

	// Read elements

	char buff[255] = {0};

	for (int iRows = 0; iRows < nRows; iRows++) {
		for(int iColms = 0; iColms < nColms-1; iColms++) {
			ifs.getline(buff,255,delim);
			a[iRows*nColms+iColms] = atof(buff);
		}
		ifs.getline(buff,255);
		a[iRows*nColms+nColms-1] = atof(buff);
	}

	ifs.close();
}