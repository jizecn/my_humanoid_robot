// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-
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
#include <iostream>
#include <yarp/os/RateThread.h>

using namespace yarp::os;
using namespace yarp::dev;

using namespace std;
// IMPORTANT
// TO BE ABLE TO FIND KASPAR Devices
#ifdef USE_KASPAR_MOD
#include "drivers.h"
#endif



int main(int argc, char *argv[])
{
  // IMPORTANT
  // TO BE ABLE TO FIND KASPAR Devices
#ifdef USE_KASPAR_MOD
  yarp::dev::DriverCollection dev;
#endif
  
  //Network yarp;
  //Port p;   
  //p.open("/out");    // Give it a name on the network.
  
  Property config;
  
  config.fromCommand(argc, argv);
  
  Property options;
  //options.put("device", "qFsrSensor");
  options.put("device", "UHskinprototype");
  options.put("canbusdevice", "ecan");
  options.put("CanDeviceNum", 0);
  options.put("SkinCanIds", "15");
  options.put("Period", 500);
  //PolyDriver dd(config);
  PolyDriver dd(options);
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

  RateThread *rt;
  if(!dd.view(rt))
      {
      printf("Problem of open ia device\n");
      return 1;
      }
  rt->start();

  printf("hello\n");
  yarp::os::Time::delay(2);
  rt->stop();

  return 0;
}
