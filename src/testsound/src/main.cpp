/*
 * Copyright (C) 2011 Ze Ji
 * University of Hertfordshire. 
 * EU FP7 RoboSkin 
 * CopyPolicy: Released under the terms of the GPL v2.0, see GPL.TXT
 */
#include <yarp/os/all.h>
#include <iostream>
#include <errno.h>
#include <vector>
#include <string>
#include "SoundPlay.h"
#include "TestSound.h"
using namespace std;


int main(int argc, char *argv[])
{
  //yarp::os::Network yarp;
  // IMPORTANT
  // TO BE ABLE TO FIND KASPAR Devices
  //YARP_REGISTER_DEVICES(kasparmod)
/*
	  ActionExecutor ae;
  ae.setName("/executor");

  yarp::os::ResourceFinder rf;

  rf.setVerbose(true);
  rf.setDefaultConfigFile("config_actionexecutor.ini");
  rf.configure("KASPAR_ROOT",argc, argv);
  return ae.runModule(rf);
  */
	//TestSound ts;
	//ts.play();
	//ts.play();
	//	ts.start();
	SoundPlay ss;
	ss.play("byebye.wav","D:/roboskin/coding/KasparRoboSkinConf/Kaspar1cData/sounds");
	yarp::os::Time::delay(2);
	return 0;
}
