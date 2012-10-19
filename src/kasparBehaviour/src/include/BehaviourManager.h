#ifndef __BAHAVIOUR_MANAGER__
#define __BAHAVIOUR_MANAGER__

#include <ace/OS_NS_stdio.h>
#include <ace/Vector_T.h>
#include <yarp/os/Network.h>
#include <yarp/os/Port.h>
#include <yarp/os/Bottle.h>
#include <yarp/os/Time.h>
#include <stdio.h>
#include <stdlib.h>
#include <yarp/dev/PolyDriver.h>
#include <yarp/dev/ControlBoardInterfaces.h>
#include <yarp/dev/all.h>
#include <yarp/sig/SoundFile.h>
using namespace yarp::sig::file;

#include <string>

#include "BehaviourMode.h"
//#include "UserMode.h"
#include "AutonomousMode.h"
//#include "utils/KasparPoseFile.h"


using namespace yarp::os;
using namespace yarp::dev;

/**
 * This class is to ensure that only one working mode is selected. It is not recommended to run more than one modes simultaneously, to avoid the device is accessed by multiple modes and also some modes will start separate threads, which need to be stopped either. 
 * To change the mode, it is recommended to stop one mode and start another one. 
 * Therefore, it is needed to use this BehaviourManager.
 *
 * Also, Kaspar's configuration, including motors, sensors, and others should be also managed here. And these configuration properties should be assigned to BehaviourMode
 */
class BehaviourManager
{
public:
	enum BehaviourModeType { USER_MODE, AUTO_MODE, HYBRID_MODE, INVALID_MODE};

    BehaviourManager();
	~BehaviourManager();

	bool initMotorDriver(PolyDriver *pd);
    bool initAnalogDriver(PolyDriver *pd);

	/**
	* @param mode: user commanded or autonomous. 
	* possible modes include: 
	* 1. User mode
	* 2. Autonomous mode,
	* --> APF
	* --> Deliberate rule-based. e.g. touch1 -> reaction1, touch2 -> reaction2... Defined in XML? 
	* --> Sampling-based control. 
	*/
	bool setBehaviourMode(BehaviourModeType m);
	BehaviourManager::BehaviourModeType getBehaviourMode();
	void getCurrentMode(BehaviourMode *bm);
    void initConfiguration(char *filename);
	bool sendCommand(std::string command); 

private:
    PolyDriver *dd;  //polydriver for motor drivers
    IPositionControl *pos;
	IEncoders *enc;

    PolyDriver *ad; // polydriver for analog sensors (tactile)
    IAnalogSensor *ana;
	
	PolyDriver soundDevice;  // Sound device (portaudio)
	IAudioRender *audioOut;
    //UserMode *userMode;
    AutonomousMode *autoMode;
	BehaviourMode *currentMode;
	//yarp::os::Thread *th;
        
    // mode: user commanded or autonomous.
	// possible modes include: 
	// 1. User mode
	// 2. Autonomous mode,
	// --> APF
	// --> Deliberate rule-based. e.g. touch1 -> reaction1, touch2 -> reaction2... Defined in XML? 
	// --> Sampling-based control. 
	BehaviourModeType mode;
};

#endif
