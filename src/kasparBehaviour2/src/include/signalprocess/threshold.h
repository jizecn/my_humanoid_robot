#ifndef _THRESHOLD_METHOD_
#define _THRESHOLD_METHOD_

#include <stdio.h>
#include <stdlib.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_statistics.h>

class ThresholdMethod {

 public: 
  enum TouchType {soft, hard, none};
  struct TouchEvent {
    TouchType type;
    double level;  // to quantify the level of touch event, 
  };

  static TouchEvent simpleThreshold(double *data, int len, double threshold_soft, double threshold_hard, double *refBaseLines)
  {
	  TouchEvent te;
	  te.type = none;
	  te.level = 0;
	  double mean, variance, largest, smallest;
	  //mean = gsl_stats_mean(sensorsData, 1, 12*sensorsNum);
	  //variance = gsl_stats_variance(sensorsData, 1, 12*sensorsNum);

        for(int i = 0; i < len; i++)
        {
            // NEW ADDED. NOT TESTED
			//printf("%f %f %f\n", data[i], refBaseLines[i], data[i]-refBaseLines[i] );
            // data[i] = data[i] - refBaseLines[i];
			//printf("data at %d is %f %f\n", i, data[i],  refBaseLines[i]);
        }

	  largest = gsl_stats_max(data, 1, len);
	  smallest = gsl_stats_min(data, 1, len);
	  te.level = largest;
	  if(largest > threshold_hard)
	  {
		  //printf("max is %f %f\n", largest, smallest);
		  te.type = hard;
	  }
	  else if(largest > threshold_soft)
	  {
		  //printf("max is %f %f\n", largest, smallest);
		  te.type = soft;
	  }
	  else
	  {
		  te.type = none;
	  }
	  return te;
  }

  static TouchEvent threshold2(double *data, double *previous_data, int len, double max_threshold, double delta_threshold)
  {
    TouchEvent te; 
    te.type = none;
    te.level = 0;
	
    return te;    
  }
};

#endif // _THRESHOLD_METHOD_
