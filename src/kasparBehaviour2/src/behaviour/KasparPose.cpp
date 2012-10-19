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

/*
ServoPosition &KasparPose::getServoPositionAt(int i)
{
	return positionList[i];
}
*/

bool KasparPose::getServoPositionAt(int i, ServoPosition &sp)
{
	if(i >= positionList.size())
	{
		ACE_OS::fprintf(stderr, "index out of servoPosition bound\n");
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
	FILE *file = ACE_OS::fopen((string(filepath) + FILESEPARATOR + filename).c_str(), "r");

	int ret = 0;
	char *tmpTok;
	// how long? 100 is enough for here
	char buf[150]; // = (char *)malloc(sizeof(char)*100);
	if (file == NULL) {
		ACE_OS::fprintf(stderr, "Failed to open pose file %s", filename);
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
		//ACE_OS::printf("READ %d LINE: %s\n", i + 1, tmpBuf);
		if (strlen(tmpBuf) == 0) {
			ACE_OS::printf("ERROR HERE. String length is 0\n");
			continue;
		} else if (tmpBuf[0] == ';') {
			continue;
		}
		// process buf

		if (i == 0) {
			this->setName(tmpBuf);
			//        ACE_OS::printf("First line %s\n", tmpBuf);
		} 
		//else if (i == 1) {
		//	if(strcmp(tmpBuf, filename) != 0)
		//    {
		// todo: why this line? !!!!!!!!!!!!!!!!!!

		// no need to compare as below... 
		//ACE_OS::fprintf(stderr, "Filename (%s) does not match the content of the file (%s)\n", filename, tmpBuf );
		//return KasparFile::FileFormatError;
		//    }
		//    this->setFileName(filename);
		//kasPose.setFileName(tmpBuf);
		// ACE_OS::printf("Second line: %s\n", tmpBuf);
		//        } 
		else if (i == 1) {
			// ACE_OS::printf("%s\n", tmpBuf);
			//tmpPoseNum = zjConvertToInt(tmpBuf);
			if(!zjConvertToInt(tmpBuf, tmpPoseNum))
			{
				return KasparFile::NumberFormatError;
			}
			//pfp->numOfPoses = convertToInt(tmpBuf);
		} else {
			// content part
			// string tokenizer ......
			ACE_Tokenizer tok(tmpBuf);
			tok.delimiter_replace(',', 0);

			ServoPosition servoPos;

			tmpTok = tok.next();
			if (!tmpTok) {
				ACE_OS::fprintf(stderr, "Pose Partname format incorrect (NULL)\n");
				return KasparFile::FileFormatError;
			} else {
				servoPos.setPartName(tmpTok);
			}

			// ACE_OS::printf("PARTNAME:::: %s\n", a.partName.c_str());

			tmpTok = tok.next();
			if (!tmpTok) {
				ACE_OS::fprintf(stderr, "Pose Position format incorrect (NULL)\n");
				return KasparFile::FileFormatError;
			} else {
				int tmpInt;
				if(!zjConvertToInt(tmpTok, tmpInt))
				{
					return KasparFile::NumberFormatError;
				}
				servoPos.setPosition(tmpInt);
				//servoPos.setPosition(zjConvertToInt(tmpTok));
				// ACE_OS::printf("POSITION:::: %d\n", a.pos);
			}

			tmpTok = tok.next();
			if (!tmpTok) {
				ACE_OS::fprintf(stderr, "Pose Speed format incorrect (NULL)\n");
				return KasparFile::FileFormatError;
				//throw FileFormatException("Pose Speed format incorrect (NULL)");
			} else {
				int tmpInt;
				if(!zjConvertToInt(tmpTok, tmpInt))
				{
					return KasparFile::NumberFormatError;
				}
				servoPos.setSpeed(tmpInt);
				//ACE_OS::printf("SPEED:::: %d\n", a.speed);
			}

			tmpTok = tok.next();
			if (!tmpTok) {
				ACE_OS::fprintf(stderr, "Pose IfDelay format incorrect (NULL)\n");
				return KasparFile::FileFormatError;
			} else {
				servoPos.setIfDelay(true ? false : strcmp(tmpTok, "true") != 0);
				//ACE_OS::printf("IFDELAY:::: %d\n", a.ifDelay);
			}
			// assign servo to servoPos (servoPosition object)
			servoPos.assignServoFrom(this->servos);

			//... fill into KasparPose
			this->addServoPosition(servoPos);
			//kasPose.addServoPosition(servoPos);
		}
		i++; // line number increments
	} while (true);
	// check if the pose number is right
	if(tmpPoseNum + 2 != i) {
		//ACE_OS::fprintf(stderr, "i is -> %d\t   numOfPoses is -> %d\n\n", i, pfp->numOfPoses + 3);
		ACE_OS::fprintf(stderr, "Pose number (%d) does not match with the line numbers (%d)\n", tmpPoseNum + 2, i);
		return KasparFile::FileFormatError;
	}

	if (ferror(file) || !feof(file)) {
		ACE_OS::fprintf(stderr, "Failed to read pose file %s", filename);
		return KasparFile::FileIOError;
	}
	ret = ACE_OS::fclose(file);
	if (ret == EOF || ret != 0) {
		ACE_OS::fprintf(stderr, "Failed to close pose file %s", filename);
		return KasparFile::FileIOError;
	} else {
		// no problem
	}
	return KasparFile::OK;
}

int KasparPose::getPoseFrom(string name, ACE_Vector<KasparPose> poses)
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