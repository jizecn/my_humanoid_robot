/*
 * KasparPose.h
 *
 *  Created on: 30 Jun 2010
 *      Author: ze
 */

#ifndef KASPARPOSE_H_
#define KASPARPOSE_H_

//#ifdef HAVE_TOKENIZER_T_H
//#include <ace/Tokenizer_T.h>
//#endif 
#include <vector>
#include "ServoPosition.h"
#include <stdio.h>
#include "utils/Resources.h"
//#include "utils/NumberFormatException.h"
#include "utils/Utils.h"
#include <string>
#include <string.h>
#include "utils/Tokenizer.h"
//using namespace std;

class KasparPose
{
public:
	KasparPose();
	KasparPose(std::string name);  //deprecated. to be removed
	~KasparPose();

	int getNumOfServoPositions();
	void addServoPosition(ServoPosition sPos);
	void removeAllPoses();
	bool getServoPositionAt(int i, ServoPosition &sp);
	std::string getName();
	std::string getFileName();
	void setName(std::string name);
	void setFileName(std::string filename);

    KasparFile::RetCode initFromFile(const char *filepath, const char *filename);

	/*
	 * (similar to the function in KasparPose)
	 * provide the pointer to all servos available, so that each servoposition object can find its own servo object. 
	 * this should be run before initFromFile()
	 * Every servo should be used as a pointer, so that a global change to the property of any servo would take into effect immediately
	*/
	void setServos(Servos *s);


	/**
	* @return index of the found pose by the given name. -1 if not found
	*/
	static int getPoseFrom(std::string name, std::vector<KasparPose> poses);

private:
	//static const int NUM_SERVO_POSITIONS = 16;

	//ACE_Vector<ServoPosition, NUM_SERVO_POSITIONS> positionList;
	std::vector<ServoPosition> positionList;
	std::string name;
	std::string filename;
	Servos *servos;

};

#endif /* KASPARPOSE_H_ */
