/*
 * Copyright (C) 2011 Ze Ji
 * University of Hertfordshire. 
 * EU FP7 RoboSkin 
 * CopyPolicy: Released under the terms of the GPL v2.0, see GPL.TXT
 */


#ifndef __CLASSIFICATION_EVENT_H__
#define __CLASSIFICATION_EVENT_H__

#include "TouchEvent.h"
#include <vector>
#include <string>
#include <map>
#include "SinglePartEvent.h"

class ClassificationEvent : public TouchEvent
{
 public:
  ClassificationEvent();
  ~ClassificationEvent();
  bool init(yarp::os::Bottle &event);

  std::string getType()
  {
    // this is obvious. but just used to get the name tag, as a standard name
    return "Classification";
  }

  int getNumOfParts()
  {
      return touchClasses.size();
  }

  SinglePartEvent * lookUpPart(std::string &partname)
  {
      for(int i = 0; i < touchClasses.size(); i++)
      {
          if (touchClasses[i]->getPartName() == partname)
          {
              return touchClasses[i];
          }
      }
  }

  SinglePartEvent * lookUpPart(std::string &partName, std::string &subPartName)
  {
      for(int i = 0; i < touchClasses.size(); i++)
      {
          if (touchClasses[i]->getPartName() == partName && touchClasses[i]->getSubPartName() == subPartName)
          {
              return touchClasses[i];
          }
      }
  }

  std::vector<SinglePartEvent *> lookUpNonEmptyEvents()
  {
      std::vector<SinglePartEvent *> events;
      for (int i = 0; i < touchClasses.size(); i++)
      {
          if(touchClasses[i]->getTouchType() != "")
          {
              events.push_back(touchClasses[i]);
          }
      }
      return events;
  }

  bool compareWith(ClassificationEvent &ce)
  {
      // TODO: 
      //for(int i 
      //if(ce1.compareWith
      return true;
  }

private:
  //std::map<std::string, SinglePartEvent *> touchClassesMap;
  std::vector<SinglePartEvent *> touchClasses;
};

#endif // __CLASSIFICATION_EVENT_H__
