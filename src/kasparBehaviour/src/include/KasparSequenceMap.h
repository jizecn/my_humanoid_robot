/*
 * KasparSequenceMap.h
 *
 *  Created on: 1 Jul 2010
 *      Author: ze
 */

#ifndef KASPARSEQUENCEMAP_H_
#define KASPARSEQUENCEMAP_H_

//#include <ace/Map_T.h>
#include <string>
#include <map>
#include <ace/SString.h>
#include "KasparSequence.h"

#ifdef HAVE_TOKENIZER_T_H
#include <ace/Tokenizer_T.h>
#endif 

using namespace std;

struct keyPair {
    string key;
    string poseName;
};

class KasparSequenceMap
{
public:
	KasparSequenceMap()
	{

	}

	~KasparSequenceMap()
	{

	}

    // return the number of sequences that are inserted, with a found key map
    int assignSeqToMap(ACE_Vector<KasparSequence> &seqs)
    {
        int s = seqs.size();
        int i = 0;
        int j = 0;
        string temp = "";
        int num = 0;   // number of sequences inserted 
        for(j = 0; j < this->keyPairList.size(); j++)
        {
		//			ACE_OS::printf("=========== %d %s\n", j, this->keyPairList[j].poseName.c_str());
			for(i = 0; i < s; i++)
			{
                if(this->keyPairList[j].poseName.compare(seqs[i].getName()) == 0)
                {
					printf("%d %s %s\n", i, seqs[i].getName().c_str(), this->keyPairList[j].poseName.c_str());
                    addKeyMap(this->keyPairList[j].key, &seqs[i]);
                    break;
                }
            }
        }
		ACE_OS::printf("KEY MAP SIZE IS : %d %d %d %d\n", keyMap.size(), num, this->keyPairList.size(), s);
		map<string,KasparSequence *>::iterator it;

		for ( it=keyMap.begin() ; it != keyMap.end(); it++ )
		{
			//std::cout << (*it).first << " => " << (*it).second->getName().c_str() << std::endl;
			ACE_OS::printf("%s => %s\n", (*it).first.c_str(), (*it).second->getName().c_str());
		}

        return num;
    }
	
	void addKeyMap(string key, KasparSequence* value)
	{
		map<string, KasparSequence *>::iterator it;
		it = keyMap.find(key);
		if(it != keyMap.end())
		{
			keyMap.erase(it);  //maybe should change
		}
		map<string, string>::iterator itName;
		itName = keyMapName.find(key);
		if(itName != keyMapName.end())
		{
			keyMapName.erase(itName);  //maybe should change
		}

		keyMap[key] = value;
		keyMapName[key] = value->getName();
		
	}

	bool get(string key, KasparSequence &kSeq)
	{
		map<string, KasparSequence*>::iterator it;
		it = keyMap.find(key);
		if(it == keyMap.end())
		{
			ACE_OS::printf("Key %s has no associated value\n", key.c_str());
			return false;
		}
		//return *it->second;
		kSeq = *(it->second);
		//ACE_OS::printf("&&&&&&&&&&   %s\n", kSeq.getName().c_str());;
		//kSeq = keyMap[key];
		return true;
	}
	
	// TODO: 
    KasparFile::RetCode initFromFile(const char *filepath,const char *filename)
	{

    FILE *file = ACE_OS::fopen((string(filepath) + FILESEPARATOR + filename).c_str(), "r");
    int ret = 0;
    // how long? 100 is enough for here
    char buf[150]; // = (char *)malloc(sizeof(char)*100);
    char *tmpTok;

    if (file == NULL) {
        ACE_OS::fprintf(stderr, "Failed to open key map file %s", filename);
        return KasparFile::FileIOError;
    }

    if (fgets(buf, 150, file) == NULL) {
        ACE_OS::printf("Seems file format is wrong, when reading first line (NULL).\n"); 
        return KasparFile::FileFormatError;
    }

    zjStripNewline(buf);

    // string x = buf + string(".skm");
    //x = x.substr(0, x.length() - 2) + ".skm"; // not a very nice way... but temporarily it is fine here
    //if (strcmp(x.c_str(), filename)) {
    if (strcmp(strcat(buf, ".skm"), filename)) {
        ACE_OS::printf("Seems file format is wrong, when reading first line \n"); 
        return KasparFile::FileFormatError;
    }

    int i = 0; // count line number
    //char poseName[100];
    do {
        //keyMap km;
        char tmpBuf[150];
        if (fgets(buf, 150, file) == NULL) {
            break;
        }
        strcpy(tmpBuf, buf);
        zjStripNewline(tmpBuf);

        ACE_OS::printf("READ %d LINE: %s\n", i + 1, tmpBuf);
        if (strlen(tmpBuf) == 0) {
            ACE_OS::printf("ERROR HERE. String length is 0\n");
            continue;
        } else if (tmpBuf[0] == ';') {
            continue;
        }
        // process buf
        // content part

        // string tokenizer ......
        ACE_Tokenizer tok(tmpBuf);
        tok.delimiter_replace(',', 0);

        tmpTok = tok.next();

        keyPair kpair;

        if (!tmpTok) {
            ACE_OS::printf("Keymap file (pose name) format incorrect (NULL)\n");
            return KasparFile::FileFormatError;
        } else {
            kpair.poseName = tmpTok;
            ACE_OS::printf(kpair.poseName.c_str());
            ACE_OS::printf("  ");
        }

        tmpTok = tok.next();
        if (!tmpTok) {
            ACE_OS::printf("Keymap file (pose key) format incorrect (NULL)\n");
            return KasparFile::FileFormatError; 
        } else {
            kpair.key = tmpTok;
            ACE_OS::printf(kpair.key.c_str());
            ACE_OS::printf("   \n");
        }
        
        // to do 
        this->keyPairList.push_back(kpair);
        i++;
    } while (true);

    if (ferror(file) || !feof(file)) {
        ACE_OS::fprintf(stderr, "Failed to read keymap file %s", filename);
        return KasparFile::FileIOError;
    }
    ret = ACE_OS::fclose(file);
    if (ret == EOF || ret != 0) {
        ACE_OS::fprintf(stderr, "Failed to close keymap file %s", filename);
        return KasparFile::FileIOError;
    } else {
        // no problem
    }

    return KasparFile::OK;
	}

private:
	// all the pointers would be handled outside, by BehaviourManager, BehaviourMode etc.
	map<string, KasparSequence*> keyMap;
    map<string, string> keyMapName;
    ACE_Vector<keyPair> keyPairList;
};

#endif /* KASPARSEQUENCEMAP_H_ */
