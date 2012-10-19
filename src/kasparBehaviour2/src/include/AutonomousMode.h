/* 
 * File:   AutonomousMode.h
 * Author: ze
 *
 * Created on 15 June 2010, 15:06
 */

#ifndef __AUTONOMOUSMODE_H__
#define	__AUTONOMOUSMODE_H__

#include <string>
#include "BehaviourMode.h"
#include <yarp/os/Thread.h>
#include <yarp/os/Time.h>
#include <yarp/os/all.h>
#include "SoundPlay.h"

class AutonomousMode : public BehaviourMode {
public:
    AutonomousMode();
    
    ~AutonomousMode();

    // inherited from Thread
    
    virtual void beforeStart();
    virtual void afterStart(bool success);
    virtual void threadRelease();
    virtual void onStop();
    virtual void run();
    virtual bool threadInit();

    //int initConfiguration(char *file);
    int initConfiguration(yarp::os::Property &config);
	virtual void enable(bool enable);

    virtual bool prepare(std::string command);

    virtual bool action();

    // maybe this is better to be in ServoGroup class?
    // @param stop: if stop the servos after setting torques
    void servosCompliant(ServoGroup *sg, int torque, bool stop = true);

	virtual void manualSetDrifts();
private:
	yarp::os::Network yarp;
    
    yarp::os::Port p;            // Create a port.
	bool portOpen;
	yarp::os::Bottle b;          // Make a place to store things.
	BufferedPort<Bottle> skin_port;
	//yarp::os::Semaphore mutex;  

	//bool processData(yarp::os::Bottle *b);

};

#endif	/* __AUTONOMOUSMODE_H__ */