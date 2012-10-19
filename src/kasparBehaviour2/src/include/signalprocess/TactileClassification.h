#ifndef _TACTILECLASSIFICATION_METHOD_
#define _TACTILECLASSIFICATION_METHOD_

#include <stdio.h>
#include <stdlib.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_statistics.h>

// Simple classification: Counting activated taxels
#define POKING_NUM_TAXEL 12   // below it is poking, above it is grasping/palm touching/etc

class TactileClassification {

public: 
	enum TouchType {poke, touch, na};
	// temporarily hardcoded
	enum TouchLocation {palm, dorsal};
	enum TouchForce {soft, hard, none};
	struct TouchEvent {
		TouchForce force;
		TouchType type;
		TouchLocation loc; 
		double level;  // to quantify the level of touch event, 
	};

	static TouchEvent simpleThreshold(double *data, int len, double threshold_soft, double threshold_hard, double *refBaseLines)
	{
		TouchEvent te;
		te.type = na;
		te.force = none;
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
			te.force = hard;
		}
		else if(largest > threshold_soft)
		{
			//printf("max is %f %f\n", largest, smallest);
			te.force = soft;
		}
		else
		{
			te.force = none;
		}
		return te;
	}

	static TouchEvent threshold2(double *data, double *previous_data, int len, double max_threshold, double delta_threshold)
	{
		TouchEvent te; 
		te.type = na;
		te.level = 0;

		return te;    
	}


	static TouchEvent simpleClassify(double *data, int len, double threshold_soft, double threshold_hard)
	{
		TouchEvent te;
		te.type = na;
		te.force = none;
		te.level = 0;

		//te.level = largest;

		int count = 0;

		for (int i = 0; i < len; i++)
		{
			if(data[i] > threshold_soft) // or other threshold
			{
				count++;
			}
		}

		if(count > POKING_NUM_TAXEL)
		{
			te.type = touch;
			te.force = soft;
		}
		else if(count < POKING_NUM_TAXEL && count > 3)
		{
			te.type = poke;
			te.force = soft; // currently all set soft.
		}
		return te;
	}
};

#endif // _TACTILECLASSIFICATION_METHOD_
