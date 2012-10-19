#ifndef __MOTION_EXECUTION__
#define __MOTION_EXECUTION__

#include <yarp/os/Thread.h>
#include <yarp/dev/ControlBoardInterfaces.h>
#include <yarp/dev/PolyDriver.h>
#include <yarp/dev/all.h>

#include "ServoPosition.h"
#include "KasparPose.h"
#include "KasparSequenceStep.h"
//#include "KasparSequenceMap.h"
#include "KasparSequence.h"

#include "Servo.h"
#include "Servos.h"
#include "ServoGroup.h"
#include <iostream>
class MotorExecution : public yarp::os::Thread
{
public: 
    MotorExecution(yarp::dev::IPositionControl *iPos, yarp::dev::IEncoders *enc, yarp::dev::ITorqueControl *torq, Servos *servos);
    ~MotorExecution();
    virtual void run();
    void setKasparSequence(KasparSequence &currentSeq);
    bool appendSequence(KasparSequence &seq);
private:
    yarp::dev::IPositionControl *pos;
    yarp::dev::ITorqueControl *torq;
    KasparSequence currentSeq;  
    Servos *servos;
    KasparSequenceStep currentSeqStep;
    yarp::dev::IEncoders *enc;
    
    KasparSequence appendedSeq;
    bool seqQueued;
    void executeSequence();
};

#endif
