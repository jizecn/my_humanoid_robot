#include "AutonomousMode.h"

AutonomousMode::AutonomousMode()
{
	this->pos = NULL;
	this->enc = NULL;
	this->torq = NULL;
	motorControl = NULL;
//	currentSession = NULL;
//	this->currentSessionName = "";
//	gameCount = 0;
//	currentCommand = "";
}

AutonomousMode::~AutonomousMode()
{
    if(this->isRunning())
    {
        if(this->stop())
        {
            printf("AutonomousMode stoped\n");
        }
        else
        {
            printf("ERROR: AutonomousMode cannot be stoped\n");
        }
    }
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
    for(int i = 0; i < skinParts.size(); i++)
    {
        if(skinParts[i])
        {
            delete skinParts[i];
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

    printf("Autonomouse deconstructor :: successful\n");
}

int AutonomousMode::initConfiguration(yarp::os::Property &config)
{
	int temp = BehaviourMode::initConfiguration(config);
	//yarp::os::ConstString gamefilepath = config.check("gamefilepath",yarp::os::Value("/home/ze/roboskin/coding/newKasparGui/KasparData/seqs"),"specify game scenario filepath").asString();

	//gameSce.initConfiguration(config.check("gamescenario", yarp::os::Value("gamescenario.txt"), "specify the game scenario file").asString().c_str(), gamefilepath.c_str());

	return temp;
}

void AutonomousMode::beforeStart()
{
	//printf("%d  \n", skinParts.size());
	mutex.wait();	
	for(int i = 0; i < skinParts.size(); i++)
	{
		if(!skinParts[i]->connect())
		{
			printf("%s connection failed\n", skinParts[i]->getPartName().c_str());			
			continue;
		}
		else
		{
			printf("%s connected\n", skinParts[i]->getPartName().c_str());
			skinParts[i]->start();    
		}
	}

	mutex.post();
	ACE_OS::printf("Start thread\n");
}

void AutonomousMode::afterStart(bool success)
{

}

void AutonomousMode::threadRelease()
{
    printf("Thread release\n");
}

void AutonomousMode::onStop()
{
    printf("Thread onStop\n");
	for(int i = 0; i < skinParts.size(); i ++)
	{
        if(skinParts[i]->isRunning())
    		skinParts[i]->stop();
	}
}

void AutonomousMode::run()
{
	for(int i = 0; i < skinParts.size(); i++)
	{
		if(!skinParts[i]->isConnected())
		{
			fprintf(stderr, "WARNING: skin sensor (%s) is not connected to yarp server. check setting\n", skinParts[i]->getPartName().c_str());
			return; // safest way. 
		}
	}

	// reset sensors 
	//yarp::os::Time::delay(0.2);
	this->manualSetDrifts();
	yarp::os::Time::delay(0.2);
	
	int i = 0;
	TactileClassification::TouchEvent te; 
	std::string actionSelected;
	int tmp = 0;
	while (!isStopping()) {
		//double t1 = yarp::os::Time::now(); 
		tmp = 0;

		for(i=0; i<skinParts.size(); i++)
		{
			te = this->skinParts[i]->processData();
			string part = skinParts[i]->getPartName();	

			if(te.type == TactileClassification::poke)
			{
				printf("TOUCH TYPE :::  POKE .......\n");
			}
			else if(te.type == TactileClassification::touch)
			{
				printf("TOUCH TYPE :::  TOUCH .......\n");
			}

			if(te.force == TactileClassification::soft || te.force == TactileClassification::hard)
			{
				if(this->prepare(part))
				{
					//continue;
					// check which servoGroup the activated skin part is linked to. 
					// here, just hard coded temporarily. Should be specified in config files. 
					if(part == "lefthand_top" || part == "lefthand_bottom")
					{
						Servo *leftHand = this->servos.getServo("ARM_L_4");
						ServoGroup *sg = leftHand->getServoGroup();
						this->servosCompliant(sg, 0, false);   // to be more compliant
					}
					else if(part == "righthand_top" || part == "righthand_bottom")
					{
						Servo *rightHand = this->servos.getServo("ARM_R_4");
						ServoGroup *sg = rightHand->getServoGroup();
						this->servosCompliant(sg, 0, false);    // to be more compliant
					}

					if(!this->motorControl->isRunning())
					{
						playSound();

						this->motorControl->setKasparSequence(&this->currentSeq);

						this->motorControl->start();

						if(this->ifLogSequence)
						{
							// log sequence 
							this->logSequence(this->currentSeq.getName().c_str());
						}
					}

				}

				//continue;

				/********** GAME SCENARIO STARTS HERE *********/
/*
				if(!waiting)
				{

					if (this->currentSession != NULL)
					{
						printf("HERE CURRENT SESSION %s\n", this->currentSessionName.c_str());
						//GameTouchInOrder::status st = this->currentSession->isCorrect(part);
						GameTouchInOrder::status st = this->currentSession->isCorrect2(part, te.type);

						printf("STATUS ------- %d\n", st);
						printf(">>>>>>> %d\n", this->currentSession->getCurrentPos());

						if(st == GameTouchInOrder::LAST)
						{
							this->currentSession->reset();
							this->currentSession = NULL;
							this->currentSessionName = "";
							awardVoice(this->correct);
							
							this->logGameStatus("Touch Event Status", "OK ALL DONE");
						}
						else if(st == GameTouchInOrder::CURRENT)
						{
							printf(">>>CURRENT \n");
						}
						else if(st == GameTouchInOrder::NEXT)
						{
							printf("NEXT \n");

							//awardVoice(this->ok);

							this->logGameStatus("Touch Event Status", "OK MOVE NEXT");
						}
						else if (st == GameTouchInOrder::DIFFTYPE)
						{
							printf("DIFFTYPE. Probably the last one, but different touch type \n");
						}
						else if(st == GameTouchInOrder::OTHER || st == GameTouchInOrder::OUTOFBOUNDARY)
						{
							awardVoice(this->wrong);   // wrong 
							this->currentSession = NULL;
							this->currentSessionName = "";
							printf("WRONG\n");
							this->logGameStatus("Touch Event Status", "WRONG");
						}
					}
					else
					{
						printf("CURRENT SESSION is NULL\n");
					}
				}
				*/
				/********** GAME SCENARIO STOPS HERE *********/
			}
			else if(te.type == TactileClassification::none)
			{
				if(part == "lefthand_top" || part == "lefthand_bottom")
				{
					Servo *leftHand = this->servos.getServo("ARM_L_4");
					ServoGroup *sg = leftHand->getServoGroup();
					this->servosCompliant(sg, 1);  // set a higher impedance
				}
				else if(part == "righthand_top" || part == "righthand_bottom")
				{
					Servo *rightHand = this->servos.getServo("ARM_R_4");
					ServoGroup *sg = rightHand->getServoGroup();
					this->servosCompliant(sg, 1);    // set a higher impedance
				}
			}			
		}

		yarp::os::Time::delay(0.1);
	}
}

bool AutonomousMode::threadInit()
{
    return true;
}

void AutonomousMode::enable(bool enable)
{
	this->enabled = enable;
	if(!this->enabled)
	{
		stop();
	}
}


void AutonomousMode::manualSetDrifts()
{
	mutex.wait();
	for(int i = 0; i < this->skinParts.size(); i++)
	{
		if(skinParts[i]->connect())
		{
			skinParts[i]->manualSetDrifts();
			skinParts[i]->logText("Manual reset sensor", "");
		}
	}
	mutex.post();
}

/*
bool AutonomousMode::processData(yarp::os::Bottle *b)
{
	//ACE_OS::printf("Got %s\n", b.toString().c_str());    
	
	yarp::os::Value av = b->get(0);
	double *x = (double *)av.asBlob();
	if(x != NULL)
	{
		for(int k = 0; k < 16; k++)
		{
			ACE_OS::printf("%f\n", x[k]);
		}
	}
	return true;
}*/

void AutonomousMode::servosCompliant(ServoGroup *sg, int torque, bool stop)
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

bool AutonomousMode::action()
{
	playSound();

	// extract kss into poses, then run command of pos->positionMove();
	KasparPose kp;
	ServoPosition spos;
	Servo s;
	double pos; 
	double speed;

	double *poss = new double[servos.getNumServos()];

	double tempTorq = 1;
	for (int i = 0; i < this->currentSeq.size(); i++)
	{
		this->currentSeq.get(i, (this->currentSeqStep));
		kp = currentSeqStep.getPose();
		int delay = (this->currentSeqStep.getDelay());
		int numPos = kp.getNumOfServoPositions();
		//ACE_OS::printf("Selected pose name is:   %s (%d)\n", this->currentSeqStep.getName().c_str(), i);

		for(int i = 0; i < servos.getNumServos(); i++)
		{
			poss[i] = -1;  // invalid value that Yarp driver will not respond to -1.
		}

		for(int j = 0; j < numPos; j++)
		{
			kp.getServoPositionAt(j, spos);
			spos.getServo(s);

			if(s.getType() == Servo::AX12)
			{
				this->torq->getRefTorque(s.getYarpId(), &tempTorq);
			}
			if(tempTorq != 0)
			{
				if( spos.getUnscaledPosition(pos) && spos.getUnscaledSpeed(speed))
				{
					// TODO: Do something with YARP to control motor here
					//ACE_OS::printf("Move now.%s %d %d %f %f %d\n", s.getName().c_str(), s.getYarpId(), s.getHardwareId(), pos, speed, j);

					this->pos->setRefSpeed(s.getYarpId(), speed);
					poss[s.getYarpId()] = pos;
				}
			}
		}
		this->pos->positionMove(poss);

		//ACE_OS::printf("Delay is %d\n", delay);
		// delay for this current pose
		yarp::os::Time::delay(delay/1000.0);
	}
	
	delete [] poss;
	poss = NULL;
	
	if(this->ifLogSequence)
	{
		this->logSequence(this->currentSeq.getName().c_str());
	}
	return true;
}

bool AutonomousMode::prepare(std::string command)
{
	bool ret = BehaviourMode::prepare(command);
	// filter out servo groups being held or touched.

	return ret;
}
