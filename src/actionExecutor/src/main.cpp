/*
 * Copyright (C) 2011 Ze Ji
 * University of Hertfordshire. 
 * EU FP7 RoboSkin 
 * CopyPolicy: Released under the terms of the GPL v2.0, see GPL.TXT
 */

#include <iostream>
#include <yarp/os/RFModule.h>
#include <yarp/os/ResourceFinder.h>
#include <yarp/os/RpcClient.h>
#include <sys/types.h>
#ifdef __linux__
#include <dirent.h>
#endif
#include <errno.h>
#include <vector>
#include <string>

#include "ActionExecutor.h"
using namespace std;

// IMPORTANT
// TO BE ABLE TO FIND KASPAR Devices
YARP_DECLARE_DEVICES(kasparmod)


int main(int argc, char *argv[])
{
  cout << "ActionExecutor module for Kaspar. RoboSkin project. " << endl;
  yarp::os::Network yarp;
  // IMPORTANT
  // TO BE ABLE TO FIND KASPAR Devices
  YARP_REGISTER_DEVICES(kasparmod)
  ActionExecutor ae;
  ae.setName("/executor");

  yarp::os::ResourceFinder rf;

  rf.setVerbose(true);
  rf.setDefaultConfigFile("config_actionexecutor.ini");
  rf.configure("KASPAR_ROOT",argc, argv);
  return ae.runModule(rf);
}
