#include "SkinGroup.h"

SkinGroup::SkinGroup() :
	Thread()
{
sentNoneEvent = false;
}

SkinGroup::~SkinGroup()
{
	for (size_t i = 0; i < this->skinParts.size(); i++)
	{
		cout << "Disconnect ...\n";
		skinParts[i]->disconnect();
	}

	classificationOutport.close();

	for (size_t i = 0; i < this->skinParts.size(); i++)
	{
		
		if (skinParts[i])
		{
			//partList[i]->disconnect();
			delete skinParts[i];
		}
	}	
closeFile();
	delete pLogFile;

}

bool SkinGroup::initConfiguration(yarp::os::Property &conf)
{

  //int s = conf.size();
  //this->groupName = conf.get(0).asString().c_str();
  
  yarp::os::Value v = conf.find("groupname");
  
  if(v.isNull())
    {
      printf("Not Group Name specified\n");
      return false;
    }
  else {
    this->groupName = v.asString().c_str();
  }
  

	initLogFile(conf);

  yarp::os::Value v2 = conf.find("parts");
  
  if(v2.isNull()) {
    return false;
  }
  else {
    // add all parts to this group
    
    if(!initSensors(conf))
      {
			cerr << "ERROR: initSensors error. (SkinGroup)" << endl;
			return false;
		}
	}

	/*
	for (int j = 1; j < conf.size(); j++)
	{
		cout << conf.get(j).asString() << endl;
		int k = 0;
		for (k = 0; k < parts->size(); k++)
		{
			if (parts->at(k)->getPartName() == string(
					conf.get(j).asString().c_str()))
			{
				cout << "part : " << parts->at(k)->getPartName().c_str()
						<< " is found and added to SkinGroup  "
						<< this->groupName << endl;
				this->skinParts.push_back(parts->at(k));
				break;
			}
		}
		if (k == parts->size())
		{
			// Not found. Error
			cerr << "Sensor " << conf.get(j).asString().c_str()
					<< " for group " << this->groupName << " is not available."
					<< endl;
			return false;
		}
	}
	// not necessary. just to double check
	if (this->skinParts.size() != conf.size() - 1)
	{
		// Not found. Error
		cerr << "Not all sensors in sensorgroup " << this->groupName
				<< " are available. Check the config files" << endl;
		return false;
	}
	 */
	cout << "OK. Sensors added for sensorgroup " << this->groupName << endl;

	// init other settings, e.g. offsets for each sensor in the whole data set

	taxelDimension = 0;
	for (size_t k = 0; k < skinParts.size(); k++)
	{
		offsets.push_back(taxelDimension);
		taxelDimension += skinParts[k]->getAllNumTaxels();
	}

	// other variables

	tempTactileData = new TactileData(taxelDimension);

	// load svm model file
	// TODO:
	yarp::os::ConstString _svmmodelfile = conf.find("svmmodel").asString();
    _svmmodelfile = conf.check("svmmodelfilepath", yarp::os::Value("traindata"), "").asString() + FILESEPARATOR + _svmmodelfile;

	this->svmmodelfile = _svmmodelfile.c_str();

	cout << "SKINGROUP : " << this->groupName << " has " << this->taxelDimension << " taxels" << endl;
	//this->classification = new TactileClassificationRawData(this->taxelDimension);
	this->classification = new TactileClassificationHist(this->taxelDimension);
	if(!this->classification->loadSVMModel(svmmodelfile.c_str()))
	{
		cout << "ERROR: cannot load SVMModel file " << svmmodelfile.c_str() << endl;
		return false;
	}

	yarp::os::Property custProp;
	custProp.put("normalisation", "none");
	custProp.put("hist_num_bins", 20);
	classification->customConfiguration(custProp);

	tState = new TactileState(TIME_DIMENSION, taxelDimension, 50);

	// init port for output broadcasting
	initOutport(conf);
	return true;
}

void SkinGroup::initOutport(yarp::os::Property &config)
{
  std::string outPortName = config.check("out_port", yarp::os::Value("/outport"), "").asString().c_str();
        
  std::cout << "Out Port name ==>" << outPortName << std::endl;
  classificationOutport.open(outPortName.c_str());
}


