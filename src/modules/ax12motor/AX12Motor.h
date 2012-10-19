// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

/*
 * Copyright (C) 2010 Ze Ji
 * CopyPolicy: Released under the terms of the GNU GPL v2.0.
 */

#ifndef _AX12MOTOR_H_
#define _AX12MOTOR_H_

#include <yarp/dev/ControlBoardInterfaces.h>
#include <yarp/dev/Drivers.h>
#include <yarp/dev/PolyDriver.h>

#include <yarp/dev/SerialInterfaces.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "AX12TxPacket.h"
#include "AX12RxPacket.h"

#define round(x) ((x)>=0?(int)((x)+0.5):(int)((x)-0.5))
#define FABS(x) (x>=0?x:-x)

using namespace yarp::os;
using namespace yarp::dev;
const int numOfAxes = 16;

const int ids [numOfAxes] = { 101,102,103,104,105,106,107,108,109,110,111,112,113, 114, 115, 116 };

class AX12Motor : public DeviceDriver, public IPositionControl {
public:
    //    int servoboard;
    ISerialDevice *serial;
    PolyDriver dd;
    //    bool move(unsigned char id, double ref, double position, double speed, ISerialDevice *serial);
    bool getAxes(int *ax);
    bool setPositionMode();
    bool positionMove(int j, double ref);
    bool positionMove(const double *refs);
    bool relativeMove(int j, double delta);
    bool relativeMove(const double *deltas);
    bool checkMotionDone(int j, bool *flag);
    bool checkMotionDone(bool *flag);
    bool setRefSpeed(int j, double sp);
    bool setRefSpeeds(const double *spds);
    bool setRefAcceleration(int j, double acc);
    bool setRefAccelerations(const double *accs);
    bool getRefSpeed(int j, double *ref);
    bool getRefSpeeds(double *spds);
    bool getRefAcceleration(int j, double *acc);
    bool getRefAccelerations(double *accs);
    bool stop(int j);
    bool stop();
	AX12RxPacket sendAndWait(AX12TxPacket packet);
	void broadcastActionCommand();

    virtual bool open(Searchable& config) {

        if(config.check("help")==true) {
          printf("SerialServoBoard Available Options:\n");
          printf(" -board NAME, where name is should be ax12 here\n");
          printf(" -comport NAME, where name is COMx on Windows, and /dev/ttySx on Linux\n");
          printf(" -baudrate RATE, where RATE is the Board baudrate\n");
          printf("\n -help shows this help\n");
          return false;
        }

        char servoboard_[80];
        strcpy(servoboard_, config.check("board", yarp::os::Value("ax12")).asString().c_str());

        char comport[80];

        strcpy(comport, config.check("comport", yarp::os::Value("/dev/ttyS0")).asString().c_str());

        int baudrate = config.check("baudrate", yarp::os::Value(57600)).asInt();
        
        Property conf;
        // no arguments, use a default
        char str[160];

        sprintf(str, "(device serialport) (comport %s) (baudrate %d) (rcvenb 1) (stopbits 2) (databits 8) (paritymode none)", comport, baudrate);

        conf.fromString(str);

        dd.open(conf);
        if (!dd.isValid()) {
            printf("Failed to create and configure a device\n");
            exit(1);
        }

        if (!dd.view(serial)) {
            printf("Failed to view device through IGPUDevice interface\n");
            exit(1);
        }

        return true;
    }

    virtual bool close() {
        dd.close();
        return true;
    }

	bool hasPacketWaiting(int id);

private:
    double positions[numOfAxes];
    double speeds[numOfAxes];
    int normalisePosition(double position);
    int normaliseSpeed(double speed);

};

#endif
