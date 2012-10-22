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

    yarp::os::ResourceFinder config;
    config.setVerbose(true);
    config.setDefaultConfigFile("sensorconf.ini");
    config.configure("KASPAR_ROOT",argc, argv);
    if(!tp->configure(config))  {
        cout << "Can not initialise the TactilePreprocessing module" << endl;
    }
    else {
        cout << "OK here .... " << endl;
        tp->runModule();
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
