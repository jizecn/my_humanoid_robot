#include "BehaviourMode.h"

int BehaviourMode::initConfiguration(char *xmlfile) {
	return 0;
}

int BehaviourMode::initConfiguration(yarp::os::Property &config) {
  printf("CONTROL MODE INIT CONFIGURATION\n");

    int ret = this->initYarpDevice(config);
    if(ret != 0)
    {
        return ret;
    }
	
    // DevProperty
	//yarp::os::ConstString devfile = config.check("devfile", yarp::os::Value("Kaspar1b.dev"), "specify dev file --file Kaspar1b.dev").asString();
	yarp::os::ConstString devFilePath = config.check("devfilepath", yarp::os::Value("/home/ze/roboskin/coding/newKasparGui"), "specify dev filepath").asString();
	yarp::os::ConstString devxmlfile = config.check("devxmlfile", yarp::os::Value("kaspar.xml"), "specify dev xml file --file Kaspar.xml").asString();

	//ACE_OS::printf("DEVFILE PATH is: %s\nDEVFILE NAME is: %s\n\n",devFilePath.c_str(), devfile.c_str());

    ret = servos.initFromXMLFile(devFilePath.c_str(), devxmlfile.c_str());
    
    if(ret != KasparFile::OK)
	{
		ACE_OS::printf("Servos initialisation return code %d\n", ret);
		return -1;
	}

    // Pose Property
	
	yarp::os::ConstString posefilepath = config.check("posefilepath",yarp::os::Value("/home/ze/roboskin/coding/newKasparGui/Kaspar1BData/pos"),"specify pose filepath").asString();

    // Sequence Property
	yarp::os::ConstString seqfilepath = config.check("seqfilepath",yarp::os::Value("/home/ze/roboskin/coding/newKasparGui/KasparData/seqs"),"specify sequence filepath").asString();
	yarp::os::Bottle seqfiles = config.findGroup("seqfiles","Sequence file list").tail();

	int s = seqfiles.size();
	ACE_OS::printf("Sequence size %d\n", s);

	if (!seqfiles.isNull()) {
		for (int i = 0; i < seqfiles.size(); i++) {
			std::string tmp;

			tmp = seqfiles.get(i).toString().c_str();
			zjTrimString(tmp, " \t\n");
			std::cout << tmp << std::endl;

			// read sequence file
			KasparSequence kSeq;
			kSeq.setServos(&servos);
            if(kSeq.initFromFile(seqfilepath.c_str(), tmp.c_str(), posefilepath.c_str()) == KasparFile::OK)
            {
                std::cout << "Added sequence " << kSeq.getName() << std::endl;
			    kSeqs.push_back(kSeq);
            }
		}
	} else {
		ACE_OS::fprintf(stderr, "Cannot find SEQUENCE FILES setting from config file\n");
		return -1;
	}

	// .SKM file. KeyMap
	yarp::os::ConstString keymapfile = config.check("keymapfile",yarp::os::Value("default.skm"), "specify skm file").asString();
	yarp::os::ConstString keymapfilepath = config.check("keymapfilepath",yarp::os::Value("/home/ze/roboskin/coding/newKasparGui/KasparData/seqs"),"specify keymap filepath").asString();

    seqMap.initFromFile(keymapfilepath.c_str(), keymapfile.c_str());
	seqMap.assignSeqToMap(kSeqs);

	// soundFilePath
	this->soundFilePath = config.check("soundFilePath", yarp::os::Value(""), "Specify sound file path").asString().c_str();

	// skin sensors
	bool ifLogData = config.check("logdata");
	yarp::os::ConstString logpath = config.check("logfilepath", yarp::os::Value(""), "specify log file path for data logging if logdata flag is specified").asString();

	yarp::os::Bottle skinfiles = config.findGroup("sensorfiles").tail();
	yarp::os::ConstString skinFilePath = config.check("skinsensorfilepath", yarp::os::Value("C:/roboskin/software_yarp_related/iCub/app/kaspar"), "specifiy skinsensor config file path").asString();
	printf("%s   %d \n", skinFilePath.c_str(), skinfiles.size());
	for(int i = 0; i < skinfiles.size(); i++)
	{
		yarp::os::Property skinproperty;
        yarp::os::Bottle sensorConf(skinfiles.get(i).toString());
		if(skinproperty.fromConfigFile((skinFilePath + FILESEPARATOR + sensorConf.get(0).asString())))
        {
			// put new attribute ifLog to the property before initialising skin sensors
			if(ifLogData)
			{
				skinproperty.put("logdata", ifLogData);
				skinproperty.put("logfilepath", logpath.c_str());
			}
			

    		SkinPart *sk = new SkinPart(skinproperty);   // assuming property file is correct. No error checking and return. so be very careful
	    	sk->setPortToConnect(sensorConf.get(1).asString());
            //yarp::os::ConstString thresfile = skinFilePath + FILESEPARATOR + sensorConf.get(2).asString();
            //sk->initThresholds(thresfile.c_str());
		    skinParts.push_back(sk);
        }
        else
        {
            fprintf(stderr, "Error loading skin sensor config file %s\n", sensorConf.get(0).asString().c_str());
        }
	}

	// set positions to default
	if(resetPositions())
	{
		printf("Servos are set to default positions\n");
	}

	// create MotorExecution instant
	motorControl = new MotorExecution(pos, enc, torq, &this->servos);

	this->initLogFile(config);

	return 0;
}

