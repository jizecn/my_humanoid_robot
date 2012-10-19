/*
 * Copyright (C) 2011 Ze Ji
 * University of Hertfordshire. 
 * EU FP7 RoboSkin 
 * CopyPolicy: Released under the terms of the GPL v2.0, see GPL.TXT
 */

#include "FakeSkin.h"

#include <vector>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <signal.h>

using namespace yarp::dev;
using namespace yarp::os;
using namespace std;

void INThandler(int);
bool init(const char *filename);

std::vector<FakeSkin *> skins;
std::vector<std::string> ports;

int main(int argc, char *argv[]) {
	signal(SIGINT, INThandler);

	yarp::os::Network yarp;

	if (!init("fakeskinconfig.ini")) {
		cerr << "ERROR: something wrong with initialisation" << endl;
		exit(-1);
	}

	Time::delay(1000);

	for (int i = 0; i < skins.size(); i++) {
		skins[i]->stop();
		delete skins[i];
	}
	return 0;
}

bool init(const char *filename) {
	yarp::os::Property config;
	config.fromConfigFile(filename);

	FakeSkin::DataSource type;

	// skin sensors
	yarp::os::ConstString skinFilePath = config.check("skinsensorfilepath",
			yarp::os::Value("./"), "specifiy skin data file path").asString();
	yarp::os::ConstString dataPath =
			config.check("skindatapath", yarp::os::Value("datasamples"),
					"specifiy skin data file path").asString();
	yarp::os::Bottle skinfiles = config.findGroup("skinsensors").tail();
	printf("%s   %d \n", skinFilePath.c_str(), skinfiles.size());

	yarp::os::ConstString datasource = config.check("DataSource",
			yarp::os::Value("RandomData"), "RandomData or Recorded").asString();
	if (datasource == "RandomData") {
		type = FakeSkin::RandomData;
	} else if (datasource == "Recorded") {
		type = FakeSkin::Recorded;
	}

	for (int i = 0; i < skinfiles.size(); i++) {
		yarp::os::Bottle sensorConf(skinfiles.get(i).toString());
		string port = sensorConf.get(0).asString().c_str();
		string datafile = sensorConf.get(1).asString().c_str();
	
		// skinsensorfilepath cfgfiles
		// skindatapath datasamples
		
		datafile = string(dataPath.c_str()).append("/").append(datafile);
		cout << datafile << endl;
		string idfile = sensorConf.get(2).asString().c_str();
		idfile = string(skinFilePath.c_str()) + string("/") + idfile;
		ports.push_back(port);
		FakeSkin *fskin = new FakeSkin(port.c_str());
		if (!fskin->init()) {
			cout << "ERROR: Cannot start " << ports[i].c_str() << endl;
			INThandler(SIGINT);
		}

		if (type == FakeSkin::Recorded) {
			fskin->setDataIds(idfile);

			fskin->setDataSource(FakeSkin::Recorded);

			if (!fskin->loadData(datafile.c_str())) {
				cerr << "ERROR: loaddata error" << endl;
				return false;
			}
		}
		fskin->start();
		skins.push_back(fskin);
	}
	return true;
}

void INThandler(int sig) {
	printf("FakeSkin quits now\n");
	for (int i = 0; i < skins.size(); i++) {
		skins[i]->stop();
		delete skins[i];
	}
	exit(0);
}
