/* 
 * File:   AutonomousMode.h
 * Author: ze
 *
 * Created on 15 June 2010, 15:06
 */

#ifndef __GAMEMODE_H__
#define	__GAMEMODE_H__

#include <string>
#include "BehaviourMode.h"
#include <yarp/os/Thread.h>
#include <yarp/os/Time.h>
#include <yarp/os/all.h>
#include "SoundPlay.h"

#include <vector>

#include "GameTouchInOrder.h"
#include "GameScenario.h"

class GameMode : public BehaviourMode {
public:

	enum SoundList {correct, ok, wrong, yourturn};

    GameMode();
    
    ~GameMode();

	virtual int initConfiguration(yarp::os::Property &config);

    // inherited from Thread
    
    virtual void beforeStart();
    virtual void afterStart(bool success);
    virtual void threadRelease();
    virtual void onStop();
    virtual void run();
    virtual bool threadInit();

    //int initConfiguration(char *file);
    //int initConfiguration(yarp::os::Property &config);
	virtual void enable(bool enable);

    virtual bool prepare(std::string command);

    virtual bool action();

    // maybe this is better to be in ServoGroup class?
    // @param stop: if stop the servos after setting torques
    void servosCompliant(ServoGroup *sg, int torque, bool stop = true);
	virtual void manualSetDrifts();
	void logGameStatus(string tag, string info);

private:
	yarp::os::Network yarp;
    
    yarp::os::Port p;            // Create a port.
	bool portOpen;
	yarp::os::Bottle b;          // Make a place to store things.
	BufferedPort<Bottle> skin_port;
	//yarp::os::Semaphore mutex;  

	//bool processData(yarp::os::Bottle *b);
	bool waiting;    // true if robot is giving command, e.g. showing sensor parts in order. when false, user should touch those parts

	std::vector<string> sensorOrder;
	int getIndexOfSensorPart(string sensor);  // for the game scenario

	//void awardVoice(bool correct);

	void awardVoice(SoundList soundFile);

	GameScenario gameSce;
	GameTouchInOrder *currentSession;
	string currentSessionName;

	int gameCount;  // count the times of game event 
	string currentCommand;

};

#endif	/* __GAMEMODE_H__ */