#ifndef __DOUBLE_BUFFER_H__
#define __DOUBLE_BUFFER_H__
#include "DataSample.h"

// testing
#include "TactileData.h"
#include "TemporalBuffer.h"

#include <vector>
using namespace std;

//template <class T>
class CircularBuffer
{
public:
	/**
	n: size of each single buffer
	*/
	CircularBuffer(int bufLen, int taxNum);
	~CircularBuffer();

	bool insert(DataSample& data);

	//void setSize(int bufLen, int taxNum);

	/*get all data of size n (n vectors, each vector is one data sample of all taxels)*/
	bool getData(double **allData, int c, int r);

	/*data should be aloocated already with size s*/
	//bool getData(T *data, size_t s);
	bool getDataAt(DataSample &d, int pos);

	DataSample* getCurrentDataPointer();

	void incrementCurrentIndex();

	int indexOfPosition(int pos);

	int getSize();
private:
	//DataSample *buffer;
	vector<DataSample*> buffer;
	int currentPoint;
	
	//int currentBufferI; // currentBufferI: index of the current buffer, 0 or 1
	//T *currentBuffer;   // either buffer1 or buffer2

	int size; 
	int taxelNum;
	//bool swapCurrentBuffer();

	//void clearBuffer();
};

#endif // __DOUBLE_BUFFER_H__
