/*
* Copyright (C) 2011 Ze Ji
* University of Hertfordshire. 
* EU FP7 RoboSkin 
* CopyPolicy: Released under the terms of the GPL v2.0, see GPL.TXT
*/


#ifndef __ACTION_DECISION_H__
#define __ACTION_DECISION_H__

#include <yarp/os/all.h>
#include <yarp/sig/all.h>
#include <yarp/os/RFModule.h>
#include <yarp/os/Thread.h>
#include <yarp/os/Time.h>
#include <yarp/os/Module.h>
#include <yarp/os/Stamp.h>
#include <iomanip>
#include <yarp/os/ResourceFinder.h>
#include <iostream>
#include "TouchEvent.h"
#include "KeyboardEvent.h"
#include "SinglePartEvent.h"
#include "ClassificationEvent.h"
#include "ActionCommand.h"
#include "pugixml.hpp"
#include <cctype>
#include <algorithm>
#include <sstream>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <map>
#include "EventActionPair.h"

#define MAX_NUM_HISTORY_EVENTS 20
#define FILESEPARATOR "/"

using namespace std;

using namespace yarp::os;
using namespace yarp::sig;
using namespace yarp::sig::draw;

/*
class compareEvent
{
public:
bool operator()(const SinglePartEvent& ce1, const SinglePartEvent& ce2) 
{
cout << ce1.getTouchType() << " <-> " <<  ce2.getTouchType() << "\n" << ce1.getPartName() << " <-> " <<  ce2.getPartName() << endl;
// not compare strength and subpartname here. 
if(ce1.getTouchType() == ce2.getTouchType() && ce1.getPartName() == ce2.getPartName())
{
//          std::cout << " two events are identical" << std::endl;
return false;
}
//	std::cout << " two events are NOT identical" << std::endl;
return true;
}
};
*/


class ActionDecision : public yarp::os::RFModule
{
public:
	ActionDecision();
	virtual ~ActionDecision();

	bool configure(ResourceFinder& config) {
		ct = 0;
		cout << "module name is :  " << getName() << endl;
		port.open(getName());

		std::string outPortName = config.check("out_port", yarp::os::Value("/action_decision/outport"), "").asString().c_str();

		std::cout << "Out Port name ==>" << outPortName << std::endl;
		actionOutPort.open(outPortName.c_str());

		std::cout << config.getContextPath().c_str() << endl;

		//std::string cfgFile = config.check("from", yarp::os::Value("conf/"), "").asString().c_str();

		std::string behXMLFile = config.check("action_decision_xml", yarp::os::Value("simpleActionMap.xml"), "").asString().c_str();
		this->parseXMLActionBehaviour(behXMLFile.c_str(), config.getContextPath().c_str());

		cmdPort.open(getName("/actiondecisioncmd")); // optional command port
		attach(cmdPort); // cmdPort will work just like terminal
		return true;
	}

	// try to interrupt any communications or resource usage
	bool interruptModule() {
		port.interrupt();
		return true;
	}

