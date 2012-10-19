#ifndef __KASPAR_SEQUENCES_H__
#define __KASPAR_SEQUENCES_H__

#include <iostream>
#include <vector>
#include "KasparSequence.h"
#include <map>
#include <string>
#include <sys/types.h>

#include <errno.h>
#include <vector>


class KasparSequences
{
public:
    KasparSequences();

    ~KasparSequences();

    bool addSequencesFrom(char *path);

    bool addKasparSequence(KasparSequence &ks);

    int getNumOfSequences()
    {
      return seqMap.size();
    }
//    KasparSequence& getKasparSequence(std::string name);

    bool getKasparSequence(std::string seqName, KasparSequence &kSeq);
//    int assignSeqToMap(vector<KasparSequence> &seqs);
	
//    void addKeyMap(string key, KasparSequence* value);

private:
    std::map<std::string, KasparSequence> seqMap; 
    //std::vector<KasparSequence *> seqList;


};

#endif // __KASPAR_SEQUENCES_H__