bool SkinGroup::initLogFile(yarp::os::Property &config)
	{

	ifLogData = config.check("logdata");
	std::string logPath = config.check("logfilepath", yarp::os::Value("logs"), "specify log file path for data logging if logdata flag is specified").asString().c_str();
	

	// if log data to file
//		ifLogData = config.check("logdata");
		yarp::os::Value nameValue = config.find("childName");
		//yarp::os::Value logPathValue = config.find("logfilepath");
		std::string childName = "";
		//string logpath = ""; //config.find("logfilepath").asString().c_str();

		if(!nameValue.isNull())
		{
			childName = nameValue.asString().c_str();
			printf("Child name is %s\n", childName.c_str());
		}
		//if(!logPathValue.isNull())
		//{
		//	logpath = logPathValue.asString().c_str();
			printf("Log File Path is 5s\n", logPath.c_str());
		//}
		if(ifLogData)
		{
			time_t rawtime;
			struct tm * timeinfo;
			time ( &rawtime );
			timeinfo = localtime ( &rawtime );
			std::string timeText = to_string(timeinfo->tm_sec) + "_" + to_string(timeinfo->tm_min) + "_" + to_string(timeinfo->tm_hour) + "_" + to_string(timeinfo->tm_mday)+ "_" + to_string(timeinfo->tm_mon + 1)+ "_" + to_string(timeinfo->tm_year + 1900);//asctime(timeinfo);

			if(childName != "")
			{
				logFile = this->getGroupName() + "_" + childName + "_" + timeText;
			}
			else
			{
				logFile = this->getGroupName() + "_" + timeText;
			}

			if(logPath != "")
			{
				logFile = (logPath + FILESEPARATOR + logFile).c_str();
			}

			printf("LOG DATA of part %s\n", logFile.c_str());
			pLogFile =  fopen (logFile.c_str() , "w");

			if (pLogFile == NULL) {
				fprintf(stderr, "Error opening file %s\n", logFile.c_str());
				fprintf(stderr, "Log File disabled\n");
				ifLogData = false;
			}
			else
			{
				// write head info. 

				this->logText("version", "0.1");

				this->logText("partname", this->getGroupName().c_str());

			//int dim = this->sensorsNum*12;
				//std::string s;
				//std::stringstream out;
				//out << dim;
				//s = out.str();
				//this->logText("dimension", s.c_str());
			}
		}
		else
		{
			printf("NOT LOG DATA\n");
		}

		return ifLogData;
	}


bool SkinGroup::initSensors(yarp::os::Property &config)
{

    yarp::os::Bottle skinfiles = config.findGroup("parts").tail();

    yarp::os::ConstString skinFilePath = config.check("skinsensorpath", yarp::os::Value("C:/roboskin/software_yarp_related/iCub/app/kaspar/conf"), "specifiy skinsensor config file path").asString();
//    yarp::os::ConstString skinFilePath = config.check("skinsensorfilepath", yarp::os::Value("C:/roboskin/software_yarp_related/iCub/app/kaspar/conf"), "specifiy skinsensor config file path").asString();
    
    printf("==== %s   %d \n", skinFilePath.c_str(), skinfiles.size());
    for(int i = 0; i < skinfiles.size(); i++)
    {
        yarp::os::Property skinproperty;
        yarp::os::Bottle sensorConf(skinfiles.get(i).toString());
        if(skinproperty.fromConfigFile((skinFilePath + FILESEPARATOR + sensorConf.get(0).asString())))
        {
            // put new attribute ifLog to the property before initialising skin sensors
            //if(ifLogData)
            //{
            //    skinproperty.put("logdata", ifLogData);
            //    skinproperty.put("logfilepath", logpath.c_str());
            //}
			
            SkinBodyPart *sk = new SkinBodyPart(skinproperty);   // assuming property file is correct. No error checking and return. so be very careful
            sk->setPortToConnect(sensorConf.get(1).asString());
            
            if(sk->initConfiguration())
            {
                this->skinParts.push_back(sk);
            }
            else
            {
                delete sk;
				return false;
            }
		}
        else
        {
            fprintf(stderr, "Error loading skin sensor config file %s\n", sensorConf.get(0).asString().c_str());
			return false;
        }
    }
	return true;
}

