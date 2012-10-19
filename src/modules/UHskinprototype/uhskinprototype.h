// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __SKIN_MESH_THREAD_H__
#define __SKIN_MESH_THREAD_H__

//#include <stdio.h>
#include <string>

#include <yarp/os/RateThread.h>
#include <yarp/os/Semaphore.h>
#include <yarp/dev/ControlBoardInterfaces.h>
#include <yarp/dev/IAnalogSensor.h>
#include <yarp/dev/PolyDriver.h>
#include <yarp/dev/CanBusInterface.h>
#include <yarp/sig/Vector.h>
#include <ace/OS_NS_stdio.h>
#include <ace/Vector_T.h>

#include "card.h"

using namespace yarp::os;
using namespace yarp::dev;

class UHSkinPrototype : public RateThread, public yarp::dev::IAnalogSensor, public DeviceDriver 
{
protected:
	PolyDriver driver;
    ICanBus *pCanBus;
    ICanBufferFactory *pCanBufferFactory;
    CanBuffer inBuffer;
    CanBuffer outBuffer;
   
    yarp::os::Semaphore mutex;

    int cardId;
    int sensorsNum;

    yarp::sig::Vector data;

    int numOfCards;
    ACE_Vector<Card> cards;
    //Card *cards; // a list of cards (sub-canbus (i2c), on microcontroller)

    void initCards(Bottle ids);

public:
    UHSkinPrototype(int period=20) : RateThread(period),mutex(1)
    {}
    
    ~UHSkinPrototype()
    {
    }

    virtual bool open(yarp::os::Searchable& config);
    virtual bool close();
       
    //IAnalogSensor interface
    virtual int read(yarp::sig::Vector &out);
    virtual int getState(int ch);
    virtual int getChannels();
    virtual bool calibrate(int ch, double v);

	virtual bool threadInit();
    virtual void threadRelease();
    virtual void run();

};

#endif
