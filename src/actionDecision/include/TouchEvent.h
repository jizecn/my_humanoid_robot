/*
 * Copyright (C) 2011 Ze Ji
 * University of Hertfordshire. 
 * EU FP7 RoboSkin 
 * CopyPolicy: Released under the terms of the GPL v2.0, see GPL.TXT
 */

#ifndef __TOUCH_EVENT_H__
#define __TOUCH_EVENT_H__

#include <string>
#include <yarp/os/Bottle.h>
#include <yarp/os/Stamp.h>

class TouchEvent
{
 public:
  virtual bool init(yarp::os::Bottle &event) = 0;
  
  virtual std::string getType() = 0;
  
  void setTimeStamp(yarp::os::Stamp &stamp)
  {
    this->timeStamp = timeStamp;
  }
  
  yarp::os::Stamp getTimeStamp()
    {
      return this->timeStamp;
    }
  
 protected:
  yarp::os::Stamp timeStamp;
  yarp::os::Bottle event;    
};

#endif // __TOUCH_EVENT_H__
