/*
 * KasparSequenceStep.h
 *
 *  Created on: 1 Jul 2010
 *      Author: ze
 */

#ifndef KASPARSEQUENCESTEP_H_
#define KASPARSEQUENCESTEP_H_

#include <string>
#include "KasparPose.h"
using namespace std;

class KasparSequenceStep
{
public:

	KasparSequenceStep(int delay = 0)
	{
	}

	KasparSequenceStep(KasparPose pose, int delay = 0)
	{
		this->pose = pose;
		this->delay = delay;

	}

	~KasparSequenceStep()
	{

	}

	string getName()
	{
		return pose.getName();
	}

	void setDelay(int delay)
	{
		this->delay = delay;
	}

	int getDelay()
	{
		return delay;
	}

	/**
	 * return the pointer of pose. No need to delete it
	 */
	KasparPose &getPose()
	{
		return this->pose;
		//kPose = this->pose;
	}

private:

	KasparPose pose;
	int delay;
	string name;

};

#endif /* KASPARSEQUENCESTEP_H_ */
