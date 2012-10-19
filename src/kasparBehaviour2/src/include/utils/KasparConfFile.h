/*
 * KasparConfFile.h
 *
 *  Created on: 8 Jul 2010
 *      Author: ze
 */

#ifndef KASPARCONFFILE_H_
#define KASPARCONFFILE_H_

#include <ace/OS_NS_stdio.h>
#include <ace/Vector_T.h>
#include <ace/SString.h>
#include <string>
#include "NumberFormatException.h"
#include "FileFormatException.h"
#include "FileIOException.h"

#include "KasparPose.h"
#include "KasparSequence.h"
#include "KasparSequenceStep.h"
#include "ServoPosition.h"
#include "Servos.h"
#include "Servo.h"
#include <iostream>
using namespace std;

//http://en.wikipedia.org/wiki/C_preprocessor
#if defined __linux__
    #define FILESEPARATOR "/"
#elif defined __linux
    #define FILESEPARATOR "/"
#elif defined __WIN32__
    // Windows stuff
    #define FILESEPARATOR "\\"
#else
    // other stuff
    #define FILESEPARATOR "/"
#endif

/*
struct devProperty {
    //;<NAME>,<SERVO_TYPE>,<minPos>,<maxPos>,<defaultPos>,<minSpeed>,<maxSpeed>,<defaultSpeed>
    string name;
    string type; // AX12 or SSC
    int id;
    int minPos;
    int maxPos;
    int defaultPos;
    int minSpeed;
    int maxSpeed;
    int defaultSpeed;
};*/
/*
struct devPoseProperty {
    // pose file example
    // HEAD_ROT,55,14,false  -- partName, position, speed, ifDelay
    string partName; // e.g. ARM_L...
    int pos;   // not understand . need to confirm with Sven
    int speed;
    bool ifDelay;
};

struct poseFileProperty {
    ACE_Vector<devPoseProperty> poseList;
    string poseName;
    string devFileName;
    int numOfPoses; // length of poseList;
};

struct seqStepProperty {
    string poseName;
    string poseFileName;
    int delay;
};

struct seqFileProperty {
    string seqName;
    int steps;
    ACE_Vector<seqStepProperty> seqList;
};

struct keyMap {
    string key;
    string poseName;
};
*/
class KasparConfFile {
public:
	static const enum ErrorCode {OK, FileFormatError, FileIOError, NumberFormatError};
    //KasparConfFile();
    //~KasparConfFile();
/*
    static void readPoseFile(const char *filepath, const char *filename, poseFileProperty *pfp) throw(FileIOException, NumberFormatException, FileFormatException);
    static void readDevFile(const char *filepath, const char *filename, ACE_Vector<devProperty> *devList) throw(FileIOException, NumberFormatException, FileFormatException);
    static void readSeqFile(const char *filepath, const char *filename, seqFileProperty *sfp) throw(FileIOException, NumberFormatException, FileFormatException);
    static void readKMFile(const char *filepath, const char *filename, ACE_Vector<keyMap> *kmList) throw(FileIOException, FileFormatException);
    static int printDevProperty(devProperty *dp);  //just to print out details of the servo property
    static int printSeqProperty(seqStepProperty *sp);
    static int printKMProperty(keyMap *km);
*/
    static KasparPose loadPoseFile(const char *filepath, const char *filename) throw(FileIOException, NumberFormatException, FileFormatException);
    static Servos loadServoFile(const char *filepath, const char *filename) throw(FileIOException, NumberFormatException, FileFormatException);

	static ErrorCode loadPoseFile(const char *filepath, const char *filename, KasparPose &kasPose);
    static ErrorCode loadServoFile(const char *filepath, const char *filename, Servos &servos);
	//static ErrorCode loadSeqFile(const char *filepath, const char *filename, KasparSequence &seq);
    
	/**
     * Trim string on both sides, ignoring space, \t, \n ...
     */
    static void trimString(string &str, const string &trimDel = " \t\n");

private:
    static const char commentSymbol = ';';
    static const char del = ',';

    static int stripNewline(char *str);    // STUPID implementation. But currently just keep it.
    static int convertToInt(char* str);
};

#endif /* KASPARCONFFILE_H_ */
