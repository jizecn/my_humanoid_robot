#include "ActionExecutor.h"

using namespace std;

ActionExecutor::ActionExecutor()
{
  cout << "Construct ActionExecutor" << endl;
  
  motorControl = NULL;
}

ActionExecutor::~ActionExecutor()
{
  if(pd.isValid())
    {
      if(pd.close())
        {
	  printf("PolyDriver closed\n");
        }
      else
        {
	  printf("ERROR: PolyDriver cannot be closed\n");
        }
    }
  if(motorControl)
    {
      motorControl->stop();
      delete motorControl;
    }
  if(this->ifLogSequence && this->pSequenceLogFile != NULL)
    {
      fclose(pSequenceLogFile);
    }
  
}

bool ActionExecutor::configure(yarp::os::ResourceFinder& config) {
  int ret = this->initYarpDevice(config);
  if (ret != 0) {
    
    return false;
  }
  
  std::cout << config.getContextPath().c_str() << endl;
  
  //std::string cfgFile = config.check("from", yarp::os::Value("conf/"), "").asString().c_str();
  
  // DevProperty
  yarp::os::ConstString devxmlfile = config.check("devxmlfile",
						  yarp::os::Value("Kaspar1c.xml"),
						  "specify dev xml file --file Kaspar.xml").asString();
  
  //printf("DEVFILE PATH is: %s\nDEVFILE NAME is: %s\n\n",devFilePath.c_str(), devfile.c_str());
  
  ret = servos.initFromXMLFile(config.getContextPath().c_str(),
			       devxmlfile.c_str());
  
  if (ret != KasparFile::OK) {
    printf("Servos initialisation return code %d\n", ret);
    return false;
  }
  
  // Pose Property
  
  yarp::os::ConstString posefilepath =
    config.check("posefilepath", yarp::os::Value("Kaspar1cData/pos"),
		 "specify pose filepath").asString();
  posefilepath = config.getContextPath() + FILESEPARATOR + posefilepath;
  
  // Sequence Property
  yarp::os::ConstString seqfilepath =
    config.check("seqfilepath", yarp::os::Value("Kaspar1cData/seqs"),
		 "specify sequence filepath").asString();
  
  seqfilepath = config.getContextPath() + FILESEPARATOR + seqfilepath;
  // sequence container / indexed by sequence names
  
  yarp::os::ConstString soundPath = config.check("soundFilePath", yarp::os::Value("Kaspar1cData/sounds"), "").asString();
  
  soundFilePath = soundPath.c_str();
  printf("=========== SOUND PATH:   %s \n", soundFilePath.c_str());
    
  //int s = seqfiles.size();
  //int getdir (string dir, vector<string> &files)
  std::vector<std::string> seqfiles;
  if (getdir(seqfilepath.c_str(), seqfiles) != 0) {
    fprintf(stderr, "Cannot find SEQUENCE FILES\n");
    return false;
  }
  int s = seqfiles.size();
  printf("Sequence file size %d\n", s);
  
  //  if (!seqfiles.isNull()) {
  for (int i = 0; i < seqfiles.size(); i++) {
    std::string tmp;
    
    tmp = seqfiles.at(i).c_str();
    zjTrimString(tmp, " \t\n");
    std::cout << tmp << std::endl;
    
    // read sequence file
    KasparSequence kSeq;
    kSeq.setServos(&servos);
    if (kSeq.initFromFile(seqfilepath.c_str(), tmp.c_str(),
			  posefilepath.c_str()) == KasparFile::OK) {
      std::cout << "Added sequence " << kSeq.getName() << std::endl;
      
      sequences.addKasparSequence(kSeq);
    }
  }
  
  // create MotorExecution instant
  motorControl = new MotorExecution(pos, enc, torq, &this->servos);
  
  /*********************/
  
  cout << "module name is :  " << getName() << endl;
  port.open(getName());
  
  cmdPort.open(getName("/kaspar/rpc:i")); // optional command port
  attach(cmdPort); // cmdPort will work just like terminal
  this->initLogFile(config);
  
  return true;
}

