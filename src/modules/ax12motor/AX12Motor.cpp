// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

/*
 * Copyright (C) 2010 Ze Ji
 * CopyPolicy: Released under the terms of the GNU GPL v2.0.
 *
 */

#include <yarp/dev/ControlBoardInterfaces.h>
#include <yarp/dev/Drivers.h>
#include <yarp/dev/PolyDriver.h>

#include <yarp/dev/SerialInterfaces.h>

#include <stdio.h>
#include <stdlib.h>

#include "AX12Motor.h"
#include "AX12Connection.h"

//TODO: check limits of operation (range of angles)?

using namespace yarp::os;
using namespace yarp::dev;


bool AX12Motor::getAxes(int *ax) {
	*ax = numOfAxes;
    return true;
}


bool AX12Motor::setPositionMode() {
	// does AX-12 support different modes?
    return true;
}


bool AX12Motor::positionMove(int j, double ref) {

    positions[j]=ref;
    AX12TxPacket send(ids[j], WRITE_DATA);
	size_t size;
    double speed;
    getRefSpeed(j, &speed);
    const int instl = 5;
    int inst[instl] = {GOAL_POSITION_L, (unsigned char)(normalisePosition(ref)&0xFF),  (unsigned char)(normalisePosition(ref)&0xFF00 >> 8),(unsigned char)(normaliseSpeed(speed)&0xFF), (unsigned char)(normaliseSpeed(speed)&0xFF00 >> 8)};
    send.setParams(inst, instl);
    
    size = 5 + 5 + 1;

    sendAndWait(send);

    return true;
    //    return this->move(j, ref, positions, speeds, serial);
}


bool AX12Motor::positionMove(const double *refs) {
    for(int k=0; k<numOfAxes; k++) {
        this->positionMove(k, refs[k]);
    }
    return true;
}


bool AX12Motor::relativeMove(int j, double delta) {
    this->positionMove(j, positions[j]+delta);

    return true;
}


bool AX12Motor::relativeMove(const double *deltas) {
    for(int k=0; k<numOfAxes; k++) {
        this->positionMove(k, positions[k]+deltas[k]);
    }

    return true;
}


bool AX12Motor::checkMotionDone(int j, bool *flag) {
    //TODO: Q?

    return true;
}


bool AX12Motor::checkMotionDone(bool *flag) {
    //TODO: Q?

    return true;
}


bool AX12Motor::setRefSpeed(int j, double sp) {
    speeds[j]=sp;
    return true;
}


bool AX12Motor::setRefSpeeds(const double *spds) {
    for(int k=0; k<numOfAxes; k++) {
        setRefSpeed(k, spds[k]);
    }
    return true;
}


bool AX12Motor::setRefAcceleration(int j, double acc) {
    return true;
}


bool AX12Motor::setRefAccelerations(const double *accs) {
    return true;
}


bool AX12Motor::getRefSpeed(int j, double *ref) {
    *ref=speeds[j];
    return true;
}


bool AX12Motor::getRefSpeeds(double *spds) {
    for(int k=0; k<numOfAxes; k++) {
        spds[k]=speeds[k];
    }
    return true;
}


bool AX12Motor::getRefAcceleration(int j, double *acc) {
    return true;
}


bool AX12Motor::getRefAccelerations(double *accs) {
    return true;
}


bool AX12Motor::stop(int j) {

    return true;
}


bool AX12Motor::stop() {
	for (int i = 0; i < numOfAxes; i++)
	{
		if (!stop(i))
			return false;
	}
    return true;
}

int AX12Motor::normalisePosition(double position)
{
	if(position < 0)
	{
		position = 0;
		printf("Invalid position value, should be from 0 to 300");
	}
	else if(position > 300)
	{
		position = 300;
		printf("Invalid position value, should be from 0 to 300");
	}

	return (int)(1024*position/301);
}

int AX12Motor::normaliseSpeed(double speed)
{
	// speed -> [1 114] RPM
	// when speed is 0, it is maximum possible speed, no speed control. so it is not useful here
	if(speed < 1)
	{
		printf("Invalid speed value, should be from 1 to 114");
		speed = 1;
	}
	else if(speed > 114)
	{
		printf("Invalid speed value, should be from 1 to 114");
		speed = 114;
	}
	return (int)(1024*speed/114 - 1);
}

AX12RxPacket AX12Motor::sendAndWait(AX12TxPacket packet)
{
	unsigned char *cmd; 
    packet.build(cmd);
    serial->send((char *)cmd, packet.getSize());
    
    int id = packet.getId();
    int count = 0;
    
    while(!hasPacketWaiting(id) && count < 10)
    {
			usleep(10000); //sleep for 10ms

	}
    
    AX12RxPacket ret; 
    
	return NULL;
}

bool AX12Motor::hasPacketWaiting(int id)
{
	
}

void AX12Motor::broadcastActionCommand()
{
	AX12TxPacket packet(BROADCAST_ID, ACTION);
	unsigned char *cmd; 
    packet.build(cmd);
	int inst[0];
	packet.setParams(inst, 0);
	serial->send((char *)cmd, packet.getSize());
}

/*
bool AX12Motor::move(unsigned char id, double ref, double position, double speed, ISerialDevice *serial)
{
	size_t size = 10;
	char cmd[size];
	cmd[0] = 0xFF;
	cmd[1] = 0xFF;
	cmd[2] = id;

	int length = 2;
	cmd[3] = (unsigned char)length;
	cmd[4] = WRITE_DATA;
	cmd[5] = 0x1E;
	cmd[6] = (unsigned char)(normalisePosition(position)&0xFF);
	cmd[7] = (unsigned char)(normalisePosition(position)&0xFF00 >> 8);
    cmd[8] = (unsigned char)(normaliseSpeed(speed)&0xFF);
    cmd[9] = (unsigned char)(normaliseSpeed(speed)&0xFF00 >> 8);
    cmd[10] = (unsigned char)1;  //checksum
	serial->send(cmd, size);
	return true;
}
*/
