#ifndef _AX12SYNCPACKET_H_
#define  _AX12SYNCPACKET_H_

#include <string>
#include "AX12TxPacket.h"

using namespace std;

class AX12SyncPacket : public AX12TxPacket
{
 public:
 AX12SyncPacket(string address, int length):AX12TxPacket(BROADCAST_ID, SYNC_WRITE)
  {
    this->address = address;
    this->length = length;    
  }

  void build(unsigned char *pk)
  {
    /*
    freePacket();
    int size = 6 + contentLength;
    packet = (unsigned char *)malloc(sizeof(unsigned char)*(size));
    
    packet[0] = HEADER;
    packet[1] = HEADER;
    packet[2] = (unsigned char)id;
    packet[3] = (unsigned char)(contentLength + 2);
    packet[4] = (unsigned char)inst;
    packet[size - 1] = getCheckSum();

    for(int i = 0; i < contentLength; i++)
    {
      packet[5 + i] = (unsigned char)content[i];
    }

    pk = packet;
    */
  }

 private:
  string address;
  int length;
};

#endif
