/*
* Copyright (C) 2011 Ze Ji
* University of Hertfordshire. 
* EU FP7 RoboSkin 
* CopyPolicy: Released under the terms of the GPL v2.0, see GPL.TXT
*/

#include "ActionDecision.h"

using namespace std;

ActionDecision::ActionDecision()
{
  historyEvents.resize(MAX_NUM_HISTORY_EVENTS);
  for(int i = 0; i< MAX_NUM_HISTORY_EVENTS; i++)
    {
      ClassificationEvent ce;
      historyEvents[i] = ce;
    }
  
  cout << "Action Decision Object created. " << endl;
}

ActionDecision::~ActionDecision()
{
  cout << "Destructor" << endl;
}

// here only a simple reaction mechanism implemented. 
// if higher level reaction needed, inherite from this class and reimplement the follow methods
bool ActionDecision::updateTouchEvent(ClassificationEvent &ce)
{
  /*
  //not necessary here. but could be useful for others
  historyEvents.push_back(ce);
  historyEvents.erase(historyEvents.begin());
  */
  std::vector<SinglePartEvent *> events = ce.lookUpNonEmptyEvents();

  //    for(int i = 0; i < events.size(); i++)
  //{
  //TODO: check with salience map... find the highest one, send command
  // here, normally, size should be just 1. 
  // .... 
  //}
  //    eventActMap;
  
  if(events.size() > 0)
    {
      //  map<SinglePartEvent, ActionCommand, compareEvent>::iterator it;
      
      for(int k = 0; k < events.size(); k++)
	{
	  SinglePartEvent spe = *events[k];
	  
	  cout << "Event Received: " << spe.getPartName() << "  " << spe.getTouchType() << endl;
	  
	  for(int i = 0; i < eventActPairs.size(); i++)
	    {
	      ActionCommand cmd;
	      std::string touchType = spe.getTouchType();
	      std::transform(touchType.begin(), touchType.end(),touchType.begin(), ::toupper);
	      if(touchType == "" || touchType == "NONE")
		{
		  //if( eventActPairs[i].getEventKey().getPartName() == "lefthand" || eventActPairs[i].getEventKey().getPartName() == "righthand")
		  if(spe.getPartName() == "lefthand" || spe.getPartName() == "righthand")
		    {
		      //cmd.setComplianceAction(ActionCommand::COMPLIANT, eventActPairs[i].getEventKey().getPartName());
		      cmd.setComplianceAction(ActionCommand::RIGID, spe.getPartName());
		      //std::cout << "=====================" << spe.getPartName() << std::endl; 
		    }
		  yarp::os::Bottle &b = actionOutPort.prepare();
		  std::string toSend = cmd.toString().c_str();
		  cout << "Send ActionCommand --> " << toSend << endl;
		  b.clear();
		  b.add("(msg_type touch_action_command)");
		  b.add(yarp::os::Value(toSend.c_str()));
		  actionOutPort.write();
		  return true;
		}
	      
	      if(eventActPairs[i].ifSameKey(spe))
		{
		  cout << ">>>>> SinglePartEvent found in database" << endl;
		  
		  if(eventActPairs[i].decideActionCommand(cmd))
		    {
		      //    cout << "Action Command sent:  " << cmd.toString() << endl;
		      
		      cout << spe.getTouchType() << endl;
		      
		      if(spe.getTouchType().find("EXTENDED") != std::string::npos)
			{
			  if( eventActPairs[i].getEventKey().getPartName() == "lefthand" || eventActPairs[i].getEventKey().getPartName() == "righthand")
			    cmd.setComplianceAction(ActionCommand::COMPLIANT, eventActPairs[i].getEventKey().getPartName());
			}
		      yarp::os::Bottle &b = actionOutPort.prepare();
		      std::string toSend = cmd.toString().c_str();
		      cout << "Send ActionCommand --> " << toSend << endl;
		      b.clear();
		      b.add("(msg_type touch_action_command)");
		      b.add(yarp::os::Value(toSend.c_str()));
		      actionOutPort.write();
						return true;
		    }
		}
	    }
	}
      //     it = eventActMap.find(spe);
      
      /*
	if(it != eventActMap.end())
	{
	ActionCommand cmd = (*it).second;
	
	yarp::os::Bottle &b = actionOutPort.prepare();
	
	string toSend = cmd.toString().c_str();
	cout << "Send ActionCommand --> " << toSend << endl;
	b.add(yarp::os::Value(toSend.c_str()));
	actionOutPort.write();
	}
	else
	{
	cout << "No ActionCommand found" << endl;
	
	}
      */
      
    }
  
  return true;
}

bool ActionDecision::updateTouchSalienceMap()
{
  return true;
}

bool ActionDecision::updateKeyboardEvent()
{
  return true;
}

