#include "circularBuffer.h"

#include <iostream>

using namespace std;

//template <class T>
CircularBuffer::CircularBuffer(int bufLen, int taxNum)
{
	//buffer = NULL;
	//setSize(bufLen, taxNum);
	buffer.resize(bufLen);

	for(int i = 0; i < bufLen; i++)
	{
		buffer[i] = new DataSample(taxNum);
	}
	currentPoint = 0;

	this->size = bufLen;
	this->taxelNum = taxNum;
	cout << bufLen << "   " << taxNum << " ----- Created Circular Buffer" <<endl;

}

//template <class T>
CircularBuffer::~CircularBuffer()
{
	/*
  if(buffer != 0)
    {
		cout << "size of buffer is .... :" << this->size << endl;
		try{
	  delete[] buffer;
		}
		catch(...)
		{
  cout << "Delete buffer error" << endl;
		}
    }*/

	cout << "Clear Buffer -- size is:   " << buffer.size() << endl;
	for(int i = 0; i < buffer.size(); i++)
	{
		//cout << i << endl;
		if(buffer[i] != 0)
		{
			//cout << buffer[i]->getDim() << endl;
			delete buffer[i];
			//buffer[i]->deleteData();
		}
	} 

  /*	for(int i = 0; i < buffer.size(); i++)
	{
		cout << i << endl;
		if(buffer[i] != 0)
		{
			delete buffer[i];
		}
	}
  */
}

//template <class T> 
bool CircularBuffer::insert(DataSample &data)
{
  *buffer[currentPoint] = data;
  this->incrementCurrentIndex();
  //currentPoint = (currentPoint + 1) % size;
  //cout << currentPoint << endl;
  return true;
}

//template <class T> 
DataSample* CircularBuffer::getCurrentDataPointer()
{
	return buffer[currentPoint];
  //return buffer + currentPoint;
}

//template <class T> 
void CircularBuffer::incrementCurrentIndex()
{
  currentPoint = (currentPoint + 1) % size;
}
/*
void CircularBuffer::clearBuffer()
{
	cout << "Clear Buffer -- size is:   " << buffer.size() << endl;
	for(int i = 0; i < buffer.size(); i++)
	{
		cout << i << endl;
		if(buffer[i] != 0)
		{
			delete buffer[i];
		}
	} 
}
*/
/*
//template <class T> 
void CircularBuffer::setSize(int bufLen, int taxNum)
{
//	buffer.clear();
  //this->clearBuffer();
	buffer.resize(bufLen);
  
	for(int i = 0; i < bufLen; i++)
	{
		buffer[i] = new DataSample(taxNum);
	}
	currentPoint = 0;
  
	this->size = bufLen;
	this->taxelNum = taxNum;
}
*/

// :) using vector is not slower than using array directly, in fact... Just tested.
//template <class T> 
bool CircularBuffer::getData(double **allData, int c, int r)
{
	/*  if(allData.size() != this->size)
    return false;

	for (int i = 0; i < this->size; i++)
    {
      allData[i] = *buffer[(this->currentPoint+i)%size];
    }
	*/
	//allData = new double[c][r];
	if(c != this->size || r != this->taxelNum)
	{
		cout << "Dimension do not match\n";
		return false;
	}

	for(int i = 0; i < c; i++)
	{
		int offset = 0; // TODO
		
		DataSample currentData = *buffer[offset];
		for (int j = 0; j < r; j++)
		{
			allData[i][j] = currentData[j];		
		}
	}
  return true;
}

/*Obsolete*/
/*
template <class T>
bool CircularBuffer<T>::getData(T *data, size_t s)
{
  if (!data || s != this->size)
    return false;
  
  //T* temp = data;
  //memcpy(temp, buffer + this->currentPoint, this->size - this->currentPoint);
  //memcpy(temp + this->currentPoint, buffer, this->currentPoint);
  return true;
}
*/

//template <class T>
int CircularBuffer::indexOfPosition(int pos)
{
	// TODO: 
	cout << "Not implemented yet. \n";
	if(pos > this->size)
	{
		// wrong
		return -1;
	}
	//int t = ((pos + 1) + this->currentPoint)%this->size;
	int t = (pos + this->currentPoint)%this->size;
	cout << "---" << t << "  "  <<  pos << "  " << this->size << endl;
	return t;
	//return -1;
}

//template <class T>
int CircularBuffer::getSize()
{
	return this->size;
}

//template <class T>
bool CircularBuffer::getDataAt(DataSample &d, int pos)
{
	int i = this->indexOfPosition(pos);

	if(i == -1)
	{
		cout << "Index of Position invalid" << endl;
		return false;
	}
	d = *buffer[i];

	return true;
}

//template class CircularBuffer<double>;
//template class CircularBuffer<DataSample>;
