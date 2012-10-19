
#ifndef _CARD_H_
#define _CARD_H_

#include <yarp/sig/Vector.h>
#include "TriangleSensor.h"

/**
 * Container of data for one card (with one unique cardId in the network)
 * Each card is connected to several sensor patches, triangles mostly. 
*/
class Card {
 public:
  Card()
    {
      //elements are:
      //sensorsNum=16*12;
      //data.resize(sensorsNum);
      numOfTriangles = 16; 
      tts = (TriangleTouchSensor *)malloc(sizeof(TriangleTouchSensor)*numOfTriangles);
    }

  Card(int cardId)
    {
      //elements are:
      //sensorsNum=16*12;
      //data.resize(sensorsNum);
      numOfTriangles = 16; 
      tts = (TriangleTouchSensor *)malloc(sizeof(TriangleTouchSensor)*numOfTriangles);
      this->setCardId(cardId);
    }
  
  ~Card()
    {
		free(tts);
    }
  
  int getCardId()
  {
    return cardId;
  }

  void setData(unsigned char *msg, unsigned int type, int sensorId)
  {
    if (type) {
      tts[sensorId].setActivationLast5(msg);
    }
    else {
      tts[sensorId].setActivationFirst7(msg);
    }     
  }

 private:
  int cardId; // unique id

  yarp::sig::Vector data;

  int sensorsNum; 

  int numOfTriangles;
  TriangleTouchSensor *tts;

  // since cardId cannot be changed, it is private 
  void setCardId(int cardId)
  {
    this->cardId = cardId;
	
    // temporary 
    for(int i = 0; i < numOfTriangles; i++)
      {
	tts[i].setId(i+cardId);
      }	
  }

};
#endif
