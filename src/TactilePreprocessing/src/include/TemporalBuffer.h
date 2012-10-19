#ifndef __TEMPORAL_BUFFER_H__
#define __TEMPORAL_BUFFER_H__

#include <yarp/os/Semaphore.h>
#include <yarp/sig/Vector.h>
#include <yarp/sig/Matrix.h>
#include <cmath>
#include <iostream>
#include <vector>
#include "svm.h"
#include "TactileData.h"

using namespace std;

/*
* TemporalBuffer is actually a circular buffer. Once the new inserted item is exceeding the boundary, it starts from zero (beginning).
* The class handles this function by incrementing an internal pointer (index), which points to the current (last) item. 
* For users, users only need to know the time epoch. For example, time = t, data(t) is the t(th) sample in the data, by calling getDataAt(d, t)
* The last data sample can be retrieved by getCurrentData().
*/
class TemporalBuffer : public yarp::sig::Matrix
{
public:
	TemporalBuffer(int bufLen, int taxNum);

	bool insert(const TactileData &data);

	bool getDataAt(TactileData &d, int pos);

	//TactileData getCurrentData();

	void incrementCurrentIndex();
	int getTimeLength();
	int indexOfPosition(int pos);
private:

	int currentPoint;
	int size; 
	int taxelNum;
	//yarp::os::Semaphore mutex;  
};

#endif // __TEMPORAL_BUFFER_H__
