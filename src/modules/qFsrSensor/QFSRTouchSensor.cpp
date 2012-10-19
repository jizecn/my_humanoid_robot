// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

/*
 * Ze Ji 2010
 */
#include "QFSRTouchSensor.h"

#include <yarp/sig/all.h>
#include <yarp/os/all.h>

using namespace yarp::os;
using namespace yarp::sig;
using namespace yarp::dev;

/**
 * @ingroup dev_impl_media
 *
 * A basic Serial Communications Link (RS232, USB).
 *
 */
QFSRTouchSensor::QFSRTouchSensor()
{
	port = new QextSerialPort(QextSerialPort::Polling);
    //command[0] = COMMAND_TO_READ_FROM_SENSOR;
    command[0] = 0x20;
	chan = 16;
    sizeOfReturn = chan*2;
	
    buffer = (char *)malloc(sizeof(char)*sizeOfReturn);
}

QFSRTouchSensor::~QFSRTouchSensor()
{
    //ftdi_list_free(&devlist);
    //ftdi_deinit(&ftdic);
    free(buffer);
	delete port;
}

bool QFSRTouchSensor::close(void)
{
    // First check if we have an open device
	if(port->isOpen())
	{
		// Yes, we have, so purge the buffers, reset the device and then close it
		port->flush();
		port->close();
	} else {
		// We don't seem to have a device open at this time
		ACE_OS::fprintf(stderr, "close():No device open to be be closed!\n");
		return false;
	}
    return true;
}

bool QFSRTouchSensor::open(yarp::os::Searchable& config)
{
	char comport[100];

	strcpy(comport, config.check("comport",Value("COM9"),"name of the serial channel").asString().c_str());
	int _baudrate = config.check("baudrate", Value(115200), "Specifies the baudrate at which the communication port operates.").asInt();

	BaudRateType baudrate; 

	/*
	BAUD50,                //POSIX ONLY
	BAUD75,                //POSIX ONLY
	BAUD110,
	BAUD134,               //POSIX ONLY
	BAUD150,               //POSIX ONLY
	BAUD200,               //POSIX ONLY
	BAUD300,
	BAUD600,
	BAUD1200,
	BAUD1800,              //POSIX ONLY
	BAUD2400,
	BAUD4800,
	BAUD9600,
	BAUD14400,             //WINDOWS ONLY
	BAUD19200,
	BAUD38400,
	BAUD56000,             //WINDOWS ONLY
	BAUD57600,
	BAUD76800,             //POSIX ONLY
	BAUD115200,
	BAUD128000,            //WINDOWS ONLY
	BAUD256000             //WINDOWS ONLY
	*/
	
	switch(_baudrate)
	{
	case 50:
		baudrate = BAUD50;
		break;
	case 75:
		baudrate = BAUD75;
		break;
	case 110:
		baudrate = BAUD110;
		break;
	case 134:
		baudrate = BAUD134;
		break;
	case 150:
		baudrate = BAUD150;
		break;
	case 300:
		baudrate = BAUD300;
		break;
	case 600:
		baudrate = BAUD600;
		break;
	case 1200:
		baudrate = BAUD1200;
		break;
	case 1800:
		baudrate = BAUD1800;
		break;
	case 2400:
		baudrate = BAUD2400;
		break;
	case 4800:
		baudrate = BAUD4800;
		break;
	case 9600:
		baudrate = BAUD9600;
		break;
	case 14400:
		baudrate = BAUD14400;
		break;
	case 19200:
		baudrate = BAUD19200;
		break;
	case 38400:
		baudrate = BAUD38400;
		break;
	case 56000:
		baudrate = BAUD56000;
		break;
	case 57600:
		baudrate = BAUD57600;
		break;
	case 76800:
		baudrate = BAUD76800;
		break;
	case 115200:
		baudrate = BAUD115200;
		break;
	case 128000:
		baudrate = BAUD128000;
		break;
	case 256000:
		baudrate = BAUD256000;
		break;
	default:
		return false;
	}

    chan = config.check("channel",Value(16),"Number of sensor channels. Normally should be 16 here").asInt();

    if (chan <= 0)
    {
        ACE_OS::fprintf(stderr, "channel should be positive\n");
        return false;
    }

    ACE_TRACE("QFSRTouchSensor::initialize");
    ACE_OS::printf("Opening QFSRTouchSensor Device\n");

	port->setPortName(QString(comport));
	port->setBaudRate(baudrate);
	port->setFlowControl(FLOW_OFF);
	port->setParity(PAR_NONE);
	port->setDataBits(DATA_8);
	port->setStopBits(STOP_1);
	//set timeouts to 500 ms
	port->setTimeout(1000);
	if(!port->open(QIODevice::ReadWrite| QIODevice::Unbuffered))
	{
		return false;
	}

    //int retCode;
/*
    char manufacturer[128], description[128];
    char serial[128];
    //int n = 0; // number of devices found
    //int k = 0;
    retCode = ftdi_usb_find_all(&ftdic, &devlist, ftdiSetting.vendor, ftdiSetting.product);
    switch (retCode)
    {
    case -1:
        ACE_OS::fprintf(stderr, "usb_find_busses() failed\n");
        return EXIT_FAILURE;
    case -2:
        ACE_OS::fprintf(stderr, "usb_find_devices() failed\n");
        return EXIT_FAILURE;
    case -3:
        ACE_OS::fprintf(stderr, "out of memory\n");
        return EXIT_FAILURE;
    case 0:
        ACE_OS::fprintf(stderr, "No device is found. Check connection.\n");
        return EXIT_FAILURE;
    default:
        ACE_OS::printf("%d devices have been found.\n", retCode);
        break;
    }
*/

    //deviceOpen = true; // Only set to be able to do close()
	//Time::delay(3); 

    return true;
}

