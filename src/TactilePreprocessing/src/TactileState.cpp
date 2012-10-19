#include "TactileState.h"

using namespace yarp::sig;
using namespace yarp::math;

TactileState::TactileState(int timeDim, int taxNum, int samplingRate)
{
	this->saliencyTaxelInd = -1;
	this->samplingRate = samplingRate;
	this->timeDimension = timeDim;
	this->taxelNum = taxNum;

	cout << timeDim << "    " << taxNum << endl;
	//TRY
	temporalBuffer = new TemporalBuffer(timeDim, taxNum);
	diffBuffer = new TemporalBuffer(timeDim - 1, taxNum);

	// temporarily. Dimension vary depending on the salienceBuffer update method. The basic one is to use 1 value to represnet the whole salience
	salienceBuffer = new TemporalBuffer(timeDim - 1, 1);
	touchTypeBuffer = new TemporalBuffer(timeDim, 1);
	for(int a = 0; a < timeDim; a++)
	{
		(*touchTypeBuffer)(a, 0) = zjMath::NO_EVENT;
	}

	//
	//tempD1 = new TactileData(this->taxelNum);
	//tempD2 = new TactileData(this->taxelNum);
	tempSalienceVec.resize(this->taxelNum);

	recentData.resize(this->taxelNum);
	previousData.resize(this->taxelNum);
	recentData.zero();
	previousData.zero();
	// should be initialised from outside later.
	setThresholdTotalLevel(400);
}

TactileState::~TactileState()
{
	if (temporalBuffer)
		delete temporalBuffer;
	if (diffBuffer)
		delete diffBuffer;
	///if (tempD1)
	//	delete tempD1;
	//if (tempD2)
	//	delete tempD2;
	if (salienceBuffer)
		delete salienceBuffer;

	if(touchTypeBuffer)
		delete touchTypeBuffer;

}

int TactileState::getSaliencyTaxelInd()
{
	return this->saliencyTaxelInd;
}

/*
 void TactileState::setMemoryLength(unsigned int timeDim)
 {
 this->temporalData->setSize(timeDim);
 }
 */

bool TactileState::insertDataSample(const TactileData &ds, bool ifUpdateState,
		methodForSalience type)
{
	if(ifUpdateState)
	{
		previousData = recentData;
		recentData = ds;
	}

	return this->temporalBuffer->insert(recentData);
}

bool TactileState::diffSalienceUpdate()
{
	//	double time0 = yarp::os::Time::now();
	//cout << "Method Time Differentiation" << endl;
	//if (this->temporalBuffer->getTimeLength() < 2)
	//{
	//	cout << "TemporalBuffer is too small. Length must be at least 2"
	//			<< endl;
	//	return false;
	//}

	//temporalBuffer->getDataAt(*tempD1, temporalBuffer->getTimeLength() - 1);
	//temporalBuffer->getDataAt(*tempD2, temporalBuffer->getTimeLength() - 2);

	tempSalienceVec = TactileData::diff(recentData, previousData, true);
	diffBuffer->insert(TactileData(tempSalienceVec));

	//double time1 = yarp::os::Time::now();
	//cout << "Salience updated.... Time " << time1 - time0 << endl;
	return true;
}

bool TactileState::salienceBufferUpdate(methodForSalienceLevel type)
{
	// enum methodForSalienceLevel {totalLevel, maxLevel, localRegional};
	TactileData td(this->tempSalienceVec.size());
	yarp::sig::Vector s(1);
	double sum1 = 0;
	switch (type)
	{
	case totalLevel:
		// simply add all salience values together (timeDiff)
		diffBuffer->getDataAt(td, diffBuffer->getTimeLength() - 1);

		for (int i = 0; i < this->taxelNum; i++)
		{
			//simply sum up
			sum1 += td[i];
			//cout << td[i] << endl;
		}
		s[0] = sum1;
		//cout << s[0] << "  " << sum1 << endl;
		salienceBuffer->insert(s);
		//cout << "Salience Sum is --> " << sum1 << endl;
		break;
	case maxLevel:
		// simply get the maximum of all salience values together (timeDiff)
		diffBuffer->getDataAt(td, diffBuffer->getTimeLength() - 1);

		for (int i = 0; i < this->taxelNum; i++)
		{
			//simply sum up
			sum1 += td[i];
			//cout << td[i] << endl;
		}
		s[0] = sum1;
		//cout << s[0] << "  " << sum1 << endl;
		salienceBuffer->insert(s);
		//cout << "Salience Sum is --> " << sum1 << endl;

		break;
	case localRegional:
		// not sure exactly what I am going to do with it. but roughly similar to potential field generation. Requires geometrical information, neighbouring information.
		break;
	default:
		break;
	}
	return true;
}

