#include "ServoGroup.h"
#include <stdio.h>
ServoGroup::ServoGroup()
{
    this->compliantMode = false;
}

ServoGroup::~ServoGroup()
{
  
}
/*
void ServoGroup::addServo(string name)
{
	this->members.push_back(name);
}*/

void ServoGroup::addServo(Servo *sv)
{
    if(sv != NULL)
    {
        this->servoMembers.push_back(sv);
        //sv->setGroup(this);
    }
    else
    {
        printf("servo is NULL, not added\n");
    }
}
/*
vector<string> *ServoGroup::getServos()
{
  return &this->members;
}*/

/*
string ServoGroup::getServo(int i)
{
  if(i < this->members.size() && i >= 0)
  {
      return this->members[i];
  }
  else
  {
      cout << "Invalid index number. Please check!" << endl;
      return NULL;
  }
}*/

Servo *ServoGroup::getServo(int i)
{
  if(i < this->servoMembers.size() && i >= 0)
  {
      return this->servoMembers[i];
  }
  else
  {
      cout << "Invalid index number. Please check!" << endl;
      return NULL;
  }
}

int ServoGroup::numOfServos()
{
    return this->servoMembers.size();
}

bool ServoGroup::inGroup(string name)
{
    for(int i = 0; i < this->servoMembers.size(); i++)
    {
        if (servoMembers[i]->getName() == name)
        {
            return true;
        }
    }
    return false;
}

void ServoGroup::setName(string name)
{
	this->groupName = name;
}

string ServoGroup::getName()
{
	return this->groupName;
}

void ServoGroup::enableCompliantMode(bool f)
{
    this->compliantMode = f;
}

bool ServoGroup::ifCompliantMode()
{
    return this->compliantMode;
}