int QFSRTouchSensor::read(yarp::sig::Vector &out)
{
    mutex.wait();
	if(!port->isOpen())
	{
		ACE_OS::printf("Port is not open, try again\n");
		return -1;
	}
	ACE_OS::printf("READ FROM SENSOR\n");
    int retCode = 0;
    int k = 0;
    //bool badAnswer = false;
    // FSRData f(16);
    // purge serial port buffer

    // request serial port to read data
    // Write data to device
	QByteArray byteArray(command, sizeof(command));
	//retCode = port->write(byteArray);
    //retCode = port->write(" ");
    //port->waitForBytesWritten(1000); 
	//ACE_OS::printf("WRITE TO SENSOR (command %c)(size %d) Return %d\n", command[0], sizeof(command), retCode);
	//retCode = ftdi_write_data(&ftdic, command, sizeof (command));
    do
    {
        // Now receive return packet
        //retCode stores the number of bytes read, which may not correspond to the number of bytes
        //one wants to read (the third parameter). This is a timing issue, so to learn how to fix
        //it, I should send a command and then continually printf the bytes that I get back. At
        //some point, I will get back the correct number of bytes. After doing this enough times,
		port->flush();
		retCode = port->write(byteArray);
		retCode = port->read(buffer, sizeOfReturn);		
		ACE_OS::printf("READ FROM SENSOR (%d)\n", retCode);

	}
    // retCode: <0: 	error code from usb_bulk_read()
    //           0: 	no data was available
    //          >0: 	number of bytes read
    while (retCode == 0); // not needed. as it will hang if no device is connected

    // read a line of data.

    // check if format is right.


	unsigned char b0;
	unsigned char b1;
    if (retCode < 0)
    {
        ACE_OS::fprintf(stderr, "Error code from usb_bulk_read() (%d)\n", retCode);
    }
    else if (retCode != sizeOfReturn)
    {
        ACE_OS::fprintf(stderr, "Returned wrong size of data (%d)\n", retCode);
        for (k = 0; k < retCode; k++)
        {
            printf("%d\n", buffer[k]);
        }
    }
    else
    {
        ACE_OS::printf("Returned %d bytes\n", retCode);
        for (k = 0; k < chan; k++)
        {
			b0 = buffer[k*2];
			b1 = buffer[k*2 + 1];
            printf("%d  %d\n", b0, b1);  // Here is wrong.... TODO
            //out[k] = ((double)(buffer[k*2]*0xFF + buffer[k*2 + 1]));
            out[k] = ((unsigned int)b0*0xFF + (unsigned int)b1);
			// printf("%f\n", out[k]);
        }
    }
    // if so, return 0

    // else, try again (5 times most)
    // otherwise return 1;
    mutex.post();
    return 0;
}

int QFSRTouchSensor::getState(int ch)
{
    // return the state of channel ch
    return AS_OK;
}

int QFSRTouchSensor::getChannels()
{
    return chan;
}

