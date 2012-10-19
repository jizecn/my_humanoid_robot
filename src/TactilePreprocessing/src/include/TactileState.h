#ifndef __TACTILE_STATE_H__
#define __TACTILE_STATE_H__

//#define TIME_DIMENSION 5

#include "DataSample.h"
#include "circularBuffer.h"

#include "TactileData.h"
#include "TemporalBuffer.h"

#include <yarp/sig/Vector.h>

#include <yarp/math/Math.h> 
#include <yarp/math/api.h>
#include <vector>
#include <yarp/sig/Matrix.h>
#include <yarp/math/api.h>
#include <yarp/math/Math.h>
#include <yarp/os/Time.h>
#include <iostream>
#include "math/TactileClassification.h"

//#include "svm.h"
//#define NO_EVENT -100

class TactileState
{
public:
	enum methodForClassification {SVM_RBF, SVM_BoF, Simple_BLOB_SIZE, SVM_INFORMATION_DISTANCE};

	enum methodForSalience {timeDiff, entropy};

	enum methodForSalienceLevel {totalLevel, maxLevel, localRegional};

	//
	TactileState(int timeEpochs, int taxNum, int samplingRate = 50);
	~TactileState();

	int getSaliencyTaxelInd();
	//void setMemoryLength(unsigned int timeDim);

	//bool insertDataSample(DataSample &ds, bool ifUpdateState = false, methodForSalience type = timeDiff);
	bool insertDataSample(const TactileData &ds, bool ifUpdateState = false, methodForSalience type = timeDiff);

	bool updateSalienceMap(methodForSalience type);
	bool salienceBufferUpdate(methodForSalienceLevel type);
	//touchType updateClassification( methodForClassification methodClassify);
	//int classifyPattern(DataSample &ds, methodForClassification methodClassify);

	void setThresholdTotalLevel(double thres);
	std::string temporalPatternClassify(int classLabel);

private:
	std::string temporalContinuousEvents(std::vector<int> &events);

	void addNewTouchEvent(int classLabel);

	int saliencyTaxelInd;

	//CircularBuffer *temporalData;

	int samplingRate;

	/*How many data samples are stored*/
	int timeDimension; 
	int taxelNum;

	TemporalBuffer *temporalBuffer;

	TemporalBuffer *diffBuffer;
	TemporalBuffer *salienceBuffer;   // the final salience. could be just a single scalar value representing the salience level for the whole body, or a few values for separate parts of the body. 
	TemporalBuffer *touchTypeBuffer;
	//yarp::os::Semaphore mutex;

	// private methods:
	bool diffSalienceUpdate();
	//TactileData *tempD1;
	//TactileData *tempD2;
	TactileData recentData;
	TactileData previousData;

	yarp::sig::Vector tempSalienceVec;

	// variables for salience calculation
	// variables for method totalLevel
	double thresholdTotalLevel;

	// get the most recent data 
	bool updateRecentData();
	std::string shortOrLongEvent(unsigned int dur, int type);
};

class ContinuousEvent
{
public:
	ContinuousEvent()
	{			
		type = zjMath::NO_EVENT;
		duration = 0;
	}
	
	ContinuousEvent(int t, unsigned int dur)
	{
		if(t != zjMath::NO_EVENT && t != zjMath::POKE && t != zjMath::TOUCH)
		{
			std::cerr << "Invalide touch type \n";
			//return false;
		}
		type = t;
		duration = dur;
	}

	~ContinuousEvent()
	{
	}
	int getEventType()
	{
		return type;
	}

	unsigned int getDuration()
	{
		return duration;
	}

	bool setEventType(int t)
	{
		if(t != zjMath::NO_EVENT && t != zjMath::POKE && t != zjMath::TOUCH)
		{
			std::cerr << "Invalide touch type \n";
			return false;
		}
		type = t;
		return true;
	}

	void setDuration(unsigned int t)
	{
		this->duration = t;
	}

private:

	int type;
	unsigned int duration;


};

#endif //__TACTILE_STATE_H__
