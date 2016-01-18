/*
 *  util_bin.h
 *  Saving data in binary format in C++
 *
 *
 *  Arrays can be saved much faster in binary format than in CSV format.
 *  One can read binary files with Matlab or Octave.
 *  There is a problem with reshaping a multidimensional arrays in Matlab.
 *  Binary data is stored and saved rows-wise in C and column-wise in Matlab (or Fortran).
 *  Thus, Matlab reshape matricies column-wise but the data you read from
 *  the binary file is saved row-wise.
 *  To read the 3D array from file tmp.bin in matlab execute :
 *
 *  %read in the same file
 *  double tmp[N1*N2*N3];
 *  writebin("tmp.bin",tmp,N1*N2*N3);
 *  fid = fopen('tmp.bin','rb')   %open file
 *  data = fread(fid, 'double')  %read in the data 'int32' 'int64'
 *  fclose(fid)   %close file
 *  data = reshape(data,N3,N2,N1);
 *  data = permute(data,3:-1:1);
 *
 *  If your data is integer then use 'int' if double use 'double'
 *  Only when you transfer data from/to 32/64 bit platform you have to specify
 *  int32 or int64.
 *
 *  Created by Michael Kuklik on 4/14/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 *
 *  LAST UPDATE:  6/15/2009
 *
 */
#define _BIN_ERROR 1

// Binary data saving
#include "stdio.h"
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include "util_bin.h"

using namespace std;


//////////////////////////////////////////////////////////////////////////////////
//
//    WRITE/READ BINARY DOUBLE     C++ ARRAY
//
//////////////////////////////////////////////////////////////////////////////////

void writebin(string fname, double *a, int N) {

  ofstream ofs;

  // Open file

  ofs.open(fname.c_str(),ofstream::binary);   // read only
  if ( !ofs.is_open()) {
    cerr << "writebin: Error opening file " << fname << endl;
    exit(_BIN_ERROR);
  }

  ofs.write( (char*) a, sizeof(double)*N);

  ofs.close();
}

void readbin(string fname, double *a, int nElms) {

  ifstream ifs;

  // Open file

  ifs.open(fname.c_str());   // read only
  if ( !ifs.is_open()){
    cerr << "readbin: Error opening file " << fname << endl;
    exit(_BIN_ERROR);
  }

  // get length of file:
  ifs.seekg (0, ios::end);
  int length = ifs.tellg();
  ifs.seekg (0, ios::beg);

  // If more elements then allocated in Array, display warning
  if ((length/sizeof(double)) > nElms) {
    cerr << "\nreadbin: WARNING not enough memory is allocated for the file " << fname <<
            "\n                 only reading what there's a space for !!!!!!!!\n\n";
  }
  if ((length/sizeof(double)) < nElms) {
    cerr << "\nreadbin: WARNING file " << fname << " is smaller than the allocated memory !!!!!!!!\n\n";
    nElms = (length/sizeof(double));
  }

  // Read
  ifs.read( (char*) a , sizeof(double)*nElms );
  ifs.close();
}

//////////////////////////////////////////////////////////////////////////////////
//
//    WRITE/READ BINARY INT     C++ ARRAY
//
//////////////////////////////////////////////////////////////////////////////////

void writebin(string fname, int *a, int N) {

  ofstream ofs;

  // Open file

  ofs.open(fname.c_str(),ofstream::binary);   // read only
  if ( !ofs.is_open()) {
    cerr << "writebin: Error opening file " << fname << endl;
    exit(_BIN_ERROR);
  }

  ofs.write( (char*) a, sizeof(int)*N);

  ofs.close();
}


void readbin(string fname, int *a, int nElms) {

  ifstream ifs;

  // Open file

  ifs.open(fname.c_str());   // read only
  if ( !ifs.is_open()){
    cerr << "readbin: Error opening file " << fname << endl;
    exit(_BIN_ERROR);
  }

  // get length of file:
  ifs.seekg (0, ios::end);
  int length = ifs.tellg();
  ifs.seekg (0, ios::beg);

  // If more elements then allocated in Array, display warning
  if ((length/sizeof(int)) > nElms) {
    cerr << "\nreadbin: WARNING not enough memory is allocated for the file " << fname <<
            "\n                 only reading what there's a space for !!!!!!!!\n\n";
  }
  if ((length/sizeof(int)) < nElms) {
    cerr << "\nreadbin: WARNING file " << fname << " is smaller than the allocated memory !!!!!!!!\n\n";
    nElms = (length/sizeof(int));
  }

  // Read
  ifs.read( (char*) a , sizeof(int)*nElms );
  ifs.close();
}


//////////////////////////////////////////////////////////////////////////////////
//
//    WRITE/READ BINARY CHAR     C++ ARRAY
//
//////////////////////////////////////////////////////////////////////////////////

void writebin(string fname, char *a, int N) {

  ofstream ofs;

  // Open file

  ofs.open(fname.c_str(),ofstream::binary);   // read only
  if ( !ofs.is_open()) {
    cerr << "writebin: Error opening file " << fname << endl;
    exit(_BIN_ERROR);
  }

  ofs.write( a, N);

  ofs.close();
}

void readbin(string fname, char *a, int nElms) {

  ifstream ifs;

  // Open file

  ifs.open(fname.c_str());   // read only
  if ( !ifs.is_open()){
    cerr << "readbin: Error opening file " << fname << endl;
    exit(_BIN_ERROR);
  }

  // get length of file:
  ifs.seekg (0, ios::end);
  int length = ifs.tellg();
  ifs.seekg (0, ios::beg);

  // If more elements then allocated in Array, display warning
  if ((length) > nElms) {
    cerr << "\nreadbin: WARNING not enough memory is allocated for the file " << fname <<
            "\n                 only reading what there's a space for !!!!!!!!\n\n";
  }
  if ((length) < nElms) {
    cerr << "\nreadbin: WARNING file " << fname << " is smaller than the allocated memory !!!!!!!!\n\n";
    nElms = (length);
  }

  // Read
  ifs.read( (char*) a , nElms );
  ifs.close();
}

