/*
 * KasparPose.cpp
 *
 *  Created on: 30 Jun 2010
 *      Author: ze
 */

#include "KasparPose.h"

KasparPose::KasparPose()
{

}

KasparPose::KasparPose(string name)
{
  this->name = name;
}

KasparPose::~KasparPose()
{

}

int KasparPose::getNumOfServoPositions()
{
  return positionList.size();
}

void KasparPose::addServoPosition(ServoPosition sPos)
{
  this->positionList.push_back(sPos);
}

void KasparPose::removeAllPoses()
{
  this->positionList.clear();   // ACE_Vector clear does not reallocate buffer, except set size 0
}

bool KasparPose::getServoPositionAt(int i, ServoPosition &sp)
{
  if(i >= positionList.size())
    {
      fprintf(stderr, "index out of servoPosition bound\n");
      return false;
    }
  
  sp = positionList[i];
  return true;
}


string KasparPose::getName()
{
  return name;
}

string KasparPose::getFileName()
{
  return filename;
}

void KasparPose::setName(string name)
{
  this->name = name;
}

void KasparPose::setFileName(string filename)
{
  this->filename = filename;
}

KasparFile::RetCode KasparPose::initFromFile(const char *filepath,const char *filename)
{
  //    KasparPose kasPose;
  FILE *file = fopen((string(filepath) + FILESEPARATOR + filename).c_str(), "r");

  int ret = 0;
  char *tmpTok;
  // how long? 100 is enough for here
  char buf[150]; // = (char *)malloc(sizeof(char)*100);
  if (file == NULL) {
    fprintf(stderr, "Failed to open pose file %s", filename);
    return KasparFile::FileIOError;
  }
  
  int i = 0;
  
  int tmpPoseNum = 0;   // used to verify file format
  do {
    char tmpBuf[150];
    if (fgets(buf, 150, file) == NULL) {
      break;
    }
    strcpy(tmpBuf, buf);
    zjStripNewline(tmpBuf);
    //printf("READ %d LINE: %s\n", i + 1, tmpBuf);
    if (strlen(tmpBuf) == 0) {
      printf("ERROR HERE. String length is 0\n");
      continue;
    } else if (tmpBuf[0] == ';') {
      continue;
    }
    // process buf
    
    if (i == 0) {
      this->setName(tmpBuf);
    } 
    //else if (i == 1) {
    //	if(strcmp(tmpBuf, filename) != 0)
    //    {
    // todo: why this line? !!!!!!!!!!!!!!!!!!
    
    // no need to compare as below...
    //return KasparFile::FileFormatError;
    //    }
    //    this->setFileName(filename);
    //kasPose.setFileName(tmpBuf);
    //        } 
    else if (i == 1) {
      //tmpPoseNum = zjConvertToInt(tmpBuf);
      if(!zjConvertToInt(tmpBuf, tmpPoseNum))
	{
	  return KasparFile::NumberFormatError;
	}
      //pfp->numOfPoses = convertToInt(tmpBuf);
    } else {
      // content part
      // string tokenizer ......
            
      ServoPosition servoPos;
      
      Tokenizer tok(tmpBuf,",");
      if(tok.NextToken())
	{
	  std::string t = tok.GetToken();
	  servoPos.setPartName(t);
	}
      else
	{
	  fprintf(stderr, "Pose Partname format incorrect (NULL)\n");
	  return KasparFile::FileFormatError;
	}
      
      if(!tok.NextToken())
	{
	  fprintf(stderr, "Pose Position format incorrect (NULL)\n");
	  return KasparFile::FileFormatError;
	}
      else
	{
	  int tmpInt;
	  
	  if(!zjConvertToInt(tok.GetToken().c_str(), tmpInt))
	    {
	      return KasparFile::NumberFormatError;
	    }
	  servoPos.setPosition(tmpInt);
	}
      
      if(!tok.NextToken())
	{
	  fprintf(stderr, "Pose Speed format incorrect (NULL)\n");
	  return KasparFile::FileFormatError;
	}
      else
	{
	  int tmpInt;
	  if(!zjConvertToInt(tok.GetToken().c_str(), tmpInt))
	    {
	      return KasparFile::NumberFormatError;
	    }
	  servoPos.setSpeed(tmpInt);
	}
      if(!tok.NextToken())
	{
	  fprintf(stderr, "Pose Speed format incorrect (NULL)\n");
	  return KasparFile::FileFormatError;
	}
      else
	{
	  servoPos.setIfDelay(true ? false : strcmp(tok.GetToken().c_str(), "true") != 0);
	}
      
      // assign servo to servoPos (servoPosition object)
      if(!servoPos.assignServoFrom(this->servos))
	{
	  cout << "WRONG in load servos\n";
	}
      
      //... fill into KasparPose
      this->addServoPosition(servoPos);
      //kasPose.addServoPosition(servoPos);
    }
    i++; // line number increments
  } while (true);
  // check if the pose number is right
  if(tmpPoseNum + 2 != i) {
    //ACE_OS::fprintf(stderr, "i is -> %d\t   numOfPoses is -> %d\n\n", i, pfp->numOfPoses + 3);
    fprintf(stderr, "Pose number (%d) does not match with the line numbers (%d)\n", tmpPoseNum + 2, i);
    return KasparFile::FileFormatError;
  }
  
  if (ferror(file) || !feof(file)) {
    fprintf(stderr, "Failed to read pose file %s", filename);
    return KasparFile::FileIOError;
  }
  ret = fclose(file);
  if (ret == EOF || ret != 0) {
    fprintf(stderr, "Failed to close pose file %s", filename);
    return KasparFile::FileIOError;
  } else {
    // no problem
  }
  return KasparFile::OK;
}

int KasparPose::getPoseFrom(string name, std::vector<KasparPose> poses)
{
  int i = -1;
  for(i = 0; i < poses.size(); i++)
    {
      if(name.compare(poses[i].getName()) == 0)
	{
	  return i;
	}
    }
  return -1;
}

void KasparPose::setServos(Servos *s)
{
  this->servos = s;
}
