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

#include "ActionDecision.h"
using namespace std;

int main(int argc, char * argv[])
{
  cout << "ActionDecision module for Kaspar. RoboSkin project. " << endl;
  Network yarp;
  // main function: receives events, and decide action for Kaspar
  ActionDecision ad;
  ad.setName("/worker");
  //ad.start();
  //ad.attachTerminal();
  yarp::os::ResourceFinder rf;
  rf.setVerbose(true);
  rf.setDefaultConfigFile("config_actiondecition.ini");
  rf.configure("KASPAR_ROOT",argc, argv);
  return ad.runModule(rf);
  // meanwhile, listen to RPC command. There could be a GUI to control the behaviour of Kaspar on the fly via RPC control.
}