	bool updateModule() {
		cout << "Run UpdateModule" << endl;
		Bottle *tEvent = port.read();

		if(port.getEnvelope(stamp))
		{
			cout << "OK. got time stamp \n";
		}
		
		if(tEvent == NULL)
			return false;
		int s = tEvent->size();
		if (s > 0)
		{
			std::string eventDescrip = tEvent->get(0).toString().c_str();
			yarp::os::Property eventProp;
			eventProp.fromString(tEvent->get(0).toString().c_str());
			//yarp::os::Value eventProp("num_of_touch_classes 1 \n msg_type touchClasses");


			yarp::os::Value msgType = eventProp.find("msg_type");

			if(msgType.isNull())
			{
				cerr << "ERROR: received message with wrong header: " << eventDescrip << endl; 
			}
			string cmd = msgType.asString().c_str();
			if(cmd == "touchClasses")
			{
				cout << "TouchClasses message --> " << cmd << endl;
				cout << tEvent->get(1).toString().c_str() << endl;

				ClassificationEvent ce;
				ce.init(*tEvent);
				if(stamp.isValid())
				{
					ce.setTimeStamp(stamp);

					cout.setf( ios_base::fixed, ios_base::floatfield );
					cout.precision(2);
					cout << "time stamp is :  " << stamp.getTime() << endl;
					cout << "time stamp count is : " << stamp.getCount() << endl;
				}
				updateTouchEvent(ce);
			}
			else if(cmd == "salience")
			{
			  cout << cmd << endl;
			  cout << tEvent->get(1).asString().c_str() << endl;
			}
			else if(cmd == "keyboard")
			  {
			    cout << cmd << endl;
			    std::string key =  tEvent->get(1).asString().c_str();
			    cout << key << endl;
			    //KeyboardEvent ke;
			    //ke.init(*tEvent);
			    yarp::os::Bottle &b = actionOutPort.prepare();
			    ActionCommand cmd;
			    map<string, ActionCommand>::iterator it;
			    it = keyActMap.find(key);
			    if(it != keyActMap.end())
			      {
				cout << "FOUND KEY " << key << endl;
				cmd = it->second;
				std::string toSend = cmd.toString().c_str();
				cout << "Send ActionCommand --> " << toSend << endl;
				b.clear();
				b.add("(msg_type keyboard_action_command)");
				b.add(yarp::os::Value(toSend.c_str()));
				actionOutPort.write();
			      }
			  }
			else
			{
				cout << cmd << endl;
				cout << "Unrecognized command received. " << endl;
			}
		}
		else
		{
			cout << "Received Empty event. Something is wrong." << endl;
		}

		return true;
	}

	
	double getPeriod()
	{
	return 0.05;
	}
	

	bool respond (const Bottle &command, Bottle &reply)
	{
		cout << "Run Respond... testing \n";
		cout << "----- you just pressed " << command.toString() << "  "<< command.size() <<  endl;
		return true;
	}

	bool parseXMLTouchEvent()
	{
	}

