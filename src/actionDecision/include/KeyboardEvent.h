#ifndef __KEYBOARD_EVENT_H__
#define __KEYBOARD_EVENT_H__

#include "TouchEvent.h"
#include <vector>
#include <string>

class KeyboardEvent : public TouchEvent
{
 public:
  KeyboardEvent();
  ~KeyboardEvent();
  bool init(yarp::os::Bottle &event);

  std::string getType()
  {
    // this is obvious. but just used to get the name tag, as a standard name
    return "Keyboard";
  }
  /*
  int getNumOfKeys()
  {
    return numOfKeys;
    }*/
  
  void setEvent(std::string key)
  {
    this->cmdEvent = key;
  }

  std::string getEvent()
    {
      ///return cmdList[0];
      return this->cmdEvent;
    }

 private:
  //std::vector<std::string> cmdList;
  int numOfKeys;
  std::string cmdEvent;
};

#endif // __KEYBOARD_EVENT_H__
