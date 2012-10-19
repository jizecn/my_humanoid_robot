// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include <gsl/gsl_math.h>
#include <memory.h>

#include <stdio.h>

#ifndef __TRIANGLE_TOUCHSENSOR_H__
#define __TRIANGLE_TOUCHSENSOR_H__

class TriangleTouchSensor
{
public:
  TriangleTouchSensor()  {
  }

  ~TriangleTouchSensor()  {
  }

  int Abs(int x)
  {
    return x>=0?x:-x;
  }
  
  void setActivationFirst7(unsigned char* data)
  {
      printf("Hey here\n");
    for (int i=0; i<7; ++i)
      {
          activation[i]=data[i+1]<=244?double(244-data[i+1]):0.0;
      }
  }
  
  void setActivationLast5(unsigned char* data)
  {
    for (int i=1; i<=5; ++i)
      {
	activation[i+6]=data[i]<=244?double(244-data[i]):0.0;
      }
  }
    
  void setId(int id)
  {
	this->id = id;
  }
  
  int getId()
  {
	return id;
  }
	
 private:
  // original
  double activation[12];
  int id;
};

#endif
