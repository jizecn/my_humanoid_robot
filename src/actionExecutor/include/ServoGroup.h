#ifndef _SERVO_GROUP_
#define _SERVO_GROUP_

class Servo;

#include <iostream>
#include <string>
#include <vector>
#include "Servo.h"

using namespace std;

class ServoGroup {

public:
  ServoGroup();
  ~ServoGroup();
  void addServo(Servo *sv);
  Servo *getServo(int i);
  int numOfServos();
  bool inGroup(string name);
  void setName(string name);
  string getName();
    void enableCompliantMode(bool f);
    bool ifCompliantMode();

private:
  string groupName;
  vector<Servo *> servoMembers;
  bool compliantMode;
};

#endif // _SERVO_GROUP_
