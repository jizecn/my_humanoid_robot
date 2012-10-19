// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __ALE_TOUCHSENSOR_H__
#define __ALE_TOUCHSENSOR_H__

// Define variables for drift removal
//TODO: Should be read from config file.. later
#define TOLERANCE 5
#define THRESHOLD_0 20
#define THRESHOLD_1 70

#define NUMBER_TAXEL 12

#include <gsl/gsl_math.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>
#include <memory.h>
#include <math.h>
#include <stdio.h>


class TouchSensor
{
    bool calibrated_skin;

public:
    TouchSensor(double gain = 1) {
        for(int i=0; i < NUMBER_TAXEL; i++)
        {
            activation[i] = 0;
			lastMeasuredActivation[i] = 0;
			filteredActivation[i] = 0;
			drifts[i] = 0;
			touched[i] = false;
			//manualDrifts[i] = 0;
		}
		dGain = gain;
		
		// initialise variables for drift removal
		tolerance = TOLERANCE;
		threshold0 = THRESHOLD_0;
		threshold1 = THRESHOLD_1;
    }

    virtual ~TouchSensor()
    {/*
     if (Exponential)
     {
     delete [] Exponential;
     Exponential=0;
     }*/
        //gsl_vector_free(gsl_activation);
    }

    void setCalibrationFlag (bool use_calibrated_skin)
    {
        calibrated_skin=use_calibrated_skin;
    }
    int Abs(int x)
    {
        return x>=0?x:-x;
    }
	/*
    void setActivationFirst7(unsigned char* data)
    {
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
	*/

    double setActivationFromPortData(double val, int id)
    {

        if (calibrated_skin)
        {
            activation[id]=val;
            //	gsl_vector_set(gsl_activation, id, val);
        }
        else
        {
            activation[id]=val<=244?double(244-val):0.0;
            //gsl_vector_set(gsl_activation, id, val<=244?double(244-val):0.0);
        }

		this->gainAdjust(activation[id]);
		// To remove drifts
		//driftRemoval(id);

		filteredActivation[id] = activation[id] - drifts[id];
		
		return activation[id];
    }

	void getData(double *data, int j, bool print=false)  {
        // length must match. 12 in this case
        //    gsl_matrix_set_col(m, j, this->gsl_activation);
        for(int i = 0; i < 12; i++)
        {
			if(print)
				printf("activation[%d] is %f, \n", i, activation[i]);
            data[i + j*12] = activation[i];
        }
    }

	void getFilteredData(double *data, int j, bool print=false)  {
        // length must match. 12 in this case
        //    gsl_matrix_set_col(m, j, this->gsl_activation);
        for(int i = 0; i < 12; i++)
        {
			if(print)
				printf("activation[%d] is %f, \n", i, filteredActivation[i]);
            data[i + j*12] = filteredActivation[i];
        }
    }

	void gainAdjust(double& data)
	{
		data *= this->dGain;//*data;
	}

	/**
	* Auto Remove drifting of the ith taxel
	*/
	void driftRemoval(int i)
	{
		// if, else, if .... 
		if (activation[i] < threshold0)
		{
			//touched[i] = false;
			drifts[i] = activation[i];
		}
		else if (activation[i] > threshold1)
		{
			//touched[i] = true;
			//drifts[i] = drifts[i]; // no change
		}
		else
		{
			if (fabs(activation[i] - lastMeasuredActivation[i]) < tolerance)
			{
				// no change
			}
			else if ((lastMeasuredActivation[i] - activation[i]) >= tolerance)
			{
				// can change
				drifts[i] = activation[i];
			}
			else
			{
				//drifts[i] = drifts[i];
			}
		}

		if (activation[i] < drifts[i])
		{
			drifts[i] = activation[i];
		}
		lastMeasuredActivation[i] = activation[i];
		filteredActivation[i] = activation[i] - drifts[i];
		//printf("Filtered data %f, measured data %f\n",filteredActivation[i], activation[i]);
	}

	void manualSetDrifts()
	{
		for (int i = 0; i < NUMBER_TAXEL; i++)
		{
			drifts[i] = activation[i];
		}
	}

protected:
    // original
    double dX[12],dY[12];
    static double dXmin,dXmax,dYmin,dYmax;
    double dXv[8],dYv[8];
    double dXc,dYc;
    double dGain;
    int    ilayoutNum;
    int    ilrMirror;

    double m_Radius,m_RadiusOrig;
    double activation[12];

	/////////// for drift removal
	double lastMeasuredActivation[12];
	double filteredActivation[12];
	double drifts[12];  // estimated drift
	double tolerance;  // = 5;
	double threshold0;  // = 20;
	double threshold1; // = 70;
	bool touched[12];  

	//double manualDrifts[12];

	//////////////////////

    //gsl_vector *gsl_activation;
    //static int m_maxRange;
    //static double *Exponential;

    // scaled
    int x[12],y[12];
    int xv[8],yv[8];

    int nVerts;

    int xMin,xMax,yMin,yMax;

    int m_Width,m_Height;
};

#endif
