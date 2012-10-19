#ifndef _GAME_TOUCH_IN_ORDER_H_
#define _GAME_TOUCH_IN_ORDER_H_

#include <string>
#include <ace/Vector_T.h>
#include <ace/SString.h>
#include <vector>
#include <string>
#include <yarp/os/Property.h>
#include "signalprocess/TactileClassification.h"

using namespace std;

/**
* Game interact with Kaspar
* This class specifies the order of sensors that user should touch
* e.g. leftarm, leftfoot...
* This class only works as a container of the sequence of events
*/
class GameTouchInOrder
{
public:
	enum status {CURRENT, NEXT, LAST, OUTOFBOUNDARY, OTHER, DIFFTYPE};
	GameTouchInOrder()
	{
		currentInd = -1;
	}

	~GameTouchInOrder()
	{
	}


	int getCurrentPos()
	{
		return this->currentInd;
	}

	bool isCurrentTouched()
	{
		if(this->currentInd == -1)
			return true;   // must be buggy
		return this->statusList[this->currentInd];
	}

	bool initConfiguration(yarp::os::Bottle &cfg)
	{
		this->seqKey = cfg.get(0).asString().c_str();
		printf("%s %d\n", seqKey.c_str(), cfg.size());
		for(int i = 1; i < cfg.size(); i++)
		{
			// format: temp==> sensorPart:touchType
			string temp = cfg.get(i).asString().c_str();

			printf("%s\n", temp.c_str());

			char *tmpCharArray;
			tmpCharArray = (char*)malloc(sizeof(char)*temp.length());
			
			strcpy(tmpCharArray, temp.c_str());

			ACE_Tokenizer tok(tmpCharArray);

			tok.delimiter_replace(':', 0);

			char *tmpTok;
			tmpTok = tok.next();
			printf("part name %s\n", tmpTok);


			//////////////////////////
			/*
				HARD CODE HAND PARTS... TOP and BOTTOM parts are treated as one part... 
			*/
			string touchPart = hardCodeConvertHandPartsName(tmpTok);
		
			////////////////////////////////


			this->sensorOrder.push_back(touchPart);

			tmpTok = tok.next();
			printf("type name %s\n", tmpTok);

			this->touchTypeOrder.push_back(tmpTok);

			if(strcmp(tmpTok, "poke") == 0)
			{
				this->touchTypeOrder1.push_back(TactileClassification::poke);
			}
			else if(strcmp(tmpTok, "touch") == 0)
			{
				this->touchTypeOrder1.push_back(TactileClassification::touch);
			}
			else
			{
				this->touchTypeOrder1.push_back(TactileClassification::poke);
			}


			this->statusList.push_back(false);
			this->touchForceOrder.push_back(TactileClassification::soft);

			free(tmpCharArray);
		}
		//printf("SIZE %d\n", sensorOrder.size());
		printInfo();

		return true;
	}

	/*
	* 
	*/
	/*
	bool isCurrent(string touchEvent)
	{
		bool temp = false;
		if(currentInd >= 0 || currentInd < sensorOrder.size())
		{
			temp = (touchEvent == sensorOrder[currentInd]);
		}
		else
		{
			temp = false;
		}
		return temp;
	}
*/
	int indexOf(string touchEvent)
	{
		for (int i = 0; i < sensorOrder.size(); i++)
		{
			if(touchEvent == sensorOrder[i])
			{
				return i;
			}
		}
		return -1;
	}

	status getStatus(string touchEvent)
	{
		//printf("%s --- %d %s...\n", touchEvent.c_str(), this->currentInd, sensorOrder[currentInd].c_str());

		if((int)currentInd >= (int)this->sensorOrder.size())
		{
			printf("Current Ind %d   %d\n\n", currentInd, this->sensorOrder.size());
			return this->OUTOFBOUNDARY;
		}

		//if(ifLast(touchEvent))
		//{
		//	return LAST;
		//}		
		
		if(this->currentInd != -1 && touchEvent == sensorOrder[currentInd])
		{
			return CURRENT;
		}
		else if(touchEvent == sensorOrder[currentInd + 1])
		{
			return NEXT;
		}
		else
		{
			return OTHER;
		}
	}

