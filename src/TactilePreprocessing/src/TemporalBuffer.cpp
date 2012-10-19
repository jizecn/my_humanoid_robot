#include "TemporalBuffer.h"

// bufLen is the row. To retrieve one tactile data sample, just use operator [r]
TemporalBuffer::TemporalBuffer(int bufLen, int taxNum) : yarp::sig::Matrix(bufLen, taxNum)
{
	currentPoint = 0;
	size = bufLen;
}

bool TemporalBuffer::insert(const TactileData &data)
{
	//mutex.wait();

	//cout << currentPoint << " size is " << size << endl;
	if(!setRow (currentPoint, data))
	{
		return false;
	}
	this->incrementCurrentIndex();
	//mutex.post();

	return true;
}

void TemporalBuffer::incrementCurrentIndex()
{
	currentPoint = (currentPoint + 1) % size;
}

// not working. 
/*TactileData TemporalBuffer::getCurrentData()
{
	TactileData td(this->getRow(this->currentPoint));
	return td;
	}*/

int TemporalBuffer::indexOfPosition(int pos)
{
	if(pos > this->size)
	{
		// wrong
		return -1;
	}
	int t = (pos + this->currentPoint)%this->size;
	//cout << "---" << t << "  "  <<  pos << "  " << this->size << endl;
	return t;
}

int TemporalBuffer::getTimeLength()
{
	return rows();
}

bool TemporalBuffer::getDataAt(TactileData &d, int pos)
{

	int i = this->indexOfPosition(pos);

	if(i == -1)
	{
		cout << "Index of Position invalid" << endl;
		return false;
	}
	d = TactileData(this->getRow(i));

	return true;
}
