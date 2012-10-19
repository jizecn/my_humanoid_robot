// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __TOUCHSENSOR_H__
#define __TOUCHSENSOR_H__

#define NUM_TAXEL_TRIANGLE 12

class TouchSensor
{
public:
    TouchSensor(int id, double gain = 1, int numTaxels = NUM_TAXEL_TRIANGLE) 
	{
        this->id = id;
        this->dGain = gain;

        this->calibrated_skin = true;
        this->setNumTaxels(numTaxels); // default
    }

    virtual ~TouchSensor()
    {
    }

    void setCalibrationFlag (bool use_calibrated_skin)
    {
        this->calibrated_skin=use_calibrated_skin;
    }

    double getGain()
    {
        return this->dGain;
    }

    int getId()
    {
        return id;
    }

    int getNumTaxels()
    {
        return this->numTaxels;
    }

private:
    // original
    int id;  // triangle id
    double dGain;
    bool calibrated_skin;
    int numTaxels;

    // private. not allow to reset
    void setNumTaxels(int numTaxels)
    {
        this->numTaxels = numTaxels;
    }

};

#endif
