#include "APFMode.h"

APFMode::APFMode()
{
    
}

APFMode::~APFMode()
{
}

void APFMode::prepare() {

}

bool APFMode::action() {
	return true;
}

// inherited from Thread and BehaviourMode

bool APFMode::stop() {
	return true;
}

// inherited from Thread

void APFMode::beforeStart() {

}

void APFMode::afterStart(bool success) {

}

void APFMode::threadRelease() {

}

void APFMode::onStop() {

}

void APFMode::run() {

}

bool APFMode::threadInit() {
	return true;
}
