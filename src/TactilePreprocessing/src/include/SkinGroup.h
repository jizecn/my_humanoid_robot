#ifndef __SKINGROUP_H__
#define __SKINGROUP_H__

#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <sstream>
#include <yarp/os/RateThread.h>
#include <yarp/os/Thread.h>
#include <yarp/os/BufferedPort.h>
#include <yarp/os/Semaphore.h>
#include <yarp/sig/Vector.h>
#include <yarp/os/ResourceFinder.h>
#include <vector>
#include <string>

#include "SkinBodyPart.h"
#include "utils/Resources.h"
#include "utils/Utils.h"
#include <algorithm>

#include "TactileState.h"
#include "TactileData.h"
#include "TemporalBuffer.h"

#include "math/TactileClassificationRawData.h"
#include "math/TactileClassificationHist.h"
#define TIME_DIMENSION 100

//#define TAXEL_DIMENSION 912

//using namespace std;
//using namespace yarp::os;
//using namespace yarp::sig;

class SkinGroup : public yarp::os::Thread
{
public:
SkinGroup();
~SkinGroup();

bool initConfiguration(yarp::os::Property &conf);

int getNumOfSkinPart();

int getNumOfTaxels();

int getIndexOfSkin(std::string &skinPartName);

bool setDataOfSkinPart(std::string &skinPartName, double *data);

std::string getGroupName();

virtual bool threadInit();

//called by start after threadInit, s is true iff the thread started
//successfully
virtual void afterStart(bool s);

virtual void run();
virtual void threadRelease();
bool sendResult(int touchClass);
bool sendResult(string touchType, string strength);
virtual void onStop();

void closeFile();

private:
std::string groupName;
std::vector<SkinBodyPart *> skinParts;

yarp::os::BufferedPort<Bottle> classificationOutport;
yarp::os::Property config;
/* ports */
std::vector<yarp::os::BufferedPort<yarp::sig::Vector> > compensatedTactileDataPorts;

std::vector<unsigned int> offsets;
TactileState *tState;
TactileData *tempTactileData;
yarp::os::Semaphore mutex;
int taxelDimension;

 TactileClassificationHist *classification;	
 std::string svmmodelfile;
	
bool initSensors(yarp::os::Property &config);	
 void initOutport(yarp::os::Property &config);
 
bool initLogFile(yarp::os::Property &config);

void logData(TactileData *data);
bool ifLogData;
FILE *pLogFile;

std::string logFile;

	void logText(string tag, string text)
	{
		//mutex.wait();
		if(this->ifLogData)
		{
			fprintf(pLogFile, "# %s # %s   # ", tag.c_str(), text.c_str() );
			// log data to file
			time_t rawtime;
			struct tm * timeinfo;
			time(&rawtime);
			timeinfo = localtime ( &rawtime );
			//std::string timeText = asctime(timeinfo);
			fprintf(pLogFile, " %f ", yarp::os::Time::now());
			fputs(asctime(timeinfo), pLogFile);   // asctime() will add "\n" automatically 
		}
		//mutex.post();
	}

	std::string lookUpTouchType(int classLabel);
	bool sentNoneEvent;
};

#endif //__SKINGROUP_H__
