/*
 * Servo.h
 *
 *  Created on: 30 Jun 2010
 *      Author: ze
 */

#ifndef SERVO_H_
#define SERVO_H_

class ServoGroup;

#include <iostream>
#include <string>
#include <vector>
#include "ServoGroup.h"

using namespace std;
class Servo
{
public:
	enum Type {AX12, SSC32, MINISSC, POLOLU};

	Servo()
	{
		this->name = "";
		this->type = AX12;
	
        this->hardwareId = 0;
		this->maxPos = 1023;
		this->minPos = 0;
		this->defaultPos = 500;
		this->minSpeed = 1;
		this->maxSpeed = 1023;
		this->defaultSpeed = 1;
        this->yarpId = 0;
	}

	Servo(string name, Type type, int hardwareId, int yarpId, double maxPos, double minPos, double defaultPos, double maxSpeed, double minSpeed, double defaultSpeed)
	{
		this->name = name;
		this->type = type;
        this->hardwareId = hardwareId;
		this->maxPos = maxPos;
		this->minPos = minPos;
		this->defaultPos = defaultPos;
		this->minSpeed = minSpeed;
		this->maxSpeed = maxSpeed;
		this->defaultSpeed = defaultSpeed;
		
        this->yarpId = yarpId;
		this->group = NULL;
	}

	~Servo()
	{

	}

    string toString()
    {
        string ret = "";
        if(type == this->AX12)
            ret = "AX12";
        else if(type == this->MINISSC)
            ret = "MINISSC";
        else if(type == this->SSC32)
            ret = "SSC32";
        else if(type == this->POLOLU)
            ret = "POLOLU";

        std::cout << ret << " " << name << " " << hardwareId << " " << yarpId << " " << minPos << " " << maxPos << " " << minSpeed << " " << maxSpeed << " " << defaultPos << " " << defaultSpeed << " \n";        
        ret = ret + " " + name + "\n ";// + hardwareId + " " + yarpId + " " + minPos + " " + maxPos + " " + minSpeed + " " + maxSpeed + " " + defaultPos + " " + defaultSpeed + " \n";
        return ret;
    }

	double getDefaultPos() const
    {
        return defaultPos;
    }

    double getDefaultSpeed() const
    {
        return defaultSpeed;
    }

	/*
	* NEVER CALL THIS Method for Yarp driver. Use getIdIndex instead. See explanation of getIdIndex(). Id is hardware coded Id in motor. 
	*/
    int getHardwareId() const
    {
        return this->hardwareId;
    }

	/*
	* Each motor has its hardware coded Id, which is id. 
	* In yarp driver, it is using index from [0, N - 1], N is the number of servos. Each index has its id. 
	* To use yarp, this method should be used, instead of getId(). 
	*/
	int getYarpId() const 
	{
        return this->yarpId;
	}

	void setYarpId(int yarpId)
	{
        this->yarpId = yarpId;
	}

    double getMaxPos() const
    {
        return maxPos;
    }

    double getMaxSpeed() const
    {
        return maxSpeed;
    }

    double getMinPos() const
    {
        return minPos;
    }

    double getMinSpeed() const
    {
        return minSpeed;
    }

    string getName() const
    {
        return name;
    }

    Type getType() const
    {
        return type;
    }

    void setDefaultPos(double defaultPos)
    {
        this->defaultPos = defaultPos;
    }

    void setDefaultSpeed(double defaultSpeed)
    {
        this->defaultSpeed = defaultSpeed;
    }

    void setHardwareId(int id)
    {
        this->hardwareId = id;
    }

    void setMaxPos(double maxPos)
    {
        this->maxPos = maxPos;
    }

    void setMaxSpeed(double maxSpeed)
    {
        this->maxSpeed = maxSpeed;
    }

    void setMinPos(double minPos)
    {
        this->minPos = minPos;
    }

    void setMinSpeed(double minSpeed)
    {
        this->minSpeed = minSpeed;
    }

    void setName(string name)
    {
        this->name = name;
    }

    void setType(Type type)
    {
        this->type = type;
    }

	void setGroup(ServoGroup *sg)
	{
		this->group = sg;
	}

    ServoGroup *getServoGroup()
    {

        return this->group;
    }

private:
	string name;
	int hardwareId;
	//int address;
	Type type;

	double maxPos;
	double minPos;
	double defaultPos;
	double defaultSpeed;
	double maxSpeed;
	double minSpeed;
	int yarpId;
    // new variables. metric
	ServoGroup *group;

    int torque;
};

#endif /* SERVO_H_ */
