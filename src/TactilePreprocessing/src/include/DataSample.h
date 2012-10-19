#ifndef __DATA_SAMPLE_H__
#define __DATA_SAMPLE_H__

//#include <yarp/os/RateThread.h>
//#include <yarp/os/Semaphore.h>
//#include <yarp/dev/ControlBoardInterfaces.h>
//#include <yarp/dev/PolyDriver.h>
//#include <yarp/dev/CanBusInterface.h>
#include <yarp/sig/Vector.h>
#include <cmath>
#include <iostream>
#include <vector>
#include "svm.h"

class DataSample
{
 public:
	 /*Constructor. Remember to run init(). otherwise, there will be segmentation fault errors etc. */
  DataSample(const unsigned int dim);
  //DataSample(unsigned int dimension);
  ~DataSample();
  

  unsigned int getDim();
  bool assignData(double *sensorData, const unsigned int offset, const unsigned int size);
  
  /*both ds and result should be of the same dimension */
  bool diff(DataSample &ds, std::vector<double> &result, bool ifAbs = false); 

  double& operator[](int i);

  int classify(svm_model *model);
  static svm_model *loadSVMModel(const char *modelFileName);

 private:
	   bool init(int dimension);
  //double * data;	
	   yarp::sig::Vector data;
	   
	   unsigned int dim;
};

#endif // __DATA_SAMPLE_H__
