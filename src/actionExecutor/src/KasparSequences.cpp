#include "KasparSequences.h"

using namespace std;

KasparSequences::KasparSequences()
{
}

KasparSequences::~KasparSequences()
{
}

bool KasparSequences::addKasparSequence(KasparSequence &ks)
{
    map<string, KasparSequence>::iterator it;
    std::string seqName = ks.getName();
    it = seqMap.find(seqName);
    if(it != seqMap.end())
    {
        seqMap.erase(it);  //maybe should change
    }
    
    seqMap[seqName] = ks;

    return true;
}

bool KasparSequences::getKasparSequence(std::string seqName, KasparSequence &kSeq)
{
    map<string, KasparSequence>::iterator it;
    it = seqMap.find(seqName);
    if(it == seqMap.end())
    {
        printf("Key %s has no associated value\n", seqName.c_str());
        return false;
    }
    kSeq = it->second;
    return true;
    //return 
}

