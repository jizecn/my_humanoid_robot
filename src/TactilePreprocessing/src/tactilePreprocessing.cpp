#include "tactilePreprocessing.h"

TactilePreprocessing::TactilePreprocessing()
{
	//sensors = NULL;
}

TactilePreprocessing::~TactilePreprocessing()
{
	/*
	 if (sensors)
	 {
	 delete sensors;
	 }
	 */


	for (int i = 0; i < this->skinGroups.size(); i++)
	{
		//cout << "Destructor: stop skingroup : " << skinGroups[i]->getGroupName() << endl;
		skinGroups[i]->stop();
		//cout << "Destructor" << std::endl;
        delete skinGroups[i];
		//TEST: ?? delete skinGroups[i]; ?? 
	}

}

bool TactilePreprocessing::configure(yarp::os::ResourceFinder &sensorCfg)
{
  cout << "TactilePreprocessing configure\n";
  // skin sensors
  //bool ifLogData = sensorCfg.check("logdata");

  //yarp::os::ConstString logpath = sensorCfg.check("logfilepath", yarp::os::Value(""), "specify log file path for data logging if logdata flag is specified").asString();

	/*
  if(!initSkinSensors(sensorCfg))
  {
	  cout << "Cannot init SkinSensors" << endl;
	  return false;
  }
	 
  cout << "OK with initSkinSensors\n";
  */
	if(!initSensorGroups(sensorCfg))
  {
	  cout << "Cannot init SensorGroups" << endl;
	  return false;
  }
  cout << "OK with initSensorGroups\n";

  for(int i = 0; i < this->skinGroups.size(); i++)
  {
	  cout << "start skingroup : " << skinGroups[i]->getGroupName() << endl;
	  if(!skinGroups[i]->start())
	  {
		  cout << "Failed to start sensors thread" << endl;

		  for(int j = 0; j < i; j++)
		  {
			  skinGroups[i]->stop();
		  }
		  return false;
	  }
  }

  /*
  sensors = new SkinSensors(sensorCfg);

  if(!sensors->initConfiguration())
  {
	  cout << "Failed to initialise SkinSensors sensors" << endl;
	  return false;
  }

  if(!sensors->start())
  {
	  cout << "Failed to start sensors thread" << endl;
	  sensors->stop();
	  return false;
  }
  sensors->stop();
*/
	return true;
}

bool TactilePreprocessing::initSkinSensors(yarp::os::ResourceFinder &config)
{
	// skin sensors
	/*
	bool ifLogData = config.check("logdata");
	yarp::os::ConstString
			logpath =
					config.check("logfilepath", yarp::os::Value(""),
							"specify log file path for data logging if logdata flag is specified").asString();

	yarp::os::Bottle skinfiles = config.findGroup("sensorcfgs").tail();
	yarp::os::ConstString skinFilePath = config.check("skinsensorfilepath",
			yarp::os::Value(
					"C:/roboskin/software_yarp_related/iCub/app/kaspar/conf"),
			"specifiy skinsensor config file path").asString();
	printf("%s   %d \n", skinFilePath.c_str(), skinfiles.size());
	for (int i = 0; i < skinfiles.size(); i++)
	{
		yarp::os::Property skinproperty;
		yarp::os::Bottle sensorConf(skinfiles.get(i).toString());
		if (skinproperty.fromConfigFile((skinFilePath + FILESEPARATOR
				+ sensorConf.get(1).asString())))
		{
			// put new attribute ifLog to the property before initialising skin sensors
			if (ifLogData)
			{
				skinproperty.put("logdata", ifLogData);
				skinproperty.put("logfilepath", logpath.c_str());
			}

			SkinBodyPart *sk = new SkinBodyPart(skinproperty); // assuming property file is correct. No error checking and return. so be very careful
			sk->setPortToConnect(sensorConf.get(2).asString());
			sk->setId(sensorConf.get(0).asInt());
			if (sk->initConfiguration())
			{
				partList.push_back(sk);
			}
			else
			{
				delete sk;
			}
		}
		else
		{
			fprintf(stderr, "Error loading skin sensor config file %s\n",
					sensorConf.get(0).asString().c_str());
			return false;
		}
	}

	// sort in the ascending order of the IDs
	// not necessary now... legacy code...
	std::sort(partList.begin(), partList.end(), TactilePreprocessing::compareIdOfSkinPart);
	 */
	return true;
}

