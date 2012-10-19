#ifndef __EVENT_ACTION_PAIR_H__
#define __EVENT_ACTION_PAIR_H__

#include <vector>
#include "SinglePartEvent.h"
#include "ActionCommand.h"

class EventActionPair
{
 public:
  EventActionPair()
    {
    }

  ~EventActionPair()
    {
    }
  
  void setEventKey(SinglePartEvent &key)
  {
    this->key = key;
  }
  
  bool ifSameKey(SinglePartEvent &nkey)
  {
    return key.compareWith(nkey);
  }
  
  SinglePartEvent getEventKey()
  {
    return this->key;
  }
  
  bool insertActionCommand(ActionCommand &cmd)
  {
    if(checkExisting(cmd))
      {
	// duplicated
	std::cout << "Dupllicated item found. Not inserted" << std::endl;
	return false;
      }
    else
      {
	cmds.push_back(cmd);
	return true;
      }
  }
  
  bool decideActionCommand(ActionCommand &cmd)
  {
    // only the simplest mode here
    if(cmds.size() > 0)
      {
	cmd = cmds[0];
	return true;
      }
    return false;
	
  }

 private:
  SinglePartEvent key;
  std::vector<ActionCommand> cmds;
  
  bool checkExisting(ActionCommand& ncmd)
  {
    for(int i = 0; i < cmds.size(); i++)
      {
	if(ncmd.compareWith(cmds[i]))
	  {
	    return true;
	  }
      }
    return false;
  }
};

#endif //__EVENT_ACTION_PAIR_H__