	bool ifLast(string touchEvent)
	{
		if(touchEvent == sensorOrder[sensorOrder.size() - 1])
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	int shiftNext()
	{
		currentInd ++;
		if(currentInd >= this->sensorOrder.size())
		{
			return -1;
		}
		return currentInd;
	}

	string getKeySeqName()
	{
		return seqKey;
	}

	void printInfo()
	{
		printf("%s %d\n", this->seqKey.c_str(), this->sensorOrder.size());
		for (int i = 0; i < this->sensorOrder.size(); i++)
		{
			printf("%s\n", this->sensorOrder[i].c_str());

		}
	}

	void reset()
	{
		currentInd = -1;
		printf("RESET\n");
		for(int i = 0; i < this->statusList.size(); i++)
		{
			printf("RESET ====  %d \n",i);
			this->statusList[i] = false;
		}
		this->setAllTouchForceOrder(TactileClassification::soft);
		this->setAllTouchTypeOrder(TactileClassification::poke);
	}

	status isCorrect(string touchPart)
	{
		//////////////////////////
		/*
			HARD CODE HAND PARTS... TOP and BOTTOM parts are treated as one part... 
		*/
		touchPart = hardCodeConvertHandPartsName(touchPart);
		
		////////////////////////////////


		status st = this->getStatus(touchPart);

		if(st == this->OUTOFBOUNDARY)
		{
			return this->OUTOFBOUNDARY;
		}
		
		if(ifLast(touchPart))
		{
			if(currentInd > 0 && currentInd < this->sensorOrder.size())
			{
				if(this->statusList[this->currentInd - 1] == false)
				{
					return OTHER;
				}
			}

			printf("LAST HERE\n\n");
			this->statusList[this->currentInd] = true;

			return LAST;
		}

		if(st == this->CURRENT)
		{
			this->statusList[this->currentInd] = true;
			
			if(ifLast(touchPart))
			{
				return LAST;
			}
			printf("<<<<%s>>>>>>\n", touchPart.c_str());
			return CURRENT;
		}
		else if(st == this->NEXT)
		{
			if(this->statusList[this->currentInd] == false)
			{
				return OTHER;
			}
	
			if(ifLast(touchPart))
			//if(st == this->LAST)
			{
				printf("LAST HERE\n\n");
				this->statusList[this->currentInd] = true;
				return LAST;
			}

			this->shiftNext();
			printf("\n%d\n", currentInd);
			if(this->currentInd >= this->sensorOrder.size())
			{				
				printf("LAST HERE\n\n");
				return OUTOFBOUNDARY;
			}
			else
			{
				this->statusList[this->currentInd] = true;
				return NEXT;
			}
		}
		else
		{
			return OTHER;
		}
		//		return 0;
	}

	/*STUPID IMPLEMENTATION... SHOULD CHANGE IN FUTURE*/

	status isCorrect2(string touchPart, TactileClassification::TouchType touchType)
	{

		//////////////////////////
		/*
			HARD CODE HAND PARTS... TOP and BOTTOM parts are treated as one part... 
		*/
		touchPart = hardCodeConvertHandPartsName(touchPart);
		
		////////////////////////////////

		status st = this->getStatus(touchPart);

		if(st == this->OUTOFBOUNDARY)
		{
			return this->OUTOFBOUNDARY;
		}



		/*
		if(st == this->LAST)
		{
			if(currentInd > 0 && currentInd < this->sensorOrder.size())
			{
				if(this->statusList[this->currentInd - 1] == false)
				{
					return OTHER;
				}
			}

			printf("LAST HERE %d\n\n", currentInd);

			if(touchType == this->touchTypeOrder1[currentInd])
			{
				this->statusList[this->currentInd] = true;
				return LAST;
			}
			
			return DIFFTYPE;
		}
		*/

		if(st == this->CURRENT)
		{
			//printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>TYPE %d  %d\n", touchType, this->touchTypeOrder1[currentInd]);
			if(touchType == this->touchTypeOrder1[currentInd])
			{
				this->statusList[this->currentInd] = true;
			}

			if(ifLast(touchPart))
			{
				if(touchType == this->touchTypeOrder1[currentInd])
				{
					this->statusList[this->currentInd] = true;
					return LAST;
				}
				else
				{
					return DIFFTYPE;
				}
			}

			//printf("<<<<%s>>>>>>\n", touchEvent.c_str());
			return CURRENT;
		}
		else if(st == this->NEXT)
		{
			if(this->currentInd != -1)
			{
				if(this->statusList[this->currentInd] == false)
				{
					//printf("OTHER TYPE OF TOUCH... \n\n");
					return OTHER;
				}
			}
			this->shiftNext();

			if(ifLast(touchPart))
			{
				printf("LAST HERE\n\n");
				if(touchType == this->touchTypeOrder1[currentInd])
				{
					this->statusList[this->currentInd] = true;
					return LAST;
				}
				else
				{
					return DIFFTYPE;
				}
			}

			// this->shiftNext();
			printf("\n%d\n", currentInd);
			if(this->currentInd >= this->sensorOrder.size())
			{				
				printf("OUT OF BOUND HERE\n\n");
				return OUTOFBOUNDARY;
			}
			else
			{
				if(touchType == this->touchTypeOrder1[currentInd])
				{
					this->statusList[this->currentInd] = true;
				}
				return NEXT;
			}
		}
		else
		{
			printf("OTHER TYPE OF TOUCH... \n\n");
			return OTHER;
		}
	}


	int getSize()
	{
		return sensorOrder.size();
	}

	string toString()
	{
		stringstream ss;
		ss << this->seqKey << " ";
		for(int i = 0; i < this->getSize(); i++)
		{
			ss << sensorOrder[i] << ":" << touchTypeToString(this->touchTypeOrder1[i]) << ":" << touchForceToString(this->touchForceOrder[i]) << " ";
		}
		//ss << "\n";
		return ss.str();
	}


	void setTouchTypeOrder(vector<TactileClassification::TouchType> tList)
	{
		touchTypeOrder1 = tList;
	}

	void setAllTouchTypeOrder(TactileClassification::TouchType type)
	{
		for(int i = 0; i < touchTypeOrder1.size(); i++)
		{
			touchTypeOrder1[i] = type;
		}
	}

	void setAllTouchForceOrder(TactileClassification::TouchForce force)
	{
		for(int i = 0; i < touchForceOrder.size(); i++)
		{
			touchForceOrder[i] = force;
		}
	}
	bool touchTypeCorrect(TactileClassification::TouchType touchType)
	{
		// no error checking. Be careful with this. 
		if(touchType == this->touchTypeOrder1[currentInd])
		{
			return true;
		}
		else
		{
			return false;
		}
	}
		

private:
	
	int currentInd;
	vector<string> sensorOrder;    // sensor part name
	vector<string> touchTypeOrder;    // touch type
	vector<TactileClassification::TouchType> touchTypeOrder1; 
	vector<TactileClassification::TouchForce> touchForceOrder;

	string seqKey; // the key (Kaspar sequence) 
	vector<bool> statusList;


	// Only to treat two hand parts the same... Due to the hardware setting. 
	string hardCodeConvertHandPartsName(string touchPart)
	{
		//////////////////////////
		/*
			HARD CODE HAND PARTS... TOP and BOTTOM parts are treated as one part... 
		*/
		if(touchPart == "lefthand_top" || touchPart == "lefthand_bottom")
		{
			touchPart = "lefthand";
		}
		else if(touchPart == "righthand_top" || touchPart == "righthand_bottom")
		{
			touchPart = "righthand";
		}

		////////////////////////////////
		return touchPart;
	}

	string touchTypeToString(TactileClassification::TouchType tType)
	{
		if(tType == TactileClassification::poke)
		{
			return "poke";
		}
		else if(tType == TactileClassification::touch)
		{
			return "touch";
		}
		else 
		{
			return "na";
		}
	}
	
	string touchForceToString(TactileClassification::TouchForce tForce)
	{
		if(tForce == TactileClassification::soft)
		{
			return "soft";
		}
		else if(tForce == TactileClassification::hard)
		{
			return "hard";
		}
		else 
		{
			return "none";
		}
	}
};



#endif // _GAME_TOUCH_IN_ORDER_H_