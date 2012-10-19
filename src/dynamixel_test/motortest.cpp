// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-
#include <yarp/os/Network.h>
#include <yarp/os/Port.h>
#include <yarp/os/Bottle.h>
#include <yarp/os/Time.h>
#include <stdio.h>
#include <stdlib.h>
#include <yarp/dev/PolyDriver.h>
#include <yarp/dev/ControlBoardInterfaces.h>

#include <iostream>

// IMPORTANT
// TO BE ABLE TO FIND KASPAR Devices
#ifdef USE_KASPAR_MOD
#include "drivers.h"
#endif

using namespace yarp::os;
using namespace yarp::dev;

int main(int argc, char *argv[])
{
	Network yarp;
	
	// IMPORTANT
	// TO BE ABLE TO FIND KASPAR Devices
    #ifdef USE_KASPAR_MOD
	yarp::dev::DriverCollection dev;
	#endif

	printf("SDLJSLKdjfklSJ\n");
	Property config;
	
	//config.fromCommand(argc, argv);
	//config.put("device", "KasparServos");
	config.fromConfigFile("D:/roboskin/coding/roboskin/src/modules/kasparServos/KasparRoboSkin.cfg");
	//Property options;

	//options.put("device", "fsrTouchSensor");
	//options.put("device", "remote_controlboard");
	PolyDriver dd;
	dd.open(config);
	
	/*CAUTION::::: device here should be remote_controlboard, rather than dynamixelAX12FtdiDriver*/
	//http://wikiri.upc.es/index.php/YARP_devices
	//http://eris.liralab.it/wiki/YARP_Motor_Control
	//http://eris.liralab.it/yarpdoc/d1/d88/classyarp_1_1dev_1_1RemoteControlBoard.html
	//PolyDriver dd("(device remote_controlboard) (local /client) (remote /controlboard)");
	if (!dd.isValid())
	{
		printf("Device not available.\n");
	//	Network::fini();
		return 1;
	}

	IPositionControl *pos;
	ITorqueControl *tor;
	IEncoders *enc;
	//yarp::os::Thread *th;
	if(!dd.view(pos))
    {
        printf("Problem of open pos device\n");
        return 0;
	}
pos->setRefSpeed(9, 10);
	std::cout << pos->positionMove(9, 150) << std::endl;
	/*
	// Test for SSC32 servos
	pos->setRefSpeed(6, 10);
	std::cout << pos->positionMove(6, 0) << std::endl;
	//Time::delay(1);
	//pos->setRefSpeed(3, 0.1);
	std::cout << pos->positionMove(6, 0) << std::endl;
	//Time::delay(1);
	//pos->setRefSpeed(4, 0.1);
	pos->setRefSpeed(5, 30);
	pos->setRefSpeed(4, 80);
	pos->setRefSpeed(3, 80);
	
	std::cout << pos->positionMove(5, 0) << std::endl;
	std::cout << pos->positionMove(4, 0) << std::endl;
		std::cout << pos->positionMove(3, -40) << std::endl;
		Time::delay(1);
		std::cout << pos->positionMove(3, 40) << std::endl;
	*/
	///*
		if(!dd.view(tor))
    {
        printf("Problem of open tor device\n");
        return 0;
	}

		tor->setRefTorque(9, 1023);

		Time::delay(2);
		tor->setRefTorque(9, 0);

		Time::delay(2);
		tor->setRefTorque(9, 1023);

		Time::delay(2);
		//tor->setRefTorque(9, 1023);
		/*
	if(!dd.view(enc))
    {
        printf("Problem of open enc device\n");
        return 0;
	}

    int jnts = 5;
	double t = 0;
	double p = 0;
	pos->getAxes(&jnts);
	std::cout << jnts << std::endl;
	pos->setRefSpeed(4, 23);
	std::cout << pos->positionMove(4, 90) << std::endl;
	pos->setRefSpeed(6, 20);
	std::cout << pos->relativeMove(6, 10) << std::endl;
	enc->getEncoder(4, &p);
	std::cout << "Position for joint 4 is --> " << p << std::endl;
//	printf("axes: %d\n", jnts);

	Time::delay(1);
	pos->setRefSpeed(2, 23);
	//yarp::os::Searchable c;

	//    std::cout << pos->positionMove(2, 15) << std::endl;
	//Time::delay(1);
	pos->setRefSpeed(4, 9);
	//yarp::os::Searchable c;

	std::cout << pos->positionMove(4, 100) << std::endl;
	yarp::os::Time::delay(1);
	//sleep(1);
	//tor->getTorque(4, &t);
	//std::cout << t << std::endl;

	std::cout << pos->relativeMove(4, 20) << std::endl;
	//sleep(1);
	//pos->relativeMove(4, 40);
    //yarp::os::Time::delay(1);
	//sleep(1);
	//sleep(1);
	for (int ii = 0; ii < 10; ii++)
	{
        //tor->getTorque(4, &t);
	//	enc->getEncoderSpeed(4, &t);
	//	std::cout << "Torque for join t 4 is --> " << t << std::endl;
		//std::cout << pos->positionMove(4, 90 + ii*5) << std::endl;

		//		sleep(1);
		yarp::os::Time::delay(0.11);
		enc->getEncoder(4, &p);
		std::cout << "Position for joint 4 is --> " << p << std::endl;
enc->getEncoder(1, &p);
		std::cout << "Position for joint 1 is --> " << p << std::endl;
		enc->getEncoder(2, &p);
		std::cout << "Position for joint 2 is --> " << p << std::endl;
		enc->getEncoder(3, &p);
		std::cout << "Position for joint 3 is --> " << p << std::endl;
	}
	yarp::os::Time::delay(1);
//	std::cout << pos->positionMove(4, 90) << std::endl;

	    std::cout << pos->relativeMove(4, -20) << std::endl;

	//    std::cout << pos->positionMove(2, 30) << std::endl;
	//    std::cout << pos->positionMove(2, 50) << std::endl;

	Time::delay(1);
*/
	dd.close();
    
	return 0;
}
