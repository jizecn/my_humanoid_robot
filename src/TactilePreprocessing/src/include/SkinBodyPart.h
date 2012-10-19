#ifndef __SKIN_BODY_PART_H__
#define __SKIN_BODY_PART_H__

#include <string>

#include <yarp/os/Semaphore.h>
#include <yarp/sig/Vector.h>
#include <yarp/os/Bottle.h>
#include <yarp/os/BufferedPort.h>
#include <time.h>
#include "utils/Resources.h"
#include "TouchSensor.h"
#include <vector>
#include <algorithm>
#include <iostream>

using namespace yarp::os;
//using namespace yarp::dev;
using namespace yarp::sig;

using namespace std;

class SkinBodyPart
{
public:
	SkinBodyPart(yarp::os::Property & config)
	{
		this->config = config;
		sensorsNum = 0;
		this->numTaxels = 0;
		
		for (int t=0; t<16; ++t)  {
			sensor[t]=NULL;
		}
	}

	~SkinBodyPart()
	{

	}

	bool initConfiguration()
	{
		// open and connect the compensated tactile data port
		robotPart = config.find("robotPart").asString();
		compensatedTactileDataPortName = "/" + robotPart;
		compensatedTactileDataPortName += config.check("compensatedTactileDataPort", yarp::os::Value("/compTactData:i")).asString().c_str();
		if (!compensatedTactileDataPort.open(compensatedTactileDataPortName.c_str())) {
			fprintf(stderr, "unable to open the compensated tactile data input port ");
			return false;  // unable to open; let RFModule know so that it won't run
		}  

		printf("SkinPart %s (%s)is created\n", compensatedTactileDataPortName.c_str(), this->robotPart.c_str());

		bool useCalibration = config.check("useCalibration");
		//useCalibration = true;

		if (useCalibration) {
			printf("Using calibrated skin values (0-255)\n");
		}
		else {
			printf("Using raw skin values (255-0)\n");
		}

		yarp::os::Bottle sensorSetConfig=config.findGroup("SENSORS").tail();

		for (int t=0; t<sensorSetConfig.size(); ++t) {       
			yarp::os::Bottle sensorConfig(sensorSetConfig.get(t).toString());

			std::string type(sensorConfig.get(0).asString());
			if (type=="triangle" || type=="fingertip") {
				int id=sensorConfig.get(1).asInt();
				double gain=sensorConfig.get(5).asDouble();

				if (id>=0 && id<16) {
					if (sensor[id]) {
						printf("WARNING: triangle %d already exists.\n",id);
					}
					else {
						// TO CHECK.... 
						sensor[id] = new TouchSensor(id, gain);

						sensor[id]->setCalibrationFlag(useCalibration);

						this->ids.push_back(id);

						++sensorsNum;
						
						// initialise number of taxels for all sensor triangles
						this->numTaxels += sensor[id]->getNumTaxels(); // should be always 12
					}
				}
				else {
					printf("WARNING: %d is invalid triangle Id [0:15].\n",id);
				}
			}
			else {
				printf("WARNING: sensor type %s unknown, discarded.\n",type.c_str());
			}
		}

		// important to alligh the data dimensions
		std::sort(this->ids.begin(), this->ids.end()); //Sort the Vector 	

		return true;
	}

	string getPartName()
	{
		return robotPart;
	}

	void setPortToConnect(const char *toPort)
	{
		this->toPort = toPort;
	}

	/*Sensor id is different from the ID used in the hardware setting, e.g. CAN ID etc. It is also not the ID for triangles. This is used to allocate space for storing data. This id is specified in the config files*/
	void setId(int id)
	{
		this->id = id;
	}

	int getId()
	{
	  return this->id;
	}
/*
	int getSensorNum()
	{
		cout << this->getSize() << ";====;;" << endl;
		return this->sensorsNum;
	}
*/
	/*TODO: Something wrong here.... */
	yarp::sig::Vector* acquireData()
	{
		cout << getSize() << "+++++++++++" << endl;
		yarp::sig::Vector *v = (compensatedTactileDataPort.read());
		if(v == NULL)
		{
			cout << "NULL POINTER\n";
		}
		cout << getSize() << "+++++++++++" << endl;

	//// WHY DOES THE SIZE CHANGE TO 0 HERE???????????????????????????
		/// Looks OK on MAC OS. SO it is a problem for VS 2008 only? 
		return v;
	}

	yarp::os::BufferedPort<Vector> *getCompensatedDataPort()
	{
		yarp::os::BufferedPort<Vector> *t = &(this->compensatedTactileDataPort);
		return t;
	}

	bool tryconnect()
	{
		if(toPort.length() == 0)
		{
			cout << "Port name is invalid: 0 char\n";
			return false;
		}
		if(!yarp::os::Network::isConnected(toPort.c_str(),  compensatedTactileDataPortName.c_str()))
		{
			if( yarp::os::Network::connect(toPort.c_str(), compensatedTactileDataPortName.c_str()))
			{
				return true;
			}
			else 
			{
				return false;
			}
		}
		return true;
	}

	bool disconnect()
	{
		if(toPort.length() == 0)
			return false;
		if(yarp::os::Network::isConnected(toPort.c_str(), compensatedTactileDataPortName.c_str()))
			return yarp::os::Network::disconnect(toPort.c_str(), compensatedTactileDataPortName.c_str());
		return true;
	}

	bool isConnected()
	{
		return yarp::os::Network::isConnected(toPort.c_str(), compensatedTactileDataPortName.c_str());
	}

	int getAllNumTaxels()
	{
	  return this->numTaxels;
	}

	int getSize()
	{
		return ids.size();
	}

	vector<int> getAvailableSensorIds()
	{
		return this->ids;
	}

private:
	int id;  
	std::string  part;
	std::string robotPart;

	TouchSensor *sensor[16];
	yarp::os::Semaphore mutex;  
	int sensorsNum;
	//int *indexArray;

	//double *sensorsData;
	std::string toPort; 

	yarp::os::BufferedPort<yarp::sig::Vector> compensatedTactileDataPort;	// input port for compensated tactile data

	yarp::os::BufferedPort<yarp::os::Bottle> testPort; 

	vector<int> ids;
	yarp::os::Property config;
	int numTaxels;   // number of taxels... by default, if every sensor has 12 taxels, then numTaxels is 12*sensorsNum
	string compensatedTactileDataPortName;
};

#endif //__SKIN_BODY_PART_H__
