#ifndef _AX12PACKET_H_
#define _AX12PACKET_H_

#include <string>
#include <iostream>

#define HEADER 0xFF

#define PING 0x01
#define READ_DATA 0x02
#define WRITE_DATA 0x03
#define REG_WRITE 0x04
#define ACTION 0x05
#define RESET 0x06
#define SYNC_WRITE 0x07

#define BROADCAST_ID 0xFE

#define GOAL_POSITION_L 0x1E // (Lower bits)
#define GOAL_POSITION_H 0x1F // (Lower bits)
#define MOVING_SPEED_L 0x20
#define MOVING_SPEED_H 0x21
#define TORQUE_LIMIT_L 0x22
#define TORQUE_LIMIT_H 0x23
#define PRESENT_POSITION_L 0x24
#define PRESENT_POSITION_H 0x25
#define PRESENT_SPEED_L 0x26
#define PRESENT_SPEED_H 0x27
#define PRESENT_LOAD_L 0x28
#define PRESENT_LOAD_H 0x29

using namespace std;

const unsigned char HEXCHARS[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

class AX12Packet {
 public:
  AX12Packet(int id)
  {
    id = 0;
    if(id <= 0xFF)
    {
      this->id = id;
    }
    else
    {
       cerr << "ERROR: id greater that 0xFF not allowed!" << endl;
    }

    packet = NULL;
    size = 0;
  }

	AX12Packet()
	{
		size = 0;
	}

  ~AX12Packet()
  {
    freePacket();
  }

  int getId()
  {
    return id;
  }

  int getInst()
  {
    return inst;
  }

	/**
	 * The size of packet. The length of packet. 
	 * Because C++ does not have length attribute as java does, 
	 * we need a separate method to obtain the size
	 */
	int getSize()
	{
		return size;
	}

  bool isEmpty()
  {
    return id == 0;
  }

  void getContent(int* c, int* length)
  {
    c = content; 
    *length = contentLength; 
  }

  unsigned char getCheckSum()
  {
    int len = contentLength + 2;
    int checksum = id + inst + len;
    for (int i = 0; i < contentLength; i++)
    {
      checksum += content[i];
    }
    return (unsigned char)(checksum & 0xFF);
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

  /**
   * Set the parameter part of the package
   * BE CAREFUL: The parameters have to be checked for correct range and 2 byte parameters have to be
   * sent as high- and low byte. The method assumes the parameter list to be correct!
   *
   * @param params Correct parameters for the instruction (usually address + parameter list)
   */
  void setParams(int *params, int pLength)
  {
    content = params;
    contentLength = pLength;
  }

  void setInst(int inst)
  {
    this->inst = inst;
  }

  
  /**
   * A convention in Java, as there is no universal C++ toString equivalent.
   */
  string toString()
  {
    unsigned char *pk;
    string s = "AX12Packet [ "; 
    build(pk);
    for (int i = 0; i < contentLength; i++)
    {
      s.append(charToStringHex(content[i]));
      s.append(" ");
    }
    s.append(string("]"));
    return s;
  }

 protected:
  int id;
  int inst;
  int *content;
  int contentLength;
  unsigned char* packet;


  void freePacket()
  {
    if (packet != NULL)
    {
      free(packet);
      packet = NULL;
    }
  }
  
protected:
int size;
 private:

  static string charToStringHex(int b)
  {
    string s;
    s.push_back(HEXCHARS[(b&0xF0)>>4]);
    s.push_back(HEXCHARS[b&0x0F]);

    return s;
  }
};

#endif
