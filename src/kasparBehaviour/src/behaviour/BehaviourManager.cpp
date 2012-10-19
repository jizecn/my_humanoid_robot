#include "BehaviourManager.h"
#include "AutonomousMode.h"
#include "SoundPlay.h"
BehaviourManager::BehaviourManager() {
    ACE_OS::printf("Create BehaviourManager object\n");
	mode = BehaviourManager::INVALID_MODE;
    //mode = 0;
    //currentMode = (BehaviourMode *) (new UserMode());
    dd = NULL;
    currentMode = NULL;
    //userMode = new UserMode();
    autoMode = new AutonomousMode();
	currentMode = autoMode;
}

BehaviourManager::~BehaviourManager() {
    if (dd) {
        dd->close();
    }
    //delete userMode;
    if(autoMode)
        delete autoMode;
    //delete currentMode;   // NO NEED TO DELETE
}

bool BehaviourManager::initMotorDriver(PolyDriver *pd) {
    dd = pd;
    if (!dd->view(pos)) {
        printf("Problem of open pos device\n");
        return 0;
    }

    if (!dd->view(enc)) {
        printf("Problem of open enc device\n");
        return 0;
    }

    Time::delay(1);
    return true;
}

bool BehaviourManager::setBehaviourMode(BehaviourModeType m) {
    if (m == this->mode)
        return true;
    switch (m) {
        case BehaviourManager::USER_MODE:
        {
            ACE_OS::printf("USER_MODE \n");
			if(this->currentMode->isRunning())
				this->currentMode->stop();
            break;
        }
        case BehaviourManager::AUTO_MODE:
        {
			ACE_OS::printf("AUTO_MODE \n");
			if(!this->currentMode->isRunning())
				this->currentMode->start();
            break;
        }
        case BehaviourManager::HYBRID_MODE:
        {
            ACE_OS::printf("HYBRID_MODE \n");
			if(!this->currentMode->isRunning())
				this->currentMode->start();
            break;
        }
        default:
        {
            ACE_OS::printf("Error with the value of behaviour mode. Nothing changed\n");
            return false;
            //break;
        }
    }
    mode = m;
    return true;
}

BehaviourManager::BehaviourModeType BehaviourManager::getBehaviourMode() {
   // ACE_OS::printf("Behaviour Mode selected is (%d) \n", mode);
    return mode;
}

bool BehaviourManager::initAnalogDriver(PolyDriver* pd) {
    ad = pd;
    if (!ad->view(ana)) {
        ACE_OS::printf("Problem of open ana device\n");
        return 0;
    }
    return true;
}

void BehaviourManager::getCurrentMode(BehaviourMode *bm)
{
	bm = currentMode;
}

void BehaviourManager::initConfiguration(char *filename) {
    ACE_OS::printf("Configuration file (BehaviourManager) name is: %s\n", filename);

    Property config;
    config.fromConfigFile(filename);
    currentMode->initConfiguration(config);
}

bool BehaviourManager::sendCommand(std::string command)
{
    if(currentMode)
    {
        if(this->mode == BehaviourManager::USER_MODE || this->mode == BehaviourManager::HYBRID_MODE)
        {
            if( currentMode->prepare(command))
            {
                currentMode->action();
                return true;
            }
        }
        else
        {
            printf("Current mode is automanous mode, that does not accept user command\n");
            return false;
        }
    }
    else
    {
        fprintf(stderr, "WARNING: Current BehaviourMode is NULL. \n");
    }
	return false;
}
