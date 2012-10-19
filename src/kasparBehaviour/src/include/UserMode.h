#ifndef __USER_MODE_H__
#define __USER_MODE_H__

#include "BehaviourMode.h"
//#include "utils/KasparPoseFile.h"

// exceptions
//#include "utils/FileIOException.h"
//#include "utils/FileFormatException.h"

#include <ace/Vector_T.h>
#include <yarp/os/Time.h>
#include "ServoPosition.h"
#include "KasparPose.h"
#include "KasparSequenceStep.h"
#include "KasparSequenceMap.h"
#include "KasparSequence.h"
#include "Servo.h"
#include "Servos.h"
#include <iostream>

using namespace yarp::os;

/**
 * UserMode is a working mode that Kaspar would be controlled by user/human.
 * In other words, Kaspar is manually and remotely (not necessary) controlled.
 */
class UserMode : public BehaviourMode {

public:
    UserMode();
    ~UserMode();

    int initConfiguration(char *file);
    //int initConfiguration(yarp::os::Property &config);
    bool prepare(string command);
    bool action();
    /*
    void pause();
    void start();
    void resume();
    void threadInit();
    
    bool stop();*/
	//bool disable();
    void setPositionDriver(yarp::dev::IPositionControl *pos);
	//void enable(bool enable);
    
private:
        // pose list
        // .SKM file. KeyMap
	/*
		KasparSequence currentSeq;

        KasparSequenceStep currentSeqStep;
        
        //KasparPose *kPose;
		Servos servos; 
		ACE_Vector<KasparSequence> kSeqs;
		KasparSequenceMap seqMap;

		string soundFilePath;
		*/
};

#endif
