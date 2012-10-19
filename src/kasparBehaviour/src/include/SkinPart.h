#ifndef __SKIN_PART__
#define __SKIN_PART__
#include <stdio.h>
#include <string>
#include <stdlib.h>

#include <yarp/os/RateThread.h>
#include <yarp/os/Semaphore.h>
#include <yarp/dev/ControlBoardInterfaces.h>
#include <yarp/dev/PolyDriver.h>
#include <yarp/dev/CanBusInterface.h>
#include <yarp/sig/Vector.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_statistics.h>
#include <yarp/os/Bottle.h>
//#include "Triangle.h"
//#include "Fingertip.h"

#include <yarp/os/Time.h>
#include "signalprocess/threshold.h"

#include <time.h>
#include "utils/Resources.h"

const int CAN_DRIVER_BUFFER_SIZE=2047;

using namespace yarp::os;
using namespace yarp::dev;

#include "TouchSensor.h"
#include <vector>
using namespace std;

class SkinPart : public yarp::os::RateThread {
public:
	SkinPart(Searchable& config,int period=40) : RateThread(period),mutex(1){

		ifLog = false; 
		logFile = "";

		sensorsNum=0;

		for (int t=0; t<16; ++t)  {
			sensor[t]=NULL;
		}

		part="/skinGui/";
		robotPart = config.find("robotPart").asString();

		part.append(robotPart);
		part.append(":i");
		printf("SkinPart %s (%s)is created\n", part.c_str(), this->robotPart.c_str());
		skin_port.open(part.c_str());
		int width =config.find("width" ).asInt();
		int height=config.find("height").asInt();
		bool useCalibration = config.check("useCalibration");
		//useCalibration = true;
		if (useCalibration==true) {
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
				double xc=sensorConfig.get(2).asDouble();
				double yc=sensorConfig.get(3).asDouble();
				double th=sensorConfig.get(4).asDouble();
				double gain=sensorConfig.get(5).asDouble();
				int    lrMirror=sensorConfig.get(6).asInt();
				int    layoutNum=sensorConfig.get(7).asInt();

				//printf("%d %f\n",id,gain);

				if (id>=0 && id<16) {
					if (sensor[id]) {
						printf("WARNING: triangle %d already exists.\n",id);
					}
					else {
						/*
						if (type=="triangle") {
						sensor[id]=new Triangle(xc,yc,th,gain,layoutNum,lrMirror);
						sensor[id]->setCalibrationFlag(useCalibration);
						}
						else {
						sensor[id]=new Fingertip(xc,yc,th,gain,layoutNum,lrMirror);
						sensor[id]->setCalibrationFlag(useCalibration);
						}*/
						sensor[id] = new TouchSensor();
						//printf("%d %s\n", id, this->robotPart.c_str());
						sensor[id]->setCalibrationFlag(useCalibration);
						++sensorsNum;
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

		initIndexArray();

		// if log data to file
		ifLog = initLogFile(config);

		//data = gsl_vector_alloc( sensorsNum * 12);
		sensorsData = (double *)malloc(sizeof(double)*sensorsNum*12);
	}

	~SkinPart(){
		for (int t=0; t<16; ++t) {
			if (sensor[t]) delete sensor[t];
		}
		free(indexArray);
		//gsl_vector_free(data);
		free(sensorsData);
		//if(baselines)
		//free(baselines);
		if(ifLog && pLogFile != NULL)
		{
			fclose(pLogFile);
		}
	}


	virtual void run()
	{
		mutex.wait();
		//printf("Running skinpart %s\n", this->part.c_str());

		Bottle *input = skin_port.read();

		yarp::sig::Vector skin_value;
		double sensorData = 0;
		int sensorId =0;
		if(input != NULL)
		{
			skin_value.resize(input->size());
			for (int i=0; i<input->size(); i++)  {
				skin_value[i] = input->get(i).asDouble();
				sensorId=i/12;
				if (sensorId<16 && sensor[sensorId]) {
					sensorData = sensor[sensorId]->setActivationFromPortData(skin_value[i], i%12);

					if(this->ifLog)
					{
						// log data to file
						fprintf(pLogFile, "%4.1f ", sensorData);
					}
				}
			}
			if(this->ifLog)
			{
				// log data to file
				time_t rawtime;
				struct tm * timeinfo;
				time(&rawtime);
				timeinfo = localtime ( &rawtime );
				//std::string timeText = asctime(timeinfo);

				fprintf(pLogFile, " %f ", yarp::os::Time::now());
				fputs(asctime(timeinfo), pLogFile);   // asctime() will add "\n" automatically 
			}
		}

		mutex.post();
	}

	/*
	return: result of data processing. Using Bottle is to make it flexible to different methods. Although currently it will only contain mean, max, min, std, etc, and text description 
	e.g. threshold method: [typeOfTouch (text), mean(double), variance (double)]

	This must be very careful: it is not thread safe. 
	processData() should be only called once in one time. 
	*/
	ThresholdMethod::TouchEvent processData()
	{
		//copying vectors from each sensor to a matrix might not be efficient, but safer. need to see if there is a better and simple way to handle data

		mutex.wait();

		for(int k = 0; k < sensorsNum; k++)
		{
			// copy data from sensor[indexArray[k]] to sensorsData with offset of k
			sensor[indexArray[k]]->getData(sensorsData, k);
		}

		// release lock to allow thread to continue to get data
		mutex.post();

		//  static TouchEvent simpleThreshold(double *data, int len, double threshold)

		ThresholdMethod::TouchEvent te = ThresholdMethod::simpleThreshold(sensorsData, 12*sensorsNum, 43, 70, this->baselines);
		/* 
		for (int i = 0; i < 16; i ++) {
		if(sensor[i])
		{
		//printf("%s %d of %d\n", this->robotPart.c_str(), i, sensorsNum);
		sensor[i]->getData(sensorsData, i);
		//printf(" sensor[ %d ] %s of %d is %f\n", i, this->robotPart.c_str(),sensorsNum, sensorsData[0]);	
		}
		}
		*/      // TODO: process data here... temporary code below
		// mean value here

		//double mean, variance, largest, smallest;
		//mean = gsl_stats_mean(sensorsData, 1, 12*sensorsNum);
		//variance = gsl_stats_variance(sensorsData, 1, 12*sensorsNum);

		/*
		largest = gsl_stats_max(sensorsData, 1, 12*sensorsNum);
		smallest = gsl_stats_min(sensorsData, 1, 12*sensorsNum);

		if(largest > 20 && largest <= 50)
		{
		printf("max is %f %f\n", largest, smallest);
		b.addString((this->robotPart + "_soft").c_str());
		}
		else if(largest > 50)
		{
		printf("max is %f %f\n", largest, smallest);
		b.addString((this->robotPart + "_strong").c_str());
		}
		else
		{
		b.addString("None");
		}*/
		//b.addDouble(mean);
		//b.addDouble(variance);
		//b.addDouble(largest);
		//b.addDouble(smallest);
		return te;
		//return b;
	}

	virtual bool threadInit()
	{
		printf("SkinMeshThreadPort initialising...\n");
		printf("... done!\n");

		printf("Waiting for port connection\n");
		return true;
	}  

	virtual void threadRelease()
	{
		printf("SkinMeshThreadPort releasing...\n");	
		if(ifLog)
		{
			fprintf(pLogFile, "# thread released here #" );
			// log data to file
			time_t rawtime;
			struct tm * timeinfo;
			time ( &rawtime );
			timeinfo = localtime ( &rawtime );

			fprintf(pLogFile, " %f ", yarp::os::Time::now());
			fputs(asctime(timeinfo), pLogFile);   // asctime() will add "\n" automatically 
		}

		printf("... done.\n");
	}

	/*
	void resize(int width,int height)
	{
	mutex.wait();
	for (int t=0; t<16; ++t) {
	if (sensor[t]) sensor[t]->resize(width,height,40);
	}
	mutex.post();
	}*/

	/*
	void eval(double *image)
	{
	mutex.wait();

	for (int t=0; t<16; ++t) {
	if (sensor[t]) sensor[t]->eval(image);
	}

	mutex.post();
	}

	void draw(unsigned char *image)
	{
	for (int t=0; t<16; ++t) {
	if (sensor[t]) sensor[t]->draw(image);
	}        
	}
	*/

	void setPortToConnect(const char *toPort)
	{
		this->toPort = toPort;
	}

	bool connect()
	{
		if(toPort.length() == 0)
		{
			return false;
		}
		if(!yarp::os::Network::isConnected(toPort.c_str(), part.c_str()))
			return yarp::os::Network::connect(toPort.c_str(), part.c_str());
		return true;
	}

	bool disconnect()
	{
		if(toPort.length() == 0)
			return false;
		if(yarp::os::Network::isConnected(toPort.c_str(), part.c_str()))
			return yarp::os::Network::disconnect(toPort.c_str(), part.c_str());
		return true;
	}

	bool isConnected()
	{
		return yarp::os::Network::isConnected(toPort.c_str(), part.c_str());
	}

	string getPartName()
	{
		return robotPart;
	}

	void setThreadhold(double threshold_soft, double threshold_hard)
	{
		this->threshold_soft = threshold_soft;
		this->threshold_hard = threshold_hard;
	}

	bool initThresholds(const char *file)
	{
		printf("%s\n", file);
		Property config;
		bool t = config.fromConfigFile(file);
		if(!t)
		{
			printf("Cannot load file %s\n", file);
		}
		return this->initThresholds(config);
	}

	bool initThresholds(Searchable& config)
	{
		yarp::os::Bottle thres = config.findGroup("threshold");
		if(thres.isNull())
		{
			printf("No threshold property found in threshold file \n"); 
			return false;
		}
		printf("%s\n", thres.toString().c_str());
		//yarp::os::Bottle *thresholds = thres.asList();
		yarp::os::Bottle thresholds = thres.tail();
		printf("Size %d\n", thresholds.size());
		/*
		if(thresholds == NULL)
		{
		printf("No threshold property found in threshold file %s\n", this->robotPart.c_str()); 
		return false;
		}*/
		if(thresholds.size() != sensorsNum*12)
		{
			printf("Wrong format of threshold file. Please check. \n");
			return false;
		}
		baselines = (double *) malloc(sizeof(double)*sensorsNum * 12);
		for ( int i = 0; i < thresholds.size(); i++)
		{
			baselines[i] = thresholds.get(i).asDouble();
			//printf("%f\n", baselines[i]);
		}
		printf("OK. loaded %s\n", this->robotPart.c_str());
		return true;
	}

	int getSensorNum()
	{
		return this->sensorsNum;
	}

private:
	//  int numOfSensors;
	std::vector<TouchSensor> sensorList;
	std::string  part;
	std::string robotPart;
	BufferedPort<Bottle> skin_port;						
	TouchSensor *sensor[16];  
	yarp::os::Semaphore mutex;  
	int sensorsNum;
	int *indexArray;

	//gsl_vector *data; // store data of sensors of this part [sensor1; sensor2;...] each is a vector of 12 items
	double *sensorsData;
	std::string toPort; 
	// for threshold method
	double *baselines;   // thresholds for each sensor
	double threshold_soft; // unified threshold
	double threshold_hard;

	// log file
	bool ifLog;
	std::string logFile;
	FILE * pLogFile;
	int count;

	void initIndexArray()
	{
		// to keep index of available sensor ids
		indexArray = (int *)malloc(sizeof(int)*sensorsNum);
		int k = 0;
		// very basic, no error checking
		for(int i = 0; i < 16; i++)
		{
			if(sensor[i])
			{
				indexArray[k] = i;
				k++;
			}	
		} 
	}
	
	bool initLogFile(Searchable &config)
	{
		// if log data to file
		ifLog = config.check("logdata");
		yarp::os::Value nameValue = config.find("childName");
		yarp::os::Value logPathValue = config.find("logfilepath");
		std::string childName = "";
		string logpath = "";//config.find("logfilepath").asString().c_str();

		if(!nameValue.isNull())
		{
			childName = nameValue.asString().c_str();
			printf("Child name is %s\n", childName.c_str());
		}
		if(!logPathValue.isNull())
		{
			logpath = logPathValue.asString().c_str();
			printf("Log File Path is 5s\n", logpath.c_str());
		}
		if(ifLog)
		{
			time_t rawtime;
			struct tm * timeinfo;
			time ( &rawtime );
			timeinfo = localtime ( &rawtime );
			std::string timeText = to_string(timeinfo->tm_sec) + "_" + to_string(timeinfo->tm_min) + "_" + to_string(timeinfo->tm_hour) + "_" + to_string(timeinfo->tm_mday)+ "_" + to_string(timeinfo->tm_mon + 1)+ "_" + to_string(timeinfo->tm_year + 1900);//asctime(timeinfo);

			if(childName != "")
			{
				logFile = this->robotPart + "_" + childName + "_" + timeText;
			}
			else
			{
				logFile = this->robotPart + "_" + timeText;
			}

			if(logpath != "")
			{
				logFile = logpath + FILESEPARATOR + logFile;
			}

			printf("LOG DATA of part %s\n", logFile.c_str());
			pLogFile =  fopen (logFile.c_str() , "w");

			if (pLogFile == NULL) {
				fprintf(stderr, "Error opening file %s\n", logFile.c_str());
				fprintf(stderr, "Log File disabled\n");
				ifLog = false;
			}
		}
		else
		{
			printf("NOT LOG DATA\n");
		}

		return ifLog;
	}
};

#endif
