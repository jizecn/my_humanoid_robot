#include "ClassificationEvent.h"
using namespace std;

ClassificationEvent::ClassificationEvent()
{

}

ClassificationEvent::~ClassificationEvent()
{
    for(int i = 0; i < this->touchClasses.size(); i++)
    {    
        if(this->touchClasses[i] != NULL)
        {
            delete touchClasses[i];
        }
    }            
    touchClasses.clear();
}

bool ClassificationEvent::init(yarp::os::Bottle &eventMessage)
{    
    int size = eventMessage.size();
    yarp::os::Property header;
    header.fromString(eventMessage.get(0).asString().c_str());

    yarp::os::Value prop = header.find("num_of_touch_classes");
    int numOfTouchClasses = 0;
    if(prop.isNull())
    {
        cerr << "WARNING: no number of touch classes information in the event" << endl;
        cerr << " force to use the content. no error checking here" << endl;
        numOfTouchClasses = size - 1;
    }
    else
    {
        numOfTouchClasses = prop.asInt();
        cout << numOfTouchClasses << endl;
        if(numOfTouchClasses != size - 1)
        {
            cerr << "ERROR: received unrecognized command (touchclasses command):  " << numOfTouchClasses << size << endl;
            return false;
        }
    }
    
    /*
    // check if the first item contains correct information to construct classificationEvent
    if (eventMessage.get(0).asString().c_str())
    {
    
    }
    */
    
    for (int i = 1; i < size; i++)
    {
        //cfg.fromString(eventMessage.get(i).asString().c_str(), true);
        yarp::os::Value cfg = eventMessage.get(i);
        if(cfg.isNull())
        {
            cerr << "ERROR: wrong format with classificationEvent: number--> " << i << endl;
            for(int j = 0; j < touchClasses.size(); j++)
            {
                if(touchClasses[j] != NULL)
                {
                    delete touchClasses[j];
                }
                touchClasses.clear();
            }
            return false;
        }
	
	yarp::os::Property propCfg;
	propCfg.fromString(cfg.asString().c_str());
        SinglePartEvent *ev = new SinglePartEvent(propCfg);
        ev->init();
        touchClasses.push_back(ev);
        // additional
        // touchClassesMap
    }
    
    return true;
}
