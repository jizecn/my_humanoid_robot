#ifndef __SKIN_SENSORS__
#define __SKIN_SENSORS__

#include <yarp/os/RateThread.h>
#include <yarp/os/Thread.h>
#include <yarp/os/Semaphore.h>
#include <yarp/dev/ControlBoardInterfaces.h>
#include <yarp/dev/PolyDriver.h>
#include <yarp/dev/CanBusInterface.h>
#include <yarp/sig/Vector.h>
#include <yarp/os/ResourceFinder.h>
#include <vector>
#include <string>

#include "SkinBodyPart.h"
#include "utils/Resources.h"
#include <algorithm>

#include "TactileState.h"

#include "TactileData.h"
#include "TemporalBuffer.h"

#include "SkinGroup.h"

#define TIME_DIMENSION 100
//#define TAXEL_DIMENSION 912


class SkinSensors: public yarp::os::Thread
{
public:
	SkinSensors(yarp::os::Property rf)
	{
		//tempDataSample = NULL;
		tState = NULL;
		tempTactileData = NULL;

		this->config = rf;
	}

	~SkinSensors()
	{

		for (size_t i = 0; i < partList.size(); i++)
		{

			if (partList[i])
			{
				//partList[i]->disconnect();
				delete partList[i];
			}
		}

		if (tState)
		{
			delete tState;
		}
		if (tempTactileData)
		{
			delete tempTactileData;
		}
		for(int i = 0; i < skinGroups.size(); i++)
		{
			delete skinGroups[i];
		}
	}

	bool threadInit();
	bool initConfiguration();
	void run();
	void threadRelease();

private:
	int numOfSensorPatches;
	yarp::os::Property config;
	/* ports */
	std::vector<yarp::os::BufferedPort<Vector> > compensatedTactileDataPorts;
	std::vector<SkinBodyPart *> partList;

	std::vector<SkinGroup*> skinGroups;

	std::vector<unsigned int> offsets;
	TactileState *tState;
	TactileData *tempTactileData;
	yarp::os::Semaphore mutex;
	bool initSensorGroups();
};

#endif
