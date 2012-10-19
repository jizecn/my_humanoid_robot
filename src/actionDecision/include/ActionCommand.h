#ifndef __ACTION_COMMAND_H__
#define __ACTION_COMMAND_H__

#include <iostream>
#include <string>

class ActionCommand
{
 public:
  
  enum ComplianceCmd {COMPLIANT, RIGID, NONE};
  ActionCommand()
    {
      priority = -1; // default. no priority
      cancel = false;
      this->waitOrAbort = false;
      this->command = "";
      this->name = "";
      compliance = ActionCommand::NONE;
      complianceMotorPart = "";
    }
  
    ~ActionCommand()
    {
    }

    void setName(std::string name)
    {
      this->name = name;
    }

    /*
    std::string getCommand()
    {
        return this->command;
    }
    */

    std::string getName()
    {
        return this->name;
    }

    void setCancelLastCmd(bool can)
    {
        this->cancel = can;
    }

    bool ifCancelLastCmd()
    {
        return this->cancel;
    }
    
    void setPriority(int p)
    {
        this->priority = p;
    }

    int getPriority()
    {
        return this->priority;
    }

    // true if wait, false if abort
    bool ifWaitOrAbort()
    {
        // if not cancel unfinished action, wait or abort?
        return this->waitOrAbort;
    }
    
    void setWaitOrAbort(bool wa)
    {
        this->waitOrAbort = wa;
    }

    void setComplianceAction(ComplianceCmd compCmd, std::string complMotorPart)
    {
      this->compliance = compCmd;
      this->complianceMotorPart = complMotorPart;
    }
    
    bool compareWith(ActionCommand &cmd)
    {
      if(cmd.ifWaitOrAbort() == this->waitOrAbort && cmd.getPriority() == this->priority && cmd.ifCancelLastCmd() == this->cancel && cmd.getName() == this->name)
        {
	  return true;
        }
      return false;
    }

    yarp::os::ConstString toString()
    {
      std::string ret = "";
      yarp::os::Property prop; 
      prop.put("name", this->name.c_str());
      prop.put("priority", this->priority);
      
      if(this->ifCancelLastCmd())
        {
	  prop.put("cancel_last_cmd","true");
        }
      else
        {
	  if(this->ifWaitOrAbort())
            {
	      prop.put("wait_or_abort", "wait");
            }
	  else
            {
	      prop.put("wait_or_abort", "abort");
            }
        }
      
      if(this->compliance == ActionCommand::COMPLIANT)
	{
	  prop.put("complianceaction", "TRUE");
	  prop.put("compliancepart", this->complianceMotorPart.c_str());
	}
      else if(this->compliance == ActionCommand::RIGID)
	{
	  prop.put("complianceaction", "FALSE");
	  prop.put("compliancepart", this->complianceMotorPart.c_str());
	}
      else if(this->compliance == ActionCommand::NONE)
	{
	  //prop.put("complianceact", "FALSE");
	}
      
      return prop.toString();
    }
    
 private:
    int priority;
    bool cancel;
    bool waitOrAbort;
    std::string name;
    std::string command;
    ComplianceCmd compliance;
    std::string complianceMotorPart;
};

#endif //__ACTION_COMMAND_H__
