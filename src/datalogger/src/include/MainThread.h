#ifndef _MAIN_THREAD_
#define _MAIN_THREAD_

#include <yarp/os/RateThread.h>

using namespace yarp::os;

class MainThread : public RateThread {
public:
	MainThread(Property &config, int period) : RateThread(period)
	{
	}
	~MainThread()
	{
	}

	void run()
	{
	}
};

#endif