bool TactilePreprocessing::initSensorGroups(yarp::os::ResourceFinder &config)
{
	yarp::os::ConstString skingroupfilepath = config.check("skingroupfilepath", yarp::os::Value("conf"), "").asString();
    yarp::os::ConstString skinsensorfilepath = config.check("skinsensorfilepath", yarp::os::Value("skin/kaspar/conf"), "").asString();
    yarp::os::ConstString svmModelPath = config.check("svmmodelfilepath", yarp::os::Value("traindata"), "").asString();
	yarp::os::ConstString logFilePath = config.check("logfilepath", yarp::os::Value("logs"), "").asString();
	bool ifLog = config.check("logdata");

    yarp::os::Bottle skingroups = config.findGroup("sensorgroups").tail();
	cout<< skingroups.size() << endl;
	for(int i = 0; i < skingroups.size(); i++)
	{
		yarp::os::ConstString groupfile = skingroups.get(i).toString();
		cout << groupfile.c_str() << endl;
		SkinGroup *sGroup = new SkinGroup();
		//yarp::os::Property groupCfg;
        yarp::os::Property groupCfg;
        //string fullpath = (skingroupfilepath + yarp::os::ConstString(FILESEPARATOR) + groupfile).c_str();
        string fullpath = (config.getContextPath() + yarp::os::ConstString(FILESEPARATOR) + skingroupfilepath + FILESEPARATOR + groupfile).c_str();
		groupCfg.fromConfigFile(fullpath.c_str());
        groupCfg.put("skinsensorpath", (config.getContextPath() + FILESEPARATOR + skinsensorfilepath).c_str());
		groupCfg.put("svmmodelfilepath", (config.getContextPath() + FILESEPARATOR + svmModelPath).c_str());

		if(ifLog)
		{
			groupCfg.put("logdata", "true");
			groupCfg.put("logfilepath", (config.getContextPath() + FILESEPARATOR + logFilePath).c_str());
		}
		cout << fullpath << "....\n";
		if(!sGroup->initConfiguration(groupCfg))
		{
			cout << "Something is wrong with the config file: " << groupfile.c_str() << endl;
			return false;
		}
		this->skinGroups.push_back(sGroup);
	}
	cout << skinGroups.size() << endl;
	
	/*
    yarp::os::Bottle skingroups = config.findGroup("sensorgroups").tail();

    for(int i = 0; i < skingroups.size(); i++)
    {
        yarp::os::Bottle groupconf(skingroups.get(i).toString());
        SkinGroup* sGroup = new SkinGroup(20);
        if(!sGroup->initConfiguration(groupconf, &partList))
        {
        	return false;
        }
        skinGroups.push_back(sGroup);
    }

    cout << skinGroups.size() << endl;
*/
	return true;
}

bool TactilePreprocessing::interruptModule()
{
	cout << this->skinGroups.size() << endl;
	for(int i = 0; i < this->skinGroups.size(); i++)
	{
		cout << "Try to stop skingroup  " << i << skinGroups.at(i)->getGroupName() << "   " << this->skinGroups.size() << endl;
		skinGroups[i]->stop();
		      delete skinGroups[i];
	//skinGroups[i]->stupidCloseFile();
	}
	return true;
}
/*
bool TactilePreprocessing::close()
{

	  for(int i = 0; i < this->skinGroups.size(); i++)
	  {
		  cout << "stop skingroup : " << skinGroups[i]->getGroupName() << endl;
		  skinGroups[i]->stop();
	  }
	return true;
}*/

bool TactilePreprocessing::respond(const Bottle& command, Bottle& reply)
{
	return true;
}

bool TactilePreprocessing::updateModule()
{
	//yarp::os::Time::delay(1);
	cout << "Still running----     ";
	return true;
}


bool TactilePreprocessing::initLogFile(yarp::os::ResourceFinder  &config)
{
	// if log data to file
	return false;
}

bool TactilePreprocessing::compareIdOfSkinPart(SkinBodyPart* a, SkinBodyPart* b)
{
    return (a->getId() < b->getId());
}

