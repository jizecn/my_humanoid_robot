// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#define FILTER_ORDER 16

#include <yarp/os/Network.h>
#include <yarp/os/Port.h>
#include <yarp/os/Bottle.h>
#include <yarp/os/Time.h>
#include <stdio.h>
#include <stdlib.h>
#include <yarp/dev/PolyDriver.h>
#include <yarp/dev/ControlBoardInterfaces.h>
#include <yarp/dev/IAnalogSensor.h>
#include <yarp/sig/Vector.h>
#include <yarp/sig/Matrix.h>
#include <iostream>

using namespace yarp::os;
using namespace yarp::dev;

yarp::sig::Matrix previousData;

double calculateSaliency()
{
    return *previousData[0,0];
    return 0;
}

int main(int argc, char *argv[])
{
	Network yarp;

    Port p;   
	p.open("/saliency/out");    // Give it a name on the network.
	Port inPort;
	inPort.open("/saliency/in");
	
	Property config;
	
	config.fromCommand(argc, argv);
	Bottle b; 
    int size = 0;

    previousData.resize(16, FILTER_ORDER);
    previousData.zero();
    p.write(previousData);
	
    double temp = 0; 
    do {
		inPort.read(b);
        if(b.isNull())
        {
            continue;
        }
        size = b.size();
        
        // TODO: 
        temp = calculateSaliency();

        p.write(previousData);
	} while(true);

/*
	Property options;

	options.put("device", "qFsrSensor");
    
	PolyDriver dd(config);
	//PolyDriver dd("(device qFsrSensor) (comport COM9) (baudrate 115200)");

	if (!dd.isValid())
	{
		printf("Device not available.\n");
		//Network::fini();
		return 1;
	}

    IAnalogSensor *ia; 
    
	if(!dd.view(ia))
    {
        printf("Problem of open ia device\n");
        return 1;
	}

    yarp::sig::Vector dv;// = new yarp::sig::Vector(); 
    dv.resize(16);

    Time::delay(2);
	yarp::os::Bottle bot;
	yarp::os::Bottle bot1;
    for(int j = 0; j < 100; j++)
    {
		cout << "Start " << j << "\n";
	    ia->read(dv);

        for(int i = 0; i < dv.size(); i++)
        {
    		cout << dv[i] << endl;
		}
	    
		yarp::os::Value vv(dv.data(), sizeof(double)*16);
		
		bot.add(vv);
		p.write(bot);      // Send the data.
		bot.clear();
		Time::delay(1);
	}

	dd.close();
	*/
	return 0;
}
