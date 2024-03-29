#include "ServoPosition.h"

ServoPosition::ServoPosition(bool ifDelay)
{
	this->ifDelay = ifDelay;
	isServoAssigned = false;
	this->compliant = false;
	this->servo = NULL;
}

ServoPosition::~ServoPosition()
{
    
}

bool ServoPosition::delay()
{
    return ifDelay;
}

std::string ServoPosition::getPartName()
{
    return servo->getName();
}

int ServoPosition::getPosition()
{
    return pos;
}

int ServoPosition::getSpeed()
{
    return speed;
}

void ServoPosition::setIfDelay(bool ifDelay)
{
    this->ifDelay = ifDelay;
}

/*
* Position is scaled from 0 - 100, 0 -> min position 100 -> max position, which are defined in dev file
*/
void ServoPosition::setPosition(int position)
{
	// position is scaled
    this->pos = position;
	//this->unscalePosition();
}

/*
* Speed is scaled from 0 - 100, 0 -> 0 100 -> 1023 position, which are defined in dev file
*/
void ServoPosition::setSpeed(int speed)
{
    this->speed = speed;
	//this->unscaleSpeed();
}

void ServoPosition::getServo(Servo &servo)
{
	servo = *(this->servo);
}

void ServoPosition::setPartName(string partName)
{
	this->partName = partName;
}

bool ServoPosition::assignServoFrom(Servos *servos)
{/*
	if(!servos->getServo(this->partName, this->servo))
	{
		return false;
	}
*/
	servo = servos->getServo(this->partName);
	isServoAssigned = true;
	return true;
}

bool ServoPosition::servoAssigned()
{
	// return this->servo != NULL;
	return isServoAssigned;
}

bool ServoPosition::unscalePosition()
{
	if(this->servo == NULL) 
	{
		printf("Servo instance is not set.\n");
		return false;
	}
	int l = this->servo->getMaxPos() - this->servo->getMinPos();
	//ACE_OS::printf("max %d - min %d: %d\n",this->servo.getMaxPos(), this->servo.getMinPos(), l);
    this->unscaledPos = (double)l * (double)this->pos / 1000.0 + (double)this->servo->getMinPos();	
	//ACE_OS::printf("unscaledPos is : %f\n", this->unscaledPos);
	return true;
}

bool ServoPosition::unscaleSpeed()
{
	if(this->servo == NULL)
	{
		printf("Servo instance is not set.\n");
		return false;
	}
	int l = this->servo->getMaxSpeed() - this->servo->getMinSpeed();
	this->unscaledSpeed = (double)l * (double)this->speed / 1000.0 + (double)this->servo->getMinSpeed();
	//ACE_OS::printf("unscaledSpeed is : %f\n", this->unscaledSpeed);
	return true;
}

bool ServoPosition::getUnscaledPosition(double &unscaledPosition)
{
	if(!this->unscalePosition())
	{
		return false;
	}
	unscaledPosition = this->unscaledPos;
	return true;
}

bool ServoPosition::getUnscaledSpeed(double &unscaledSpeed)
{	
	if(!this->unscaleSpeed())
	{
		return false;
	}
	unscaledSpeed = this->unscaledSpeed;
	return true;
}

void ServoPosition::setCompliant(bool c)
{
	this->compliant = c;
}

bool ServoPosition::isCompliant()
{
	return this->compliant;
}