bool TactileState::updateSalienceMap(methodForSalience type)
{
	double temp;
	TactileData tempDa;
	switch (type)
	{
	case timeDiff:
		if (!diffSalienceUpdate())
		{
			return false;
		}
		if (!salienceBufferUpdate(totalLevel))
		{
			return false;
		}
		// temp = (this->salienceBuffer->getCurrentData())[0];
		this->salienceBuffer->getDataAt(tempDa, salienceBuffer->getTimeLength()
				- 1);
		if (tempDa[0] > thresholdTotalLevel)
		{
			cout << tempDa[0] << " << ---- salience level.. DIM(" << this->taxelNum << ")" << endl;
		}
		break;
	case entropy:
		// look at: http://www.robots.ox.ac.uk/~timork/Saliency/background.html
		// Or search for paper for a different approach (local feature): A Computational Model for Saliency Maps by Using Local Entropy

		// seems similar to above, but dispose information about time series, and instead relying on statistical properties.
		break;
	default:

		break;
	}

	return true;
}
/*
int TactileState::classifyPattern(DataSample &ds,
		methodForClassification methodClassify)
{
	switch (methodClassify)
	{
	//SVM_RBF, SVM_BoF, Simple_BLOB_SIZE, SVM_INFORMATION_DISTANCE
	case SVM_RBF:

		break;
	case SVM_BoF:

		break;
	case Simple_BLOB_SIZE:

		break;
	case SVM_INFORMATION_DISTANCE:

		break;
	default:
		break;
	}

	return 0;
}
*/

bool TactileState::updateRecentData()
{
	/*
	if (this->temporalBuffer->getTimeLength() < 2)
	{
		cout << "TemporalBuffer is too small. Length must be at least 2"
				<< endl;
		return false;
	}

	temporalBuffer->getDataAt(*tempD1, temporalBuffer->getTimeLength() - 1);
	temporalBuffer->getDataAt(*tempD2, temporalBuffer->getTimeLength() - 2);
	*/
	return true;
}
/*
TactileState::touchType TactileState::updateClassification(
		methodForClassification methodClassify)
{
	// check if pressure level is higher than threshold?
	// TODO: think about if to put all sensors in one vector or in separate vectors?
	// It seems NMF can be more useful if using one vector for all sensors. but why doing that for classification?
	// as all parts are disconnected and far apart from each other, classification of local regions should be treated individually.
	// if current data is lower than threshold, then there is no data, so return none.

	// unnecessary to copy data to tempD1 again here as it has been done in the salience update method.
	// but this cannot gurantee to be callsed before. so just call it again here
	//temporalBuffer->getDataAt(*tempD1, temporalBuffer->getTimeLength() - 1);

	if (!recentData.aboveThreshold())
	{
		return TactileState::none;
	}
	//cout << "Yes, event detected\n";
	// then perform classification
	//recentData.performClassification();

	return TactileState::none;
}
*/
void TactileState::setThresholdTotalLevel(double thres)
{
	this->thresholdTotalLevel = thres;
}

void TactileState::addNewTouchEvent(int classLabel)
{
	yarp::sig::Vector s(1);
	s[0] = classLabel;
	touchTypeBuffer->insert(s);
}

