#ifndef __TACTILE_PROPRECESSING_H__
#define __TACTILE_PROPRECESSING_H__

#include <iostream>
#include <string>
#include <vector>

#include <yarp/os/RFModule.h>
#include <yarp/os/Network.h>
#include <yarp/os/Time.h>
#include <yarp/os/Bottle.h>
#include "DataSample.h"
#include "utils/Resources.h"
//#include "SkinSensors.h"
#include "SkinGroup.h"
using namespace yarp::os; 

class TactilePreprocessing : public RFModule
{
public: 
  TactilePreprocessing();
  ~TactilePreprocessing();
  bool configure(yarp::os::ResourceFinder &rf); // configure all the module parameters and return true if successful
  bool interruptModule();                       // interrupt, e.g., the ports 
  //bool close();                                 // close and shut down the module
  bool respond(const Bottle& command, Bottle& reply);
  //double getPeriod();

  bool updateModule();

private:
  static bool compareIdOfSkinPart(SkinBodyPart* a, SkinBodyPart* b);
  bool initLogFile(ResourceFinder &config);
  //SkinSensors *sensors;
  bool initSensorGroups(yarp::os::ResourceFinder &config);
  bool initSkinSensors(yarp::os::ResourceFinder &config);
	std::vector<SkinBodyPart *> partList;
	std::vector<SkinGroup*> skinGroups;
};

#endif //__TACTILE_PROPRECESSING_H__