int ActionExecutor::initYarpDevice(yarp::os::ResourceFinder &config)
{
  yarp::os::Property options;
  options.put("device", config.check("motordevice", yarp::os::Value("KasparServos"), "motordevice").asString().c_str());

  yarp::os::ConstString motorconfpath = config.getContextPath().c_str();
  yarp::os::ConstString motorconffile = config.check("motorconffile", yarp::os::Value(""),"config file for kaspr motor device").asString(); 
  
  options.fromConfigFile((motorconfpath + FILESEPARATOR + motorconffile).c_str());
  std::cout << (motorconfpath + FILESEPARATOR + motorconffile).c_str() << std::endl;

  if(!pd.open(options))
    {
      printf("Device not open.\n");
      return -1;
    }
  if (!pd.isValid())
    {
      printf("Device not available.\n");
      return -1;
    }
  if(!pd.view(this->pos))
    {
      printf("Problem of open pos device\n");
      return -1;
    }
  
  if(!pd.view(this->enc))
    {
      printf("Problem of open encoder device\n");
      return -1;
    }
  
  if(!pd.view(this->torq))
    {
      printf("Problem of open torque device\n");
      return -1;
    }
  
  return 0;
}


void ActionExecutor::initLogFile(yarp::os::ResourceFinder &config)
{
  // if log data to file
  ifLogSequence = config.check("logsequence");
  
  yarp::os::Value logPathValue = config.find("logfilepath");
  
  string logpath = "";//config.find("logfilepath").asString().c_str();
  
  if(!logPathValue.isNull())
    {
      logpath = logPathValue.asString().c_str();
      printf("Log File Path is 5s\n", logpath.c_str());
    }
  if(ifLogSequence)
    {
      time_t rawtime;
      struct tm * timeinfo;
      time ( &rawtime );
      timeinfo = localtime ( &rawtime );
      std::string timeText = to_string(timeinfo->tm_sec) + "_" + to_string(timeinfo->tm_min) + "_" + to_string(timeinfo->tm_hour) + "_" + to_string(timeinfo->tm_mday)+ "_" + to_string(timeinfo->tm_mon + 1)+ "_" + to_string(timeinfo->tm_year + 1900);//asctime(timeinfo);
      
      sequenceLogFile = "Sequence_" + timeText;
      
      if(logpath != "")
	{
	  sequenceLogFile = logpath + FILESEPARATOR + sequenceLogFile;
	}
      
      printf("LOG SEQUENCE: %s\n", sequenceLogFile.c_str());
      pSequenceLogFile =  fopen (sequenceLogFile.c_str() , "w");
      
      if (pSequenceLogFile == NULL) {
	fprintf(stderr, "Error opening file %s\n", sequenceLogFile.c_str());
	fprintf(stderr, "Log File disabled\n");
	ifLogSequence = false;
      }
    }
  else
    {
      printf("NOT LOG Sequence\n");
    }
}

bool ActionExecutor::updateModule() {
  std::cout << "Run UpdateModule" << std::endl;
  yarp::os::Bottle *tEvent = port.read();
  
  if(port.getEnvelope(stamp))
    {
      std::cout << "OK. got time stamp \n";
    }
  /*
    if(stamp.isValid())  cout << "Stamp is valid" << endl;
    else  cout << "Stamp is invalid" << endl;
  */
  if(tEvent == NULL)
    return false;
  cout << tEvent->toString().c_str() << endl;
  int s = tEvent->size();
  if (s >= 2)
    {
      std::string eventDescrip = tEvent->get(0).toString().c_str();
      yarp::os::Property eventProp;
      eventProp.fromString(tEvent->get(0).toString().c_str());
      //yarp::os::Value eventProp("num_of_touch_classes 1 \n msg_type touchClasses");
      
      yarp::os::Value msgType = eventProp.find("msg_type");
      
      if(msgType.isNull())
	{
	  std::cerr << "ERROR: received message with wrong header: " << eventDescrip << std::endl; 
	}
      
      std::string cmd = msgType.asString().c_str();
      
      if(cmd == "touch_action_command")
	{
	  std::cout << "Touch Reaction Command --> " << cmd << std::endl;
	  std::string msgContent = tEvent->get(1).toString().c_str();
	  std::cout << msgContent << std::endl;
	  yarp::os::Property contentProp;
	  contentProp.fromString(msgContent.c_str());
          
	  if(! executeCommand(contentProp))
	    {
	      std::cout << "NOT EXECUTING ACTION\n";
	    }
	}
      else if(cmd == "keyboard_action_command")
	{
	  std::cout << "Touch Reaction Command --> " << cmd << std::endl;
	  std::string msgContent = tEvent->get(1).toString().c_str();
	  std::cout << msgContent << std::endl;
	  yarp::os::Property contentProp;
	  contentProp.fromString(msgContent.c_str());
          
	  executeCommand(contentProp);
        }
      else if(cmd == "compliance_command")
        {
        }
      else
        {
	  std::cout << cmd << std::endl;
	  std::cout << "Unrecognized command received. " << std::endl;
        }
    }
  else
    {
      std::cout << "Received Empty event. Something is wrong." << std::endl;
    }
  
  return true;
}