bool BehaviourMode::prepare(string command) {
	// afterwards, action() should be called from outside

	if(!this->seqMap.get(command, this->currentSeq))
	{
		ACE_OS::fprintf(stderr, "Failed to get the associated pose\n");
		return false;
	}
	ACE_OS::printf("Selected sequence name is:   %s\n", this->currentSeq.getName().c_str());
	return true;
}

bool BehaviourMode::action() {

	playSound();

	// extract kss into poses, then run command of pos->positionMove();
	KasparPose kp;
	ServoPosition spos; 
	Servo s;
	double pos; 
	double speed;
	double *poss = new double[servos.getNumServos()];
					
	for (int i = 0; i < this->currentSeq.size(); i++)
	{
		this->currentSeq.get(i, (this->currentSeqStep));
		kp = currentSeqStep.getPose();
		int delay = (this->currentSeqStep.getDelay());
		int numPos = kp.getNumOfServoPositions();
		ACE_OS::printf("Selected pose name is:   %s (%d)\n", this->currentSeqStep.getName().c_str(), i);
		//double poss[16];
		for(int i = 0; i < servos.getNumServos(); i++)
		{
			poss[i] = -1;
		}
		for(int j = 0; j < numPos; j++)
		{
			kp.getServoPositionAt(j, spos);
			spos.getServo(s);
		
			if( spos.getUnscaledPosition(pos) && spos.getUnscaledSpeed(speed))
			{
				//ACE_OS::printf("Selected pose position is:   %d %f %d %d\n", spos.getPosition(), spos.getUnscaledPosition(), s.getMaxPos(), s.getMinPos());

				//ACE_OS::printf("Selected pose speed is:   %d %f\n", spos.getSpeed(), spos.getUnscaledSpeed());
				// TODO: Do something with YARP to control motor here
				ACE_OS::printf("Move now.%s %d %d %f %f %d\n", s.getName().c_str(), s.getYarpId(), s.getHardwareId(), pos, speed, j);

				this->pos->setRefSpeed(s.getYarpId(), speed);
				poss[s.getYarpId()] = pos;
				//			if(this->pos->positionMove(s.getYarpId(), pos))
				//			{
				//				printf("OK moved to position\n");
				//			}
			}
		}
		this->pos->positionMove(poss);
		/*
		for(int j = 0; j < numPos; j++)
		{
			kp.getServoPositionAt(j, spos);
			spos.getServo(s);
		
			pos = spos.getUnscaledPosition();
			speed = spos.getUnscaledSpeed();
			//ACE_OS::printf("Selected pose position is:   %d %f %d %d\n", spos.getPosition(), spos.getUnscaledPosition(), s.getMaxPos(), s.getMinPos());

			//ACE_OS::printf("Selected pose speed is:   %d %f\n", spos.getSpeed(), spos.getUnscaledSpeed());
			// TODO: Do something with YARP to control motor here
			ACE_OS::printf("Move now.%s %d %d %f %f\n", s.getName().c_str(), s.getYarpId(), s.getHardwareId(), pos, speed);
			this->pos->setRefSpeed(s.getYarpId(), speed);
			if(this->pos->positionMove(s.getYarpId(), pos))
			{
				printf("OK moved to position\n");
			}
		}*/

		ACE_OS::printf("Delay is %d\n", delay);
		// delay for this current pose
		yarp::os::Time::delay(delay/1000.0);
	}
	
	/*
	kss->getPose(kp);
	int delay = kss->getDelay();
	int numPos = kp->getNumOfServoPositions();
	*/
	/*
	for(int i = 0; i < numPos; i++)
	{
		Servo *sv;
		if(delay >=0)
		{
			// as Yarp does not have delay interface, this needs to be implemented here.
			// delay times should be sorted, and started from the shortest one.
			// yarp::os::Time::delay((double)delay);
		}
		ServoPosition d;
		//d = kp->getServoPositionAt(i);
		kp.getServoPositionAt(i, d);
		d.getServo(sv);
		int id = sv->getId();
		pos->positionMove(id, d.getPosition());
	}*/
	delete [] poss;
	poss = NULL;

	if(this->ifLogSequence)
	{
		this->logSequence(this->currentSeq.getName().c_str());
	}

	return true;
}

