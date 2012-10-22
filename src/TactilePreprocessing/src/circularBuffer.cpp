#include "circularBuffer.h"

#include <iostream>

using namespace std;

CircularBuffer::CircularBuffer(int bufLen, int taxNum)
{
  buffer.resize(bufLen);
  
  for(int i = 0; i < bufLen; i++) {
    buffer[i] = new DataSample(taxNum);
  }
  currentPoint = 0;
  
  this->size = bufLen;
  this->taxelNum = taxNum;
  cout << bufLen << "   " << taxNum << " ----- Created Circular Buffer" <<endl;
  
}

CircularBuffer::~CircularBuffer()
{
  cout << "Clear Buffer -- size is:   " << buffer.size() << endl;
  for(int i = 0; i < buffer.size(); i++) {
    if(buffer[i] != 0) {
      delete buffer[i];
    }
  } 
}

bool CircularBuffer::insert(DataSample &data)
{
  *buffer[currentPoint] = data;
  this->incrementCurrentIndex();
  return true;
}

DataSample* CircularBuffer::getCurrentDataPointer()
{
  return buffer[currentPoint];
}

void CircularBuffer::incrementCurrentIndex()
{
  currentPoint = (currentPoint + 1) % size;
}

bool CircularBuffer::getData(double **allData, int c, int r)
{
  if(c != this->size || r != this->taxelNum) {
    cout << "Dimension do not match\n";
    return false;
  }
  
  for(int i = 0; i < c; i++) {
    int offset = 0; // TODO
    
    DataSample currentData = *buffer[offset];
    for (int j = 0; j < r; j++) {
      allData[i][j] = currentData[j];		
    }
  }
  return true;
}

int CircularBuffer::indexOfPosition(int pos)
{
  // TODO: 
  cout << "Not implemented yet. \n";
  if(pos > this->size) {
    // wrong
    return -1;
  }
  //int t = ((pos + 1) + this->currentPoint)%this->size;
  int t = (pos + this->currentPoint)%this->size;
  cout << "---" << t << "  "  <<  pos << "  " << this->size << endl;
  return t;
}

int CircularBuffer::getSize()
{
  return this->size;
}

bool CircularBuffer::getDataAt(DataSample &d, int pos)
{
  int i = this->indexOfPosition(pos);
  
  if(i == -1) {
    cout << "Index of Position invalid" << endl;
    return false;
  }
  d = *buffer[i];
  
  return true;
}
