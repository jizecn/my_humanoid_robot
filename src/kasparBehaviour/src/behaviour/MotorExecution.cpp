#include "MotorExecution.h"

MotorExecution::MotorExecution(yarp::dev::IPositionControl *iPos, yarp::dev::IEncoders *enc, yarp::dev::ITorqueControl *torq, Servos *servos)
{
	this->pos = iPos;
	this->servos = servos; 
	this->torq = torq;
	this->enc = enc;
	currentSeq = NULL;
}

MotorExecution::~MotorExecution()
{
}

void MotorExecution::run()
{
	if(this->currentSeq == NULL)
	{
		cout << "Sequence is not set. \n"; 	
		return;
	}

	// extract kss into poses, then run command of pos->positionMove();
	KasparPose kp;
	ServoPosition spos;
	Servo s;
	double pos; 
	double speed;

	double *poss = new double[servos->getNumServos()];

	double tempTorq = 1;
	for (int i = 0; i < this->currentSeq->size(); i++)
	{
		this->currentSeq->get(i, (this->currentSeqStep));
		kp = currentSeqStep.getPose();
		int delay = (this->currentSeqStep.getDelay());
		int numPos = kp.getNumOfServoPositions();
		ACE_OS::printf("Selected pose name is:   %s (%d)\n", this->currentSeqStep.getName().c_str(), i);

		for(int i = 0; i < servos->getNumServos(); i++)
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
					ACE_OS::printf("Move now.%s %d %d %f %f %d\n", s.getName().c_str(), s.getYarpId(), s.getHardwareId(), pos, speed, j);

					this->pos->setRefSpeed(s.getYarpId(), speed);
					poss[s.getYarpId()] = pos;
				}
			}
		}
		this->pos->positionMove(poss);

		ACE_OS::printf("Delay is %d\n", delay);
		// delay for this current pose
		yarp::os::Time::delay(delay/1000.0);
	}
	delete [] poss;
	poss = NULL;
	yarp::os::Time::delay(0.8);
	this->currentSeq = NULL;
}

void MotorExecution::setKasparSequence(KasparSequence *currentSeq)
{
	this->currentSeq = currentSeq;
}
