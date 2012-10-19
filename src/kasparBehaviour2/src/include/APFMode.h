/* 
 * File:   APFMode.h
 * Author: ze
 *
 * Created on 15 June 2010, 17:30
 */

#ifndef __APFMODE_H__
#define	__APFMODE_H__

#include "AutonomousMode.h"
//#include "yarp/os/Thread.h"

class APFMode : public AutonomousMode {
public:
    APFMode();
    ~APFMode();

//    void initConfiguration(char *file);
    
    void prepare();
    bool action();

    // inherited from Thread and BehaviourMode
    bool stop();

    // inherited from Thread

    void beforeStart();
    void afterStart(bool success);
    void threadRelease();
    void onStop();
    void run();
    bool threadInit();
	virtual void manualSetDrifts();
private:

};

#endif	/* __APFMODE_H__ */