std::string TactileState::temporalPatternClassify(int classLabel)
{
	addNewTouchEvent(classLabel);
	int l = touchTypeBuffer->getTimeLength();
	int i = l - 1;
	//int index = touchTypeBuffer->indexOfPosition(i);
	int flag = classLabel;
	//int countOfEvents = 0;
	//TactileData temp(1);
	std::vector<int> events; 
	while(i >= 0)
	{
		//int tempStatus = (*touchTypeBuffer)(i, 0);
		TactileData td;
		touchTypeBuffer->getDataAt(td, i);
		int tempStatus = td[0];
		if(tempStatus != zjMath::NO_EVENT)
		{
			events.push_back(tempStatus);
			//cout << tempStatus << endl;
			//countOfEvents++;
		}
		else
		{
			break;
		}
		i--;
	}

	//cout << ">>>>>>>>>>>>>>  " << events.size() << endl;

	if(events.size() < zjMath::SHORT_TOUCH_DURATION)
	{
		return "";
	}
	
	std::string tempClass = temporalContinuousEvents(events);
	return tempClass;
}

std::string TactileState::temporalContinuousEvents(std::vector<int> &reversedEvents)
{
	std::string ret = "";
	int numPoke = 0;
	int numTouch = 0;
	int s = reversedEvents.size();

	std::vector<ContinuousEvent> conEvents;
	if(s <= 0)
	{
		return "";
	}
	ContinuousEvent ce(reversedEvents[0], 1);

	for(int i = 1; i < s; i++)
	{
		if(reversedEvents[i] == zjMath::NO_EVENT)
		{
			if(ce.getDuration() > 0)
			{
				conEvents.push_back(ce);
				ce.setDuration(0);
				ce.setEventType(zjMath::NO_EVENT);
			}
			break;  // wont happen.
		}
		//cout << " +++++++++++++++++++++   " << reversedEvents[i] << "    " << ce.getEventType() << endl;

		if(reversedEvents[i] == ce.getEventType())
		{
			ce.setEventType(reversedEvents[i]);
			ce.setDuration(ce.getDuration() + 1);
		}
		else
		{
			if(ce.getDuration() > 0)
			{
				conEvents.push_back(ce);
				ce.setDuration(1);
				ce.setEventType(reversedEvents[i]);
			}
		}
		
		if(reversedEvents[i] == zjMath::POKE)
		{
			numPoke ++;
		}
		if(reversedEvents[i] = zjMath::TOUCH)
		{
			numTouch++;
		}
	}
//	if(s == ce.getDuration())
//	{
		conEvents.push_back(ce);
//	}

	//
	//	const int SHORT_TOUCH_DURATION = 3;
	//const int LONG_TOUCH_DURATION = 50;

	int ces = conEvents.size();
	cout << "Num of events: " << ces << endl;
	if(ces == 0)
	{
		return ret;
	}

	ret = shortOrLongEvent(conEvents[ces-1].getDuration(), conEvents[ces-1].getEventType());
	if(ret.compare("") == 0)
	{
		if(ces == 1)
		{
			return ret;
		}
		else if(ces == 2)
		{
			ret = shortOrLongEvent(conEvents[ces-2].getDuration(), conEvents[ces-2].getEventType());
			if(ret.compare("") == 0)
			{
				if(reversedEvents.size() > zjMath::SHORT_TOUCH_DURATION)
				{
					if(conEvents[ces - 1].getEventType() == zjMath::POKE)
					{
						return "SHORT_POKE";
					}
					else
					{
						return "SHORT_TOUCH";
					}
				}
			}
		}
		else
		{
			// more than 3 different events
			// simple count the num
			if(numTouch > numPoke)
			{
				return shortOrLongEvent(numTouch, zjMath::TOUCH);
			}
			else
			{
				return shortOrLongEvent(numTouch, zjMath::POKE);
			}
		}
	}

	return ret;
}

std::string TactileState::shortOrLongEvent(unsigned int dur, int type)
{
	std::string  ret = "";
	if( dur > zjMath::LONG_TOUCH_DURATION)
	{
		if(type == zjMath::POKE)
		{
			ret = "EXTENDED_POKE";
		}
		else if(type == zjMath::TOUCH)
		{
			ret = "EXTENDED_TOUCH";
		}
		
		return ret;
	}
	else if(dur < zjMath::LONG_TOUCH_DURATION && dur > zjMath::SHORT_TOUCH_DURATION)
	{
		if(type == zjMath::POKE)
		{
			ret = "SHORT_POKE";
		}
		else if(type == zjMath::TOUCH)
		{
			ret = "SHORT_TOUCH";
		}
		return ret;
	}
	return ret;
}