#include <ace/OS_NS_stdio.h>
#include <ace/Vector_T.h>
#include <yarp/os/Network.h>
#include <yarp/os/Port.h>
#include <yarp/os/Bottle.h>
#include <yarp/os/Time.h>
#include <stdio.h>
#include <stdlib.h>
#include <yarp/dev/PolyDriver.h>
#include <yarp/dev/ControlBoardInterfaces.h>
#include "SkinPart.h"
#include  <signal.h>
#include <vector>
#include <string>
#include <time.h> 
#include "MainThread.h"
using namespace std;
#define FILESEPARATOR "/"

using namespace yarp::os;
using namespace yarp::dev;

void     INThandler(int);

void writeToFile();

FILE * pFile;
char buffer [100];

double *data;
int datasize = 0;
vector<SkinPart *> skinParts;
int main(int argc, char **argv) {
	Network yarp; 
	signal(SIGINT, INThandler);

	Property config;

	std::string configFile; 
	std::string dataFile;
	if(argc == 1)
	{
		printf("datalogger configFile datafile. Or datalogger datafile\n");
		return -1;
	}
	else if(argc == 2)
	{
		configFile = "/home/zeji/roboskin/coding/uhroboskin/roboskin/src/kasparBehaviour/config_linux.ini";
		dataFile = argv[1];
	}
	else if(argc == 3)
	{
		configFile = argv[1]; //"/home/zeji/roboskin/coding/uhroboskin/roboskin/src/kasparBehaviour/config_linux.ini"
		dataFile = argv[2];
	}
	
	config.fromConfigFile(configFile.c_str());
	
	pFile = fopen (dataFile.c_str() , "w");
	
	if (pFile == NULL) {
		perror ("Error opening file");
		return -1;
	}

	//MainThread mainThread(config, 50);
	//mainThread.start();
	// skin sensors
	yarp::os::Bottle skinfiles = config.findGroup("sensorfiles").tail();
	yarp::os::ConstString skinFilePath = config.check("skinsensorfilepath", yarp::os::Value("C:/roboskin/software_yarp_related/iCub/app/kaspar"), "specifiy skinsensor config file path").asString();
	printf("%s   %d \n", skinFilePath.c_str(), skinfiles.size());
	for(int i = 0; i < skinfiles.size(); i++)
	{
		yarp::os::Property skinproperty;
		yarp::os::Bottle sensorConf(skinfiles.get(i).toString());
		if(skinproperty.fromConfigFile((skinFilePath + FILESEPARATOR + sensorConf.get(0).asString())))
		{
			SkinPart *sk = new SkinPart(skinproperty);   // assuming property file is correct. No error checking and return. so be very careful
			sk->setPortToConnect(sensorConf.get(1).asString());
			skinParts.push_back(sk);
		}
		else
		{
			fprintf(stderr, "Error loading skin sensor config file %s\n", sensorConf.get(0).asString().c_str());
			return -1;
		}
	}

	for(int i = 0; i < skinParts.size();i++)
	{
		if(!skinParts[i]->connect())
		{
			printf("%s connection failed\n", skinParts[i]->getPartName().c_str());	
			return -1;
		}

		printf("%s connected\n", skinParts[i]->getPartName().c_str());
		skinParts[i]->start();  
	}


	yarp::os::Time::delay(1);

	for(int i = 0; i < skinParts.size(); i++)
	{
		datasize += skinParts[i]->getSensorsNum();
	}
	data = (double *)malloc(sizeof(double)*datasize*12);

	for(int i = 0; i < datasize*12; i ++)
	{
		data[i] = 0;
	}

	int counter = 0;
	time_t rawtime;
	struct tm * timeinfo;
	double temptime = 0; //yarp::os::Time::now();


	for(int i = 0; i < skinParts.size(); i++)
	{
        printf(skinParts[i]->infoToString().c_str());
        printf("\n");
    }

	while (true)
	{
		for(int i = 0; i < skinParts.size(); i++)
		{
			skinParts[i]->writeToFile(pFile);
		}
		if(counter == 0)
		{   
			time ( &rawtime );
			timeinfo = localtime ( &rawtime );

			fprintf(pFile, " %f ", yarp::os::Time::now() - temptime);
			temptime = yarp::os::Time::now();
			fputs(asctime(timeinfo), pFile);
		}
		else
		{
			fprintf(pFile, "\n");
		}
		counter ++;
		if(counter == 50) {

			counter = 0;
		}
		yarp::os::Time::delay(0.05);
	}

	return 0;
}

void writeToFile()
{
	for(int i = 0; i < datasize*12; i++)
	{
		fprintf(pFile, "%d ", data[i]); 
	}
	fprintf(pFile, "\n");
}

void  INThandler(int sig)
{
	printf("INT handler\n");
	char  c;

	signal(sig, SIG_IGN);
	printf("OUCH, did you hit Ctrl-C?\n"
		"Do you really want to quit? [y/n] ");
	c = getchar();
	if (c == 'y' || c == 'Y')
	{
		for(int i = 0; i < skinParts.size(); i++)
		{
			skinParts[i]->stop();
			skinParts[i]->disconnect();
		}
		fclose(pFile);
		if(data)	
			free(data);
		exit(0);
	}
	else
		signal(SIGINT, INThandler);


}
