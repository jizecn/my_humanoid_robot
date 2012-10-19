/* 
 * File:   ServoPosition.h
 * Author: ze
 *
 * Created on 29 June 2010, 18:04
 */

#ifndef _SERVOPOSITION_H
#define	_SERVOPOSITION_H

#include "Servo.h"
#include "Servos.h"
//#include <ace/Vector_T.h>
#include <string>
//using namespace std;

class ServoPosition
{
public:
	//ServoPosition();
	ServoPosition(bool ifDelay = false);
    ~ServoPosition();

    std::string getPartName();
    int getPosition();
	bool getUnscaledPosition(double &unscaledPosition);

    int getSpeed();
    bool getUnscaledSpeed(double &unscaledSpeed);

	bool delay();
    void setSpeed(int speed);
    void setPosition(int position);
    void setPartName(std::string partName);
    void setIfDelay(bool ifDelay);
    void getServo(Servo &servo);
    bool assignServoFrom(Servos *servos);
    bool servoAssigned();
    
	void setCompliant(bool c);
	bool isCompliant();

private:
    Servo *servo;
    int pos;
	double unscaledPos;
    
	int speed;
	double unscaledSpeed;

    bool ifDelay;
    std::string partName;
    bool isServoAssigned;

	bool compliant; // used to determine if this current position should be compliant/passive. In other words, this position should be diabled. This is needed when some parts are touched by users. 

private: 
	bool unscalePosition();
	bool unscaleSpeed();
};

#endif	/* _SERVOPOSITION_H */

