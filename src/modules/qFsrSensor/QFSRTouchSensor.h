// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

/*
 * Copyright (C) 2007 Alex Bernardino, Carlos Beltran-Gonzalez
 * CopyPolicy: Released under the terms of the GNU GPL v2.0.
 *
 * Modified by Ze Ji 2009
 */

#ifndef __QFSRTOUCHSENSOR_H__
#define __QFSRTOUCHSENSOR_H__

#define COMMAND_TO_READ_FROM_SENSOR ' '

#include <yarp/dev/DeviceDriver.h>
#include <yarp/dev/IAnalogSensor.h>
#include <yarp/os/Bottle.h>

//#include <ftdi.h>
//#include <usb.h>

#include <ace/OS_NS_stdio.h>
#include <ace/Vector_T.h>

#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <string.h>
#include <iostream>
#include <ace/DEV_Connector.h>
#include <ace/TTY_IO.h>
#include <ace/OS_NS_stdio.h>
#include <yarp/os/Time.h>

#include <yarp/os/Semaphore.h>
#include "qextserialport.h"
namespace yarp
{
    namespace dev
    {
        class QFSRTouchSensor;
    }
}

/**
 * FtdiDeviceSettings contains information to identify specific device
 * Such a device can contain information:
 * such as: Manufacturer: FTDI, Description: FT232R USB UART, Serial A7003MhG
 */
 /*
class FtdiDeviceSettings
{
public:
    int vendor;                     //0x0403 normally. Can be found by lsusb on linux
    int product;                    //0x6001   Can be found by lsusb on linux
    char description[100];          // Contains the description
    char manufacturer[100];         // Contains manufacturer
    char serial[100];               // used mainly, as this seems unique
    int baudrate;                   // if ((retCode = ftdi_set_baudrate(&ftdic, config2.SerialParams.baudrate)) != 0)
    int readtimeout;
    // int bits;                    //    if (ftdi_set_line_property(&ftdic, BITS_8, STOP_BIT_1, NONE) == -1)
    // int parity;
    // int sbit;
    int flowctrol;                  // flow control to use. should be SIO_DISABLE_FLOW_CTRL, SIO_RTS_CTS_HS, SIO_DTR_DSR_HS or SIO_XON_XOFF_HS

    unsigned int write_chunksize;   // e.g. ftdi_write_data_set_chunksize(&ftdic, 3);
    unsigned int read_chunksize;    // ftdi_read_data_set_chunksize(&ftdic, 256);
};
*/
using namespace yarp::os;


/**
 * @ingroup dev_impl_media
 *
 * A basic Serial Communications Link (RS232, USB).
 *
 */
class yarp::dev::QFSRTouchSensor : public DeviceDriver, public IAnalogSensor
{
private:
    //  bool flag; // status flag
    int chan;
    int sizeOfReturn;

    char command[1];
    unsigned char *header;
    char *buffer;

    yarp::os::Semaphore mutex;
    QextSerialPort *port;

public:
    QFSRTouchSensor();

    virtual ~QFSRTouchSensor();
    virtual bool open(yarp::os::Searchable& config);
    virtual bool close(void);

    virtual int read(yarp::sig::Vector &out);
    virtual int getState(int ch);
    virtual int getChannels();
};

#endif