bool ActionExecutor::executeCommand(yarp::os::Property &contentProp)
{
  std::string complianceAct = contentProp.check("complianceaction", yarp::os::Value(""), "").asString().c_str();
  std::string compliancePart = "";
  //if(complianceAct == "TRUE" || complianceAct == "true")
  std::transform(complianceAct.begin(), complianceAct.end(),complianceAct.begin(), ::toupper);
  if(complianceAct == "TRUE")
    {
      compliancePart = contentProp.check("compliancepart", yarp::os::Value(""), "").asString().c_str();
      std::cout << "===> " << compliancePart.c_str() << std::endl;
      std::transform(compliancePart.begin(), compliancePart.end(),compliancePart.begin(), ::tolower);
      if(compliancePart != "")
	{
	  Servo *part = NULL;
	  if(compliancePart == "lefthand")
	    {
	      part = this->servos.getServo("ARM_L_4");
	    }
	  else if(compliancePart == "righthand")
	    {
	      part = this->servos.getServo("ARM_R_4");
	    }
	  
	  ServoGroup *sg;
	  if(part != NULL)
	    { 
	      sg = part->getServoGroup();
	      if(sg != NULL)
		{
		  this->servosCompliant(sg, 0, false);
		}
	    }
	  else
	    {
	      std::cout << "No servo found by the give name: " << compliancePart << std::endl;
	    }
	}
    }
  //else if(complianceAct == "FALSE" || complianceAct == "false")
  else if(complianceAct == "FALSE")
    {
      compliancePart = contentProp.check("compliancepart", yarp::os::Value(""), "").asString().c_str();
      std::transform(compliancePart.begin(), compliancePart.end(),compliancePart.begin(), ::tolower);
      if(compliancePart != "")
	{
	  Servo *part = NULL;
	  if(compliancePart == "lefthand")
	    {
	      part = this->servos.getServo("ARM_L_4");
	    }
	  else if(compliancePart == "righthand")
	    {
	      part = this->servos.getServo("ARM_R_4");
	    }
	  
	  ServoGroup *sg;
	  if(part != NULL)
	    {
	      ServoGroup *sg = part->getServoGroup();
	      if(sg != NULL)
		this->servosCompliant(sg, 1);
	    }
	  else
	    {
	      std::cout << "No servo found by the give name: " << compliancePart << std::endl;
	    }
	}
    }
  
  std::string actionName = contentProp.check("name", yarp::os::Value(""), "").asString().c_str();
  cout << actionName << endl;
  bool ifCancelLastCmd = contentProp.check("cancel_last_cmd");
  std::string waitOrAbort = contentProp.check("wait_or_abort", yarp::os::Value("abort"), "").asString().c_str();
  //waitOrAbort = false;
  int priority = contentProp.check("priority");
  
  if(!prepare(actionName))
    {
      return false;
    }
  
  if(!this->motorControl->isRunning())
    {
      std::cout << "MotorControl is not running. Execute action " << actionName << std::endl;
      playSound();
      
      this->motorControl->setKasparSequence(this->currentSeq);
      this->motorControl->start();
      if(this->ifLogSequence)
	{
	  // log sequence 
	  this->logSequence(this->currentSeq.getName().c_str());
	}
      return true;
    }
  else
    {
      /*
	if(ifCancelLastCmd) {
	// cancel the current action if there is any
	// execute(actionName, true);  // true is to cancel
	
	// TODO: Cancel last command
	playSound();
	this->motorControl->setKasparSequence(this->currentSeq);
	this->motorControl->stop();
	this->motorControl->start();
	if(this->ifLogSequence)
	{
	// log sequence 
	this->logSequence(this->currentSeq.getName().c_str());
	}
	return true;
	}
	else {
	if(waitOrAbort == "wait") {
	// wait for last action to finish
	// execute(actionName, false);  // true is to cancel
	this->motorControl->appendSequence(this->currentSeq);
	if(this->ifLogSequence)
	{
	// log sequence 
	this->logSequence(this->currentSeq.getName().c_str());
	}
	return true;
	}
	else if(waitOrAbort == "abort"){
	// abort. not to execute. 
	std::cout << "do nothing. as there is exisitng action being executed." << std::endl;                
	return true;
	}
	}
      */
    }
  return false;
}