void BehaviourMode::enable(bool enable)
{
	this->enabled = enable;
}

int BehaviourMode::initYarpDevice(Property &config)
{
	yarp::os::Property options;
	options.put("device", config.check("motordevice", yarp::os::Value("KasparServos"), "motordevice").asString().c_str());
	options.fromConfigFile(config.check("motorconffile", yarp::os::Value("D:/roboskin/coding/roboskin/src/modules/kasparServos/servos.cfg"), "config file for kaspr motor device").asString().c_str());
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

void BehaviourMode::playSound()
{
	// sample program to play a sound file.filename should be obtained from kasparsequence object

	if(sp.play(this->currentSeq.getSoundFile().c_str(), soundFilePath.c_str()))
	{
		ACE_OS::printf("OK to play (%s / %s)\n", soundFilePath.c_str(), this->currentSeq.getSoundFile().c_str());
	}

}

bool BehaviourMode::resetPositions()
{
	int n = 0;
	Servo *sv;
	bool ret = true;
    if(pos->getAxes(&n))
    {
        for(int i = 0; i < n; i++)
        {
			sv = servos.getServo(i);
			if(sv != NULL)
			{
				ret = this->pos->setRefSpeed(i, sv->getDefaultSpeed());
				ret = ret && this->pos->positionMove(i, sv->getDefaultPos());
		        // initialise all axes with high torques
				
	            ret = ret && this->torq->setRefTorque(i, 1);   // or use setRefTorques?
			}
			else
			{
				printf("Servo not found (%d)\n", i);
				ret = false;
			}
		}
    }

	return ret;
}

void BehaviourMode::initLogFile(Property &config)
{
	// if log data to file
	ifLogSequence = config.check("logsequence");
	yarp::os::Value nameValue = config.find("childName");
	yarp::os::Value logPathValue = config.find("logfilepath");
	std::string childName = "";
	string logpath = "";//config.find("logfilepath").asString().c_str();

	if(!nameValue.isNull())
	{
		childName = nameValue.asString().c_str();
		printf("Child name is %s\n", childName.c_str());
	}
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

		if(childName != "")
		{
			this->sequenceLogFile = "Sequence_" + childName + "_" + timeText;
		}
		else
		{
			sequenceLogFile = "Sequence_" + timeText;
		}

		if(logpath != "")
		{
			sequenceLogFile = logpath + FILESEPARATOR + sequenceLogFile;
		}

		printf("LOG SEQUENCE\n");
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


void BehaviourMode::logSequence(const char *content)
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
}
