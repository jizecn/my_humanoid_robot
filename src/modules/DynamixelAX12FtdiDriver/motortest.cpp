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

//using namespace yarp::os;
using namespace yarp::dev;

int main(int argc, char *argv[]) {
    yarp::os::Network yarp;
    /*
    if(argc!=7) {
        printf("  Error: correct usage is %s --board BOARD --comport COM --baudrate BAUDRATE\n", argv[0]);
        printf("          BOARD is one out of ssc32, minissc, pontech_sv203x, mondotronic_smi, pololu_usb_16servo, picopic\n");
        printf("          COM is COMx or /dev/ttyS0\n");
        printf("          BAUDRATE is the baud rate, eg: 38400");

      exit(1);
    }
    */
    yarp::os::Property config;
    config.fromCommand(argc, argv);

    yarp::os::Property options;
    //options.put("robot", "icub"); // typically from the command line.
    //options.put("device", "SerialServoBoard");
    options.put("device", "dynamixelAX12FtdiDriver");
    //options.put("device", "ax12motor");
    //options.put("board", config.check("board", yarp::os::Value("ax12")).asString().c_str());
    //options.put("comport", config.check("comport", yarp::os::Value("/dev/ttyS0")).asString().c_str());
    //options.put("baudrate", config.check("baudrate", yarp::os::Value(38400)).asInt());

    PolyDriver dd(options);
    
    if(!dd.isValid()) {
      printf("Device not available.\n");
      yarp::os::Network::fini();
      return 1;
    }
     
    IPositionControl *pos;
	ITorqueControl *tor;
	
    dd.view(pos);
	dd.view(tor);
    int jnts = 5;
    double t = 0;

    pos->getAxes(&jnts);
    std::cout << jnts << std::endl;
    //printf("axes: %d\n", jnts);

    //Time::delay(1);
    pos->setRefSpeed(2, 23);
    //yarp::os::Searchable c;
   
    //    std::cout << pos->positionMove(2, 15) << std::endl;
    //Time::delay(1);
    pos->setRefSpeed(4, 10);
    //yarp::os::Searchable c;
   
 //   std::cout << pos->positionMove(4, 90) << std::endl;
   // sleep(1);
tor->getTorque(4, &t);
    std::cout << t << std::endl;

   // std::cout << pos->relativeMove(4, 20) << std::endl;
    //sleep(1);  
	for(int ii = 0; ii < 1000; ii++)
	{
	tor->getTorque(4, &t);
    std::cout << t << std::endl;
}
//    std::cout << pos->relativeMove(4, -20) << std::endl;

    tor->getTorque(4, &t);
    std::cout << t << std::endl;
    //    std::cout << pos->positionMove(2, 30) << std::endl;
    //    std::cout << pos->positionMove(2, 50) << std::endl;
    
    yarp::os::Time::delay(1);
    
    dd.close();

    return 0;
}
