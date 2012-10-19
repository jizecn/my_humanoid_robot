#ifndef __BEHAVIOUR_MODE_H__
#define __BEHAVIOUR_MODE_H__

#include <yarp/dev/ControlBoardInterfaces.h>
#include <yarp/dev/IAnalogSensor.h>
#include <ace/OS_NS_stdio.h>
#include <ace/Vector_T.h>
#include <yarp/os/Network.h>
#include <yarp/os/Port.h>
#include <yarp/os/Bottle.h>
#include <yarp/dev/PolyDriver.h>

#include <string>

#include "ServoPosition.h"
#include "KasparPose.h"
#include "KasparSequenceStep.h"
#include "KasparSequenceMap.h"
#include "KasparSequence.h"

#include "Servo.h"
#include "Servos.h"
#include "ServoGroup.h"

#include "SkinPart.h"
#include <iostream>
#include "SoundPlay.h"
#include <yarp/os/Thread.h>
#include "MotorExecution.h"
using namespace yarp::dev;

/**
 * BehaviourMode can be inherited by UserManualMode, APFMode, RuleBasedMode
 */
class BehaviourMode : public  yarp::os::Thread {
public:

    virtual int initConfiguration(char *file);
    virtual int initConfiguration(yarp::os::Property &config);
    //virtual void setPositionDriver(yarp::dev::IPositionControl *pos);
    //virtual void setEncoders(yarp::dev::IEncoders *enc);
    //virtual void setTactileSensors(yarp::dev::IAnalogSensor *ana);

	// basically, the robot will be controlled under sensor-action loop. Therefore, prepare -> action -> prepare -> ...
    virtual bool prepare(std::string command);
    //virtual bool action(KasparSequenceStep *kss) = 0;
    virtual bool action();
    // as most modes would be thread based, the names of the functions are following the same style.
    // not all modes need to be threaded
	    
    virtual void beforeStart() = 0;
    virtual void afterStart(bool success) = 0;
    virtual void threadRelease() = 0;
    virtual void onStop() = 0;
    virtual void run() = 0;
    virtual bool threadInit() = 0;

	virtual void enable(bool enable);

	// manually set sensor drifts as current value. 
	virtual void manualSetDrifts();

protected:
	IPositionControl *pos;
	IEncoders *enc;
    ITorqueControl *torq;
	IAnalogSensor *ana;
	bool enabled;

	// .SKM file. KeyMap
	KasparSequence currentSeq;
	KasparSequenceStep currentSeqStep;

	//KasparPose *kPose;
	Servos servos; 
	ACE_Vector<KasparSequence> kSeqs;
	KasparSequenceMap seqMap;

	string soundFilePath;
	PolyDriver pd; 
	vector<SkinPart *> skinParts;

	SoundPlay sp;

	MotorExecution *motorControl; 
	string sequenceLogFile; 
	bool ifLogSequence; 
	FILE *pSequenceLogFile;

	yarp::os::Semaphore mutex;  

	void initLogFile(Property &config);

	int initYarpDevice(yarp::os::Property &config);
	bool resetPositions();
	void playSound();

	void logSequence(const char *content);
private:

};

#endif
