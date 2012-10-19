#include "UserMode.h"
#include "KasparPose.h"
#include "KasparSequenceMap.h"
#include "KasparSequenceStep.h"
#include "KasparSequence.h"
#include "Servos.h"
#include "SoundPlay.h"

UserMode::UserMode() {
	ACE_OS::printf("User Mode constructor\n");
}

UserMode::~UserMode() {
//	free(soundPath);
}

int UserMode::initConfiguration(char *file) {
	return 0;
}
/*
int UserMode::initConfiguration(yarp::os::Property &config) {
	ACE_OS::printf("CONTROL MODE INIT CONFIGURATION\n");
*/
	/*
	Value *vControlMode;
	if (!config.check("controlmode", vControlMode)) {
		ACE_OS::fprintf(stderr, "Config file does not have option of controlmode\n");
		return -1;
	} 
	else if (strcmp(vControlMode->asString().c_str(), "usermode") != 0) {
		ACE_OS::fprintf(stderr, "controlmode is not user manual mode (usermode)\n");
		return -1;
	}*/
/*
	// DevProperty
	yarp::os::ConstString devfile = config.check("devfile", yarp::os::Value(
			"Kaspar1a.dev"), "specify dev file --file Kaspar1a.dev").asString();
	yarp::os::ConstString devFilePath = config.check("devfilepath",
			yarp::os::Value("/home/ze/roboskin/coding/newKasparGui"),
			"specify dev filepath").asString();

	ACE_OS::printf("DEVFILE PATH is: %s\nDEVFILE NAME is: %s\n\n",
			devFilePath.c_str(), devfile.c_str());

	int ret = servos.initFromFile(devFilePath.c_str(), devfile.c_str());
	ACE_OS::printf("Servos initialisation return code %d\n", ret);

	// Pose Property
	yarp::os::ConstString posefilepath = config.check("posefilepath",
			yarp::os::Value(
					"/home/ze/roboskin/coding/newKasparGui/Kaspar1AData/pos"),
			"specify pose filepath").asString();
	yarp::os::Bottle posefiles =
			config.findGroup("posefiles", "Pose file list").tail();

	// Sequence Property
	yarp::os::ConstString seqfilepath = config.check("seqfilepath",
			yarp::os::Value(
					"/home/ze/roboskin/coding/newKasparGui/KasparData/seqs"),
			"specify sequence filepath").asString();
	yarp::os::Bottle seqfiles = config.findGroup("seqfiles",
			"Sequence file list").tail();

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
			kSeq.initFromFile(seqfilepath.c_str(), tmp.c_str(), posefilepath.c_str());
			kSeqs.push_back(kSeq);
		}
	} else {
		ACE_OS::fprintf(stderr, "Cannot find SEQUENCE FILES setting from config file\n");
		return -1;
	}

	// .SKM file. KeyMap
	yarp::os::ConstString keymapfile = config.check("keymapfile",
			yarp::os::Value("default.skm"), "specify skm file").asString();
	yarp::os::ConstString keymapfilepath = config.check("keymapfilepath",
			yarp::os::Value(
					"/home/ze/roboskin/coding/newKasparGui/KasparData/seqs"),
			"specify keymap filepath").asString();

    seqMap.initFromFile(keymapfilepath.c_str(), keymapfile.c_str());
	seqMap.assignSeqToMap(kSeqs);

	// soundFilePath
	this->soundFilePath = config.check("soundFilePath", yarp::os::Value(""), "Specify sound file path").asString().c_str();

	return 0;
}*/

bool UserMode::prepare(string command) {
	ACE_OS::printf("PREPARE USERMODE\n");
	// prepare currentSeqStep and the index. in order to call get(int i, xxxxx);
	// i should be incrementing, or zero if the sequence is changed
	// kSeqMap->
	// afterwards, action() should be called from outside
	
	if(!this->seqMap.get(command, this->currentSeq))
	{
		ACE_OS::fprintf(stderr, "Failed to get the associated pose\n");
		return false;
	}
	ACE_OS::printf("Selected sequence name is:   %s\n", this->currentSeq.getName().c_str());
	return true;
}

bool UserMode::action() {

	// sample program to play a sound file.filename should be obtained from kasparsequence object
	SoundPlay sp;

	if(sp.play(this->currentSeq.getSoundFile().c_str(), soundFilePath.c_str()))
	{
		ACE_OS::printf("OK to play\n");
	}

	// extract kss into poses, then run command of pos->positionMove();
	KasparPose kp;
	ServoPosition spos; 
	Servo s;
	double pos; 
	double speed;
					
	for (int i = 0; i < this->currentSeq.size(); i++)
	{
		this->currentSeq.get(i, (this->currentSeqStep));
		kp = currentSeqStep.getPose();
		int delay = (this->currentSeqStep.getDelay());
		int numPos = kp.getNumOfServoPositions();
		ACE_OS::printf("Selected pose name is:   %s (%d)\n", this->currentSeqStep.getName().c_str(), i);
		
		for(int j = 0; j < numPos; j++)
		{
			kp.getServoPositionAt(j, spos);
			spos.getServo(s);
		
			if( spos.getUnscaledPosition(pos) && spos.getUnscaledSpeed(speed))
			{
			//ACE_OS::printf("Selected pose position is:   %d %f %d %d\n", spos.getPosition(), spos.getUnscaledPosition(), s.getMaxPos(), s.getMinPos());

			//ACE_OS::printf("Selected pose speed is:   %d %f\n", spos.getSpeed(), spos.getUnscaledSpeed());
			// TODO: Do something with YARP to control motor here
			ACE_OS::printf("Move now.%s %d %d %f %f\n", s.getName().c_str(), s.getYarpId(), s.getHardwareId(), pos, speed);
			this->pos->setRefSpeed(s.getYarpId(), speed);
			this->pos->positionMove(s.getYarpId(), pos);
			}
		}

		ACE_OS::printf("Delay is %d\n", delay);
		// delay for this current pose
		yarp::os::Time::delay(delay/1000.0);
	}
	if(this->ifLogSequence)
	{
		this->logSequence(this->currentSeq.getName().c_str());
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
	return true;
}

/*
 void UserMode::pause()
 {
 }

 void UserMode::start()
 {
 }

 void UserMode::resume()
 {
 }

 void UserMode::threadInit()
 {
 }


bool UserMode::stop() {
	return true;
}
 */
void UserMode::setPositionDriver(yarp::dev::IPositionControl *pos)
{
	this->pos = pos;
}
