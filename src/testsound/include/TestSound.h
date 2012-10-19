#ifndef _TESTSOUND_
#define _TESTSOUND_
#include <yarp/os/Thread.h>
#include <yarp/os/Time.h>
#include "SoundPlay.h"
class TestSound : public yarp::os::Thread
{
public:
    virtual void run()
	{
		while(true)
		{  	

			yarp::os::Time::delay(2);
			std::cout << "Running... \n";
		}
	}

	void play()
	{
			sp.play("hahaha.wav", "D:/roboskin/coding/KasparRoboSkinConf/Kaspar1cData/sounds/");
	}
protected:

	SoundPlay sp;


};

#endif