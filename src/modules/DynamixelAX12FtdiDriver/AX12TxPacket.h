#ifndef _AX12TXPACKET_H_
#define  _AX12TXPACKET_H_

#include "AX12Packet.h"

class AX12TxPacket : public AX12Packet
{
 public:
 AX12TxPacket(int id, int inst):AX12Packet(id)
  {
    if(inst == PING || 
       inst == READ_DATA ||
       inst == WRITE_DATA ||
       inst == REG_WRITE ||
       inst == ACTION ||
       inst == RESET ||
       inst == SYNC_WRITE)
      this->inst = inst;
    else
      cerr << "ERROR: Instruction " << inst << " not known!" << endl;
  }

  void build(unsigned char *pk)
  {
    freePacket();
    size = 6 + contentLength;
    packet = (unsigned char *)malloc(sizeof(unsigned char)*(size));
    
    packet[0] = HEADER;
    packet[1] = HEADER;
    packet[2] = (unsigned char)id;
    packet[3] = (unsigned char)(contentLength + 2);
    packet[4] = (unsigned char)inst;


    for(int i = 0; i < contentLength; i++)
    {
      packet[5 + i] = (unsigned char)content[i];
    }
    packet[size - 1] = getCheckSum();
     
    pk = packet;
  }
};

#endif