int SkinGroup::getNumOfSkinPart()
{
	return this->skinParts.size();
}

int SkinGroup::getIndexOfSkin(string &skinPartName)
{
	for (int i = 0; i < this->skinParts.size(); i++)
	{
		if (skinPartName == this->skinParts[i]->getPartName())
		{
			return i;
		}
	}

	// not found
	return -1;
}

bool SkinGroup::setDataOfSkinPart(string &skinPartName, double *data)
{
	int pos = this->getIndexOfSkin(skinPartName);
	if (pos == -1)
	{
		return false;
	}

	return true;
}

int SkinGroup::getNumOfTaxels()
{
	return this->taxelDimension;
}

string SkinGroup::getGroupName()
{
	return groupName;
}

bool SkinGroup::threadInit()
{
  cout << "Init Thread ..........  " << this->groupName << endl;

	for (size_t i = 0; i < this->skinParts.size(); i++)
	{
		if (!skinParts[i]->tryconnect())
		{
			cout << "Skin Sensor is not connected:  " << i
					<< skinParts[i]->getPartName() << endl;
			return false;
		}
		//yarp::os::Time::delay(1);
		cout << "OK connected. " << skinParts[i]->getPartName() << "   " << this->taxelDimension << endl;
	}
	return true;
}

void SkinGroup::afterStart(bool s)
{
	if (s)
		printf("Thread started successfully\n");
	else
		printf("Thread did not start\n");
}

void SkinGroup::run()
{
	while(true)
	{
		mutex.wait();
		//cout << "Run Thread ..........  " << this->groupName << "   "  << this->taxelDimension << endl;
		for (size_t i = 0; i < skinParts.size(); i++)
		{
			yarp::sig::Vector* compensatedData =
				skinParts[i]->getCompensatedDataPort()->read();//
			//yarp::sig::Vector* compensatedData = (partList[i]->acquireData());

			if (compensatedData == NULL)
			{
				cout
					<< "Could not get data... Use old data instead. But there might be a problem with the sensors/YARP/CanUSB/etc"
					<< endl;
				continue;
			}
			else
			{
				//cout << "OK ---" << compensatedData->size() << "----" << endl;
			}

			unsigned int offset = offsets[i];

			vector<int> sensorIds = skinParts[i]->getAvailableSensorIds();
			double *tempData = compensatedData->data();
			//cout << "Received data size is ----> " << compensatedData->size() << endl;

			for (int j = 0; j < skinParts[i]->getSize(); j++)
			{
				//cout << "j :   " << j << "   " << sensorIds[j] << "  " << sensorIds[j]*NUM_TAXEL_TRIANGLE << "  " <<  offset + j*NUM_TAXEL_TRIANGLE << endl;

				if (!tempTactileData->assignData(tempData + sensorIds[j]* NUM_TAXEL_TRIANGLE, offset + j * NUM_TAXEL_TRIANGLE, NUM_TAXEL_TRIANGLE, true))
				{
					cout << "CANNOT ASSIGN DATA INTO DATASAMPLE.......... " << this->groupName << endl;
				}
			}
		}

		tState->insertDataSample(*tempTactileData, false);
		if (!tState->updateSalienceMap(TactileState::timeDiff))
		{
			cerr << "ERROR: CANNOT UPDATE SALIENCE MAP\n";
		}
		//bool TactileState::updateClassification( methodForClassification &methodClassify)
		// 	int classifyPattern(DataSample &ds, methodForClassification &methodClassify);
		/*
		if (!tState->updateClassification(TactileState::SVM_RBF))
		{
		cerr << "ERROR: Cannot perform classification\n";
		}
		*/

		if(this->ifLogData)
		{
			logData(tempTactileData);
		}

		if (!tempTactileData->aboveThreshold())
		{
			//cout << "No touch detected " << endl;
			//return TactileState::none;
			tState->temporalPatternClassify(zjMath::NO_EVENT);
			if(!sentNoneEvent)
			{
				if(!this->sendResult("NONE", "NONE"))
				{
					std::cout << "No valid result sent" << std::endl;
				}
				else
				{
					std::cout << "sent touch class result: NONE" << std::endl;
				}
			}
			sentNoneEvent = true;
		}
		else 
		{
			// only log data when there are events. 
			/*
			if(this->ifLogData)
			{
				logData(tempTactileData);
			}*/
			

			this->classification->prepareSVMNode(*tempTactileData, false);
			int touchClass = this->classification->performClassification();

			std::string softHard = "NONE";

			double maxValue = tempTactileData->max();

			/*
			if(maxValue > zjMath::THRESHOLD_SOFT_HARD)
			{
				softHard = "HARD";
			}
			else
			{
				softHard = "SOFT";
			}
			*/

			//cout << touchClass << endl;
			//cout << "Perform Classification for part: " << this->groupName << ". Predicted label is:  " << touchClass << endl;

			std::string strClass = tState->temporalPatternClassify(touchClass);

			// everything is done here. Need to send result out of port via YARP now.    
			//std::cout << "Sending out result to port " << this-> classificationOutport.getName().c_str() << std::endl;
			//if(!this->sendResult(touchClass))
			if(!this->sendResult(strClass, softHard))
			{
				std::cout << "No valid result sent" << std::endl;
			}
			else
			{
				std::cout << "sent touch class result: " << strClass << std::endl;
				sentNoneEvent = false;
			}

			//yarp::os::Time::delay(0.1);	
		}

		mutex.post(); 
	}
}

