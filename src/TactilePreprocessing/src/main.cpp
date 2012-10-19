// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-
#include <yarp/os/Network.h>
#include <yarp/os/Port.h>
#include <yarp/os/Bottle.h>
#include <yarp/os/Time.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "svm.h"
#include <signal.h>
#include "circularBuffer.h"
#include "tactilePreprocessing.h"
//#include "SkinSensors.h"

//#define __TEST__

//using namespace yarp::os;
//using namespace yarp::dev;
using namespace std;

//void INThandler(int);

TactilePreprocessing *tp = new TactilePreprocessing();	

int main(int argc, char *argv[])
{
	//signal(SIGINT, INThandler);
	yarp::os::Network yarp;
	/*
	CircularBuffer<double> db(100);
	vector<double> vd(100);
	
	double t1 = yarp::os::Time::now();
	for (int i = 0; i < 200; i++)
	{
		double d = 10;
		db.insert(d);
	}
	double t2 = yarp::os::Time::now();

	cout << t2 - t1 << endl;
	db.getData(vd);
	//cout << vd.size();
	for(int i = 0; i < vd.size(); i++)
	{
		//cout << vd[i] << endl;
	}
	double t3 = yarp::os::Time::now();
	cout << t3 - t2 << endl;
	*/

	//SkinSensors sens();


	/***** test part... will be deleted finally. *****/ 

	//DataSample *ds = new DataSample(100);
	//delete ds;


	/**************END OF TEST**************/


    /*
	string confFile = "sensorconf.ini";
	if(argc > 1)
	{
		confFile = argv[1];
	}

	cout << confFile << endl;
    */
    yarp::os::ResourceFinder config;
    config.setVerbose(true);
    config.setDefaultConfigFile("sensorconf.ini");
    config.configure("KASPAR_ROOT",argc, argv);
    if(!tp->configure(config))
        {
			cout << "Can not initialise the TactilePreprocessing module" << endl;
        }
    else
		{
			cout << "OK here .... " << endl;
			tp->runModule();
			/*
			while(true)
			{
				yarp::os::Time::delay(100);
				cout << "Still running\n";
			}
			*/
		}
    
	return 0;
}

/*
void INThandler(int sig) {
	printf(" quit now\n");
	delete tp;
	exit(0);
}
*/