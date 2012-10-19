#ifndef _GAME_SCENARIO_H_
#define _GAME_SCENARIO_H_

#include "GameTouchInOrder.h"
#include <map>

using namespace std;

class GameScenario
{
public:
	GameScenario()
	{
	}
	
	~GameScenario()
	{
		for (map<string, GameTouchInOrder *>::iterator it = scenarioList.begin(); it != scenarioList.end(); ++it)
	    {
			delete it->second;
		}
	}

	bool initConfiguration(string filename, string filepath)
	{
		yarp::os::Property gameCfg;

		if(gameCfg.fromConfigFile((filepath + FILESEPARATOR + filename).c_str()))
		{
			yarp::os::Bottle gameTouchOrder = gameCfg.findGroup("touchgameorder").tail();
			for (int i = 0; i < gameTouchOrder.size(); i++)
			{
				yarp::os::Property sensorList;
				yarp::os::Bottle eventSeq(gameTouchOrder.get(i).toString());
				GameTouchInOrder *gameEvent = new GameTouchInOrder();
				gameEvent->initConfiguration(eventSeq);

				this->addGameEvent(gameEvent->getKeySeqName(), gameEvent);
			}
			return true;
		}
		else
		{
			printf("something wrong with the config file %s\n", filename.c_str());
			return false;
		}

	}

	/*
	* key: is the name of command
	*/
	GameTouchInOrder *getGameTouchSequence(string key)
	{
		map<string, GameTouchInOrder *>::iterator it;
		it = scenarioList.find(key);
		//printInfo();
		if (it != scenarioList.end())
		{
			return it->second;
		}

		//printf("XXXXXXXXXXXXXXXXXXXXXXX %d %s\n", this->scenarioList.size(), key.c_str());	
		return NULL;
	}

	void printInfo()
	{
		map<string, GameTouchInOrder *>::iterator it;
		for( it = scenarioList.begin(); it != scenarioList.end(); it++ ) {
			printf("key: %s\n", it->first.c_str());
			it->second->printInfo();
			//std::cout << "word: " << it->first << std::endl;
		}
	}

private:

	map<string, GameTouchInOrder *> scenarioList;

	void addGameEvent(string key, GameTouchInOrder *sensorOrder)
	{
		map<string, GameTouchInOrder *>::iterator it;
		it = scenarioList.find(key);
		if (it != scenarioList.end())
		{
			scenarioList.erase(it);
		}
		scenarioList[key] = sensorOrder;
	}

};

#endif // _GAME_SCENARIO_H_