void SkinGroup::logData(TactileData *data)
{
	for( int i = 0; i < data->size(); i++)
	{
		fprintf(pLogFile, "%7.3f ", (data->data())[i]);
	}
	
	time_t rawtime;
	struct tm * timeinfo;
	time(&rawtime);
	timeinfo = localtime ( &rawtime );
	//std::string timeText = asctime(timeinfo);

	fprintf(pLogFile, " #  %f ", yarp::os::Time::now());
	fputs(asctime(timeinfo), pLogFile);   // asctime() will add "\n" automatically 
}

std::string SkinGroup::lookUpTouchType(int classLabel)
{
	std::string touchType = "";
	switch(classLabel)
	{
	case 1:
		touchType = "touch";
		break;
	case -1:
		touchType = "poke";
		break;
	default:
		std::cout << "Invalid class label. "<< std::endl;
		break;
	}
	return touchType;
}

bool SkinGroup::sendResult(int classLabel)
{
	std::string touchType = lookUpTouchType(classLabel);
	if(touchType == "")
	{
		std::cout << "Invalid class label" << std::endl;
		return false;
	}

	std::string res = "(PARTNAME " + this->getGroupName() + ") " + "(TYPE " + touchType + ") " + " (STRENGTH " + "NONE" + ")";
	std::string header = "(msg_type touchClasses) (num_of_touch_classes 1)";
	//std::cout << res.toString() << endl;
    yarp::os::Bottle &b = classificationOutport.prepare();
	b.clear();

	b.add(yarp::os::Value(header.c_str()));
	b.add(yarp::os::Value(res.c_str()));
    
    classificationOutport.write();

	//std::cout << "Successfully sent out result to port " << this->classificationOutport.getName().c_str() << std::endl;
	return true;
}

bool SkinGroup::sendResult(string touchType, string strength)
{
	if(touchType == "")
	{
		std::cout << "Invalid class label" << std::endl;
		return false;
	}

	std::string res = "(PARTNAME " + this->getGroupName() + ") " + "(TYPE " + touchType + ") " + " (STRENGTH " + strength + ")";
	std::cout << res << std::endl;
	std::string header = "(msg_type touchClasses) (num_of_touch_classes 1)";
    yarp::os::Bottle &b = classificationOutport.prepare();
	b.clear();

	//yarp::os::Property header;
	//  header.put("msg_type", "touchClasses");
    // touchClasses, salience, keyboard
    
	//    header.put("num_of_touch_classes", 1);
	b.add(yarp::os::Value(header.c_str()));
	b.add(yarp::os::Value(res.c_str()));
    
    classificationOutport.write();
	
	return true;
}

void SkinGroup::threadRelease()
{
}

void SkinGroup::onStop()
{
	closeFile();
	cout << "OK\n";
}

void SkinGroup::closeFile()
{
	if(ifLogData && pLogFile != NULL)
	{
		cout << "Close log file..." << this->getGroupName() << endl;
		fclose(pLogFile);
	}
}