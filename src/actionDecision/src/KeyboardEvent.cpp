#include "KeyboardEvent.h"
#include <iostream>
#include <string>
using namespace std;
using namespace yarp::os;

KeyboardEvent::KeyboardEvent()
{
  this->numOfKeys = 0;
}

KeyboardEvent::~KeyboardEvent()
{
}

bool KeyboardEvent::init(Bottle &event)
{
  this->event = event;
  int size = event.size();
  if(size < 2) {
    cerr << "ERROR: received unrecognized command (keyboard command)" << endl;
    return false;
  }
  //  yarp::os::Value prop = event.get(0).find("num_of_keys");
  //  if(prop.isNull())
  //{
  //cerr << "WARNING: no number of keys information in the keyboard event" << endl;
  //cerr << " force to use the content. no error checking here" << endl;
  // this->numOfKeys = size - 1;
  //}
  //else
  //{
  //this->numOfKeys = prop.asInt();
  // if(numOfKeys != size - 1)
  // {
  //      cerr << "ERROR: received unrecognized command (keyboard command)" << endl;
  //      return false;
  //  }
  //}
  /*
    for (int i = 0; i < this->numOfKeys; i++)
    {
    this->cmdList.push_back(event.get(i+1).asString().c_str());
    }
  */
  this->setEvent(event.get(1).asString().c_str());
  return true;
}