	virtual bool parseXMLActionBehaviour(const char *filename, const char *filepath)
	{
		pugi::xml_document doc;
		pugi::xml_parse_result result = doc.load_file((string(filepath) + string(FILESEPARATOR) + string(filename)).c_str());
		if(result.status != pugi::status_ok)
		{
			cout << "XML file load failed: " << filepath << "/" << filename << endl;
			return false;
		}

		pugi::xml_node reactBeh = doc.child("REACTIONBEHAVIOUR");
		pugi::xml_attribute reactEngine = reactBeh.attribute("METHOD");
		cout << reactEngine.value() << endl;

		if(string(reactEngine.value()) == "SIMPLEMAP")
		{
			//      map<SinglePartEvent, ActionCommand, compareEvent>::iterator it;
			for (pugi::xml_node actionDecision = reactBeh.child("ACTIONDECISION"); actionDecision; actionDecision = actionDecision.next_sibling("ACTIONDECISION"))
			{

				ActionCommand ac;
				pugi::xml_node actionNode = actionDecision.child("ACTION");
				std::string name = actionNode.child_value("NAME");
				std::string cancelLastCmd = actionNode.child_value("CANCELLASTCMD");
				std::transform(cancelLastCmd.begin(), cancelLastCmd.end(),cancelLastCmd.begin(), ::toupper);
				bool ifCancelLastCmd = (cancelLastCmd == "TRUE");
				std::string waitOrAbort = actionNode.child_value("WAITORABORT");
				std::transform(waitOrAbort.begin(), waitOrAbort.end(),waitOrAbort.begin(), ::toupper);
				bool ifWaitOrAbort = (waitOrAbort == "TRUE");
				std::string priority = actionNode.child_value("PRIORITY");
				istringstream buffer(priority);
				int priorityInt;
				buffer >> priorityInt;

				ac.setName(name);
				ac.setCancelLastCmd(ifCancelLastCmd);
				ac.setPriority(priorityInt);
				ac.setWaitOrAbort(ifWaitOrAbort);
				cout << "loading XML. Add ActionCommand : " << ac.toString() << endl;


			  SinglePartEvent spe;
				pugi::xml_node eventNode = actionDecision.child("EVENT");
				pugi::xml_attribute eventType = eventNode.attribute("TYPE");
				if(string(eventType.value()) == "TOUCH")
				  {
				std::string partname = eventNode.child_value("PARTNAME");
				std::string subpartname = eventNode.child_value("SUBPARTNAME");
				std::string type = eventNode.child_value("TYPE");
				std::string strength = eventNode.child_value("STRENGTH");

				spe.setPartName(partname);
				spe.setSubPartName(subpartname);
				spe.setTouchType(type);
				spe.setStrength(strength);

				cout << "loading XML. Add Event :  " << spe.toString() << endl;

				this->insertPair(spe, ac);
				cout << " Pair size is ==> " <<  eventActPairs.size() << endl;
				  }
				else if(string(eventType.value()) == "KEYBOARD")
				  {
				    std::string key = eventNode.child_value("KEY");
				    map<string, ActionCommand>::iterator it;
				    it = keyActMap.find(key);
				    if(it != keyActMap.end())
				      {
					keyActMap.erase(it);
				      }
				    keyActMap[key] = ac;
				cout << "loading XML. Add Keyboard Event :  " << key << endl;

				  }

				// add to map
				//eventActMap[spe] = ac;

				// test if 
				//it = eventActMap.begin();
				//it = eventActMap.insert(it, pair<SinglePartEvent, ActionCommand>(spe,ac));
				//cout << ">>>>  " << eventActMap[spe].toString() << endl;
			}


			/*
			int ii = 0;
			cout << "EVentActMap size is " << (int)eventActMap.size() << endl;
			// show content:
			for ( it=eventActMap.begin() ; it != eventActMap.end(); it++ )
			{
			cout << ii++ << endl;
			SinglePartEvent tspe = (*it).first;
			cout << tspe.toString() << " => " << (*it).second.toString() << endl;
			}

			map<SinglePartEvent, ActionCommand, compareEvent>::iterator itn;
			itn = eventActMap.find(spe);
			if(itn !=  eventActMap.end())
			{
			ActionCommand cmd = (*itn).second;
			cout << "ActionCommand Found:  " << cmd.toString() << endl;
			}
			else
			{
			cout << "Not Found ActionCommand" << endl;
			}

			*/	  
		}
		else if(string(reactEngine.value()) == "MULTIACTIONMAP")
		{

		}
		return true;
	}

private:
	// Make a port for reading and writing images
	BufferedPort<Bottle> port;
	Port cmdPort;
	int ct;
	string eventType;
	yarp::os::Stamp stamp; 
	BufferedPort<Bottle> actionOutPort;

	std::vector<ClassificationEvent> historyEvents;

	bool updateTouchEvent(ClassificationEvent &ce);
	bool updateTouchSalienceMap();
	bool updateKeyboardEvent();
	bool decisionMaking();

	vector<EventActionPair> eventActPairs;
	//map<SinglePartEvent, ActionCommand, compareEvent> eventActMap;
	map<std::string, ActionCommand> keyActMap;

	bool insertPair(SinglePartEvent& spe, ActionCommand& ac)
	{
		int i = 0;
		for(i = 0; i < eventActPairs.size(); i++)
		{
			if(eventActPairs[i].ifSameKey(spe))
			{
				eventActPairs[i].insertActionCommand(ac);
				return true;
			}
		}
		if(i == eventActPairs.size())
		{
			EventActionPair newpair;
			newpair.setEventKey(spe);
			newpair.insertActionCommand(ac);
			eventActPairs.push_back(newpair);
			std::cout << "Created new EventActionPair" << std::endl;
		}
		return true;
	}

};

#endif // __ACTION_DECISION_H__
