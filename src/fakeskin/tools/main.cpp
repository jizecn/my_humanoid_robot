/*
 * Copyright (C) 2011 Ze Ji
 * University of Hertfordshire. 
 * EU FP7 RoboSkin 
 * CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
 *
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <yarp/sig/Vector.h>

#define DIMENSION 192

using namespace std;

bool writeToFile(size_t dim, double *data, ofstream &file);
bool dataToString(size_t dim, double *data, string &out);
bool prepareData(char *input, vector<yarp::sig::Vector> &data);

bool dataToString(size_t dim, double *data, string &out)
{
  std::ostringstream  s;
  for(int i = 0; i < dim; i++)
    {
      s << data[i] << " ";
    }
  out = s.str();
  return true;
}

bool writeToFile(size_t dim, double *data, ofstream &file)
{
  string out;
  if(dataToString(dim, data, out))
    {
      file << out.c_str() << endl;
      return true; 
    }
  else
    return false;
}

bool prepareData(char *input, int dim, vector<yarp::sig::Vector> &data)
{
  // temp data for testing
  for (int i = 0; i < 100; i++)
    {
      yarp::sig::Vector v; 
      for (int j = 0; j < dim; j++)
	{
	  v.push_back(0);
	}
      data.push_back(v);
    }
  return true;
}

int main(int argc, char *argv[])
{
  ofstream file;
  double tempData[DIMENSION];
  cout << "Prepare Data File that FakeSkin can load" << endl;
  
  if (argc < 3)
    {
      cout << "ERROR: please specify input and output files. prepareDataForFakeSkin input output" << endl;
      exit(-1);
    }
  vector<yarp::sig::Vector> data;
  if(!prepareData(argv[1], DIMENSION, data))
    {
      cout << "ERROR: could not load input data" << endl;
      exit(-1);
    }
  file.open(argv[2]);
  
  for(int i = 0; i<100; i++)
    {
      writeToFile(DIMENSION, (data[i]).data(), file);
    }
  
  file.close();
  return 0;
}
