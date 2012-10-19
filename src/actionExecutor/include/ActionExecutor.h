/*
 * Copyright (C) 2011 Ze Ji
 * University of Hertfordshire. 
 * EU FP7 RoboSkin 
 * CopyPolicy: Released under the terms of the GPL v2.0, see GPL.TXT
 */

#ifndef __ACTION_EXECUTOR_H__
#define __ACTION_EXECUTOR_H__

#include <yarp/os/all.h>
#include <yarp/sig/all.h>
#include <yarp/os/RFModule.h>
#include <yarp/os/Thread.h>
#include <yarp/os/Time.h>
#include <yarp/os/Module.h>
#include <yarp/os/Stamp.h>
#include <iomanip>
#include <yarp/os/ResourceFinder.h>
#include <iostream>
#include <string>
#include <vector> 
#include "KasparPose.h"
#include "Servo.h"
#include "KasparSequence.h"
#include "ServoGroup.h"
#include "ServoPosition.h"
#include "Servos.h"
#include "KasparSequenceStep.h"
#include "MotorExecution.h"
#include "KasparSequences.h"
#include <yarp/dev/ControlBoardInterfaces.h>
#include "MotorExecution.h"
#include "SoundPlay.h"
#include <time.h>

#include <algorithm>

#ifdef __linux__
#include <dirent.h>
#endif

#ifdef _WIN32 || _WIN64
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#endif

class ActionExecutor : public yarp::os::RFModule
{
public:
    ActionExecutor();
    ~ActionExecutor();

    bool configure(yarp::os::ResourceFinder& config);
    // try to interrupt any communications or resource usage
    bool interruptModule() {
		port.close();
		cmdPort.close();
        return true;
    }
    int initYarpDevice(yarp::os::ResourceFinder &config);
    bool updateModule();

    bool respond (const yarp::os::Bottle &command, yarp::os::Bottle &reply)
    {
        return true;
    }

	void logSequence(const char *content);
	void playSound();

    void servosCompliant(ServoGroup *sg, int torque, bool stop = true);

		
	double getPeriod()
	{
	return 0.05;
	}
	

private:
    // Make a port for reading and writing images
    yarp::os::BufferedPort<yarp::os::Bottle> port;
    yarp::os::Port cmdPort;
    yarp::os::Stamp stamp; 
    yarp::dev::PolyDriver pd; 
    yarp::dev::IPositionControl *pos;
    yarp::dev::IEncoders *enc;
    yarp::dev::ITorqueControl *torq;
    Servos servos; 
    //std::vector<KasparSequence> kSeqs;
    
    bool prepare(string command);
   
	void initLogFile(yarp::os::ResourceFinder &config);
    int getdir (string dir, vector<string> &files)
    {
#ifdef __linux__
        DIR *dp;
        struct dirent *dirp;
        if((dp  = opendir(dir.c_str())) == NULL) {
        	std::cout << "Error(" << errno << ") opening " << dir << std::endl;
            return errno;
        }

        while ((dirp = readdir(dp)) != NULL) {
            files.push_back(std::string(dirp->d_name));
        }
        closedir(dp);
#endif
		
#ifdef _WIN32 || _WIN64
		std::cout << "WINDOWS SYSTEM..............\n";
		std::cout << "Search ...  " << dir.c_str() << std::endl;
		
		WIN32_FIND_DATA FindFileData;
   HANDLE hFind;

   hFind = FindFirstFile((dir + "\\*").c_str(), &FindFileData);
   if (hFind == INVALID_HANDLE_VALUE) 
   {
      printf ("FindFirstFile failed (%d)\n", GetLastError());
      return -1;
   } 
   else 
   {
      _tprintf (TEXT("The first file found is %s\n"), 
                FindFileData.cFileName);
		files.push_back(FindFileData.cFileName);	      
   }
   	while(FindNextFile(hFind, &FindFileData) != 0)
	{
		files.push_back(FindFileData.cFileName);	      
	}

	FindClose(hFind);
#endif
		return 0;
    }

    std::string sequenceLogFile; 
    FILE *pSequenceLogFile;
    SoundPlay sp;
    std::string soundFilePath;
    bool executeCommand(yarp::os::Property &contentProp);
    MotorExecution *motorControl; 
    bool ifLogSequence;
    KasparSequence currentSeq;
    KasparSequenceStep currentSeqStep;
    KasparSequences sequences;
};

#endif //_ACTION_EXECUTOR_H__
