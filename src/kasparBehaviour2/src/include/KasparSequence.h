/*
 * KasparSequence.h
 *
 *  Created on: 2 Jul 2010
 *      Author: ze
 */

#ifndef KASPARSEQUENCE_H_
#define KASPARSEQUENCE_H_

#include <string>
#include <ace/Vector_T.h>
#include <ace/SString.h>

#ifdef HAVE_TOKENIZER_T_H
#include <ace/Tokenizer_T.h>
#endif 

#include "utils/Resources.h"
#include "utils/Utils.h"
#include "KasparSequenceStep.h"
#include "Servos.h"

using namespace std;

class KasparSequence
{
public:
    KasparSequence()
    {
		this->name = "";
		soundFile = "";
    }

	KasparSequence(string name)
	{
		this->name = name;
		soundFile = "";
	}

	~KasparSequence()
	{

	}

	void setName(string name)
	{
		this->name = name;
	}

	string getName()
	{
		return name;
	}

	bool set(int index, KasparSequenceStep seqStep)
	{
		if (!seqSteps.set(seqStep, index))
		{
			ACE_OS::printf("index is out of valid scope");
			return false;
		}
		return true;
	}

	void append(KasparSequenceStep seqStep)
	{
		seqSteps.push_back(seqStep);
	}

	bool get(int index, KasparSequenceStep &kseq)
	{
		//ACE_OS::printf("SIZE IS %d\n", size());
		//ACE_OS::printf("SEQUENCE STEP at %d is %s\n", index, seqSteps[0].getPose().getName().c_str());
		if (index >= seqSteps.size() || index < 0)
		{
			ACE_OS::printf("index is out of valid scope");
			return false;
		}
		kseq = seqSteps[index];

		return true;
	}

	int size()
	{
		return seqSteps.size();
	}

	/* (similar to the function in KasparPose)
	 * provide the pointer to all servos available, so that each servoposition object can find its own servo object. 
	 * this should be run before initFromFile()
	 * Every servo should be used as a pointer, so that a global change to the property of any servo would take into effect immediately
	 */
	void setServos(Servos *s)
	{
		this->servos = s;
	}

	void setSoundFile(string filename)
	{
		this->soundFile = filename;
	}

	string getSoundFile()
	{
		return this->soundFile;
	}

    KasparFile::RetCode initFromFile(const char *filepath,const char *filename, const char *posefilepath)
    {
		ACE_OS::printf("%s  %s   %s\n", filepath, filename, posefilepath);
		FILE *file = ACE_OS::fopen((string(filepath) + FILESEPARATOR + filename).c_str(), "r");
		int ret = 0;
		// how long? 100 is enough for here
		char buf[150]; // = (char *)malloc(sizeof(char)*100);
		char *tmpTok;

		if (file == NULL) {
			ACE_OS::fprintf(stderr, "Failed to open sequence file %s\n", filename);
			return KasparFile::FileIOError;
		}

		int i = 0;
		int steps = 0;
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
				//ACE_OS::printf("First line %s\n", tmpBuf);
			}
			else if (i == 1) {
				//steps = zjConvertToInt(tmpBuf);
				if(!zjConvertToInt(tmpBuf, steps))
				{
					ACE_OS::printf("Error in converting string to int\n");
					return KasparFile::NumberFormatError;
				}
				//ACE_OS::printf("Second line: %s\n", tmpBuf);
			} 
			else {
				// check if this is the last line for the sound file
				if( i == 2 + steps )
				{
                    printf("Set Sound File %s\n", tmpBuf);
					// the sound file line
					this->setSoundFile(tmpBuf);
					continue;
				}

				// content part

				// string tokenizer ......
				ACE_Tokenizer tok(tmpBuf);
				tok.delimiter_replace(',', 0);
				char poseName[100];
				char poseFileName[100];

				KasparPose kp;

				int delay = 0; 
				tmpTok = tok.next();
				
				if (!tmpTok) {
					ACE_OS::fprintf(stderr, "Sequence (pose name) format incorrect (NULL)\n");
					return KasparFile::FileFormatError;
				} else {
					strcpy(poseName, tmpTok);					
				}

				tmpTok = tok.next();
				
				if (!tmpTok) {
					ACE_OS::fprintf(stderr, "Sequence (pose filename) format incorrect (NULL)\n");
					return KasparFile::FileFormatError;
				} else {
					strcpy(poseFileName, tmpTok);
				}
				//ACE_OS::printf("POSE FILE NAME:::: %s\n", poseFileName);

				kp.setServos(this->servos);
				kp.initFromFile(posefilepath, poseFileName);

				tmpTok = tok.next();
				
				if (!tmpTok) {
					ACE_OS::fprintf(stderr, "Sequence (delay) format incorrect (NULL) %s\n", tmpTok);
					return KasparFile::FileFormatError;
				} else {
					if(!zjConvertToInt(tmpTok, delay))
					{
						ACE_OS::fprintf(stderr, "Sequence Delay Number format error\n");
						return KasparFile::NumberFormatError;
					}
					//delay = zjConvertToInt(tmpTok);
				}
				//... fill into struct pfp
				KasparSequenceStep kss( kp, delay );
				this->append(kss);
			}
			i++;
		} while (true);
		// check if the pose number is right
		//if(this->soundFile.compare("") == 0)
		//{
			if (i != steps + 2) {
				ACE_OS::fprintf(stderr, "Sequence number does not match with line numbers (%d %d)\n", i, steps);
				return KasparFile::FileFormatError;
			}
		//}
		//else
		//{
		//	if (i != steps + 2) {
		//		ACE_OS::fprintf(stderr, "Sequence number does not match with line numbers\n");
		//		return KasparFile::FileFormatError;
		//	}
		//}

		if (ferror(file) || !feof(file)) {
			ACE_OS::fprintf(stderr, "Failed to read pose file %s", filename);
			return KasparFile::FileIOError;
		}
		ret = ACE_OS::fclose(file);
		if (ret == EOF || ret != 0) {
			ACE_OS::fprintf(stderr, "Failed to close pose file %s", filename);
			return KasparFile::FileIOError;
		} 
		return KasparFile::OK;
	}

private:
	string name;
	ACE_Vector<KasparSequenceStep> seqSteps;
	Servos *servos;   // should be initialised outside, and assigned here 
	string soundFile; 
	int currentIndex; 
};

#endif /* KASPARSEQUENCE_H_ */
