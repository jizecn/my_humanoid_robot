//      AX12RxPacket.h
//      
//      Copyright 2010 Ze Ji <z.ji@herts.ac.uk>
//      
//      This program is free software; you can redistribute it and/or modify
//      it under the terms of the GNU General Public License as published by
//      the Free Software Foundation; either version 2 of the License, or
//      (at your option) any later version.
//      
//      This program is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//      GNU General Public License for more details.
//      
//      You should have received a copy of the GNU General Public License
//      along with this program; if not, write to the Free Software
//      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
//      MA 02110-1301, USA.


#ifndef _AX12RXPACKET_H_
#define _AX12RXPACKET_H_

#include <iostream>
#include "AX12Packet.h"

class AX12RxPacket : public AX12Packet
{
	public:
		AX12RxPacket(int *data):AX12Packet(data[2])
		{
			error = "";
			this->id = data[2];
			//content = ;
		}
		
		AX12RxPacket():AX12Packet()
		{
		}
		
		~AX12RxPacket()
		{
		}

		string toString()
		{
			string s = "";
			return s;
		}

		bool isValid()
		{
			return true;
		}

		int *getContent()
		{
			return content;
		}
		
	private:
		string error;
		static string getAX12ErrorMessage(int error)
		{
			switch(error){
				case 0: return "Ok!";
				case 1: return "Input Voltage Error!";
				case 2: return "Angle Limit Error!";
				case 4: return "Overheating Error!";
				case 8: return "Range Error!";
				case 16: return "Checksum Error!";
				case 32: return "Overload Error!";
				case 64: return "Instruction Error!";
				default: return "Unknown Error!"; 
			}
		}
};

#endif