void ActionExecutor::logSequence(const char *content)
{
  // log data to file
  time_t rawtime;
  struct tm * timeinfo;
  time(&rawtime);
  timeinfo = localtime ( &rawtime );
  //std::string timeText = asctime(timeinfo);
  if(pSequenceLogFile)
    {
      fprintf(pSequenceLogFile,content);
      fprintf(pSequenceLogFile, " %f ", yarp::os::Time::now());
      fprintf(pSequenceLogFile,asctime(timeinfo));   // asctime() will add "\n" automatically 
    }
  std::cout << content << std::endl;
}

void ActionExecutor::playSound()
{
  // sample program to play a sound file.filename should be obtained from kasparsequence object
  //if(sp.play("nice_tickles_me.wav","D:\\roboskin\\coding\\Kaspar\\roboskin\\app\\ActionExecutor\\Kaspar1cData\\sounds"))
  if(sp.play(this->currentSeq.getSoundFile().c_str(), soundFilePath.c_str()))
    {
      printf("OK to play (%s / %s)\n", soundFilePath.c_str(), this->currentSeq.getSoundFile().c_str());
    }
  else
    {
      printf("Cann't play file  (%s / %s)\n", soundFilePath.c_str(), this->currentSeq.getSoundFile().c_str());
    }
}

void ActionExecutor::servosCompliant(ServoGroup *sg, int torque, bool stop)
{
  int l = sg->numOfServos();
  double tmp = 0;
  for(int i = 0; i < l; i++)
    {
      Servo *sv = sg->getServo(i);
      int yarpId = sv->getYarpId();
      
      if(this->torq->getRefTorque(yarpId, &tmp))   // efficient compared to getTorque, 
	{
	  //printf("%d %d\n", (int)tmp, torque);			
	  if((int)tmp != torque)
	    {
	      this->torq->setRefTorque(yarpId, torque);   // or use setRefTorques?
	      if(this->ifLogSequence)
		{
		  // log compliance status
		  if(torque == 0)
		    this->logSequence("Set compliance on");
		  else if(torque == 1)
		    this->logSequence("Set compliance off");
		}
	    }
	}
      
      if(stop)
	{
	  //this->pos->stop(yarpId);
	}
    }
}

bool ActionExecutor::prepare(std::string command) {
  // afterwards, action() should be called from outside
  //    bool getKasparSequence(std::string seqName, KasparSequence &kSeq);
  //	if(!this->seqMap.get(command, this->currentSeq))
  if(!this->sequences.getKasparSequence(command, this->currentSeq))
    {
      fprintf(stderr, "Failed to get the associated sequence\n");
      return false;
    }
  printf("Selected sequence name is:   %s\n", this->currentSeq.getName().c_str());
  
  return true;
}
