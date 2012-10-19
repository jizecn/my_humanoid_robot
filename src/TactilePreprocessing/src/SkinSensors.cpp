#include "SkinSensors.h"
#include <iostream>

using namespace yarp::os;
using namespace std;


static bool compareIdOfSkinPart(SkinBodyPart* a, SkinBodyPart* b)
{
    return (a->getId() < b->getId());
}

bool SkinSensors::threadInit()
{
    cout << "\n\nInit Thread .........." << endl;

    for (size_t i = 0; i < partList.size(); i++)
    {
        if(!partList[i]->tryconnect())
	    {
            cout << "Skin Sensor is not connected:  " << i  << partList[i]->getPartName()<< endl;
            return false;
	    }
        cout << "OK connected. " << partList[i]->getPartName() << endl;
    }
    return true;
}

bool SkinSensors::initConfiguration()
{
    // skin sensors
    bool ifLogData = config.check("logdata");
    yarp::os::ConstString logpath = config.check("logfilepath", yarp::os::Value(""), "specify log file path for data logging if logdata flag is specified").asString();

    yarp::os::Bottle skinfiles = config.findGroup("sensorcfgs").tail();
    yarp::os::ConstString skinFilePath = config.check("skinsensorfilepath", yarp::os::Value("C:/roboskin/software_yarp_related/iCub/app/kaspar/conf"), "specifiy skinsensor config file path").asString();
    printf("%s   %d \n", skinFilePath.c_str(), skinfiles.size());
    for(int i = 0; i < skinfiles.size(); i++)
    {
        yarp::os::Property skinproperty;
        yarp::os::Bottle sensorConf(skinfiles.get(i).toString());
        if(skinproperty.fromConfigFile((skinFilePath + FILESEPARATOR + sensorConf.get(1).asString())))
        {
            // put new attribute ifLog to the property before initialising skin sensors
            if(ifLogData)
            {
                skinproperty.put("logdata", ifLogData);
                skinproperty.put("logfilepath", logpath.c_str());
            }

            SkinBodyPart *sk = new SkinBodyPart(skinproperty);   // assuming property file is correct. No error checking and return. so be very careful
            sk->setPortToConnect(sensorConf.get(2).asString());
            sk->setId(sensorConf.get(0).asInt());
            if(sk->initConfiguration())
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
            fprintf(stderr, "Error loading skin sensor config file %s\n", sensorConf.get(0).asString().c_str());
        }
    }

    // sort in the ascending order of the IDs
    std::sort(partList.begin(), partList.end(), compareIdOfSkinPart);

    unsigned int tempCountDim = 0;
	for (size_t k = 0; k < partList.size(); k++)
	{
		offsets.push_back(tempCountDim);
		tempCountDim += partList[k]->getAllNumTaxels();
	}

	// other variables
	//temporalData = new CircularBuffer<DataSample>(TIME_DIMENSION);  // 
	//this->tempDataSample = new DataSample(tempCountDim);

	tempTactileData = new TactileData(tempCountDim);
	
	// TactileData load svm model file
	// TODO:

	tState = new TactileState(TIME_DIMENSION, tempCountDim, 50);

	// initSensorGroups
	if(!initSensorGroups())
	{
		return false;
	}

	return true;
}

bool SkinSensors::initSensorGroups()
{
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

void SkinSensors::run()
{
	cout << "Start thread here....  " << partList.size() << endl;

	while(true)
	{
		for(size_t i = 0; i < partList.size(); i++)
		{
			yarp::sig::Vector* compensatedData = partList[i]->getCompensatedDataPort()->read();//
			//yarp::sig::Vector* compensatedData = (partList[i]->acquireData());
			
			if (compensatedData == NULL)
			  {
				cout << "Could not get data... Use old data instead. But there might be a problem with the sensors/YARP/CanUSB/etc" << endl;
				continue;
			  }
			else
			  {
				//cout << "OK ---" << compensatedData->size() << "----" << endl;
			  }

			unsigned int offset = offsets[i];
			//unsigned int size = partList[i]->getSensorNum();

			vector<int> sensorIds = partList[i]->getAvailableSensorIds();
			double *tempData = compensatedData->data();	

			for (int j = 0; j < partList[i]->getSize(); j++)
			{
				//cout << "j :   " << j << "   " << sensorIds[j] << "  " << sensorIds[j]*NUM_TAXEL_TRIANGLE << "  " <<  offset + j*NUM_TAXEL_TRIANGLE << endl;

				/*
				if(!tempDataSample->assignData(tempData + sensorIds[j]*NUM_TAXEL_TRIANGLE, offset + j*NUM_TAXEL_TRIANGLE, NUM_TAXEL_TRIANGLE))
				{
					cout << "CANNOT ASSIGN DATA INTO DATASAMPLE..........\n";
				}
				*/

				/*************NEW************/
				if(!tempTactileData->assignData(tempData + sensorIds[j]*NUM_TAXEL_TRIANGLE, offset + j*NUM_TAXEL_TRIANGLE, NUM_TAXEL_TRIANGLE))
				{
					cout << "CANNOT ASSIGN DATA INTO DATASAMPLE..........\n";
				}
				/*************END************/
			}
		}

		//temporalData->insert(tempDataSample);
		//tState->insertDataSample(*tempDataSample);
		//tState->updateSalienceMap(TactileState::timeDiff);
		
		tState->insertDataSample(*tempTactileData, true);
		if(!tState->updateSalienceMap(TactileState::timeDiff))
		{
			cerr << "ERROR: CANNOT UPDATE SALIENCE MAP\n";
		}
		//bool TactileState::updateClassification( methodForClassification &methodClassify)
		// 	int classifyPattern(DataSample &ds, methodForClassification &methodClassify);
/*
		if(!tState->updateClassification(TactileState::SVM_RBF))
		  {
		    cerr << "ERROR: Cannot perform classification\n";
		  }*/
	}
}

void SkinSensors::threadRelease()
{
	for (size_t i = 0; i < this->partList.size(); i++)
	{
		partList[i]->disconnect();
	}
}
