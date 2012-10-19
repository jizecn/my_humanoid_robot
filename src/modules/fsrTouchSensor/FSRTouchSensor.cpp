// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

/*
 * Ze Ji 2010
 */
#include "FSRTouchSensor.h"

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
FSRTouchSensor::FSRTouchSensor()
{
    command[0] = COMMAND_TO_READ_FROM_SENSOR;
    chan = 16;
    sizeOfReturn = chan*2;
}

FSRTouchSensor::~FSRTouchSensor()
{
    ftdi_list_free(&devlist);
    ftdi_deinit(&ftdic);
    free(buffer);
}

bool FSRTouchSensor::close(void)
{
    // First check if we have an open device
    if (deviceOpen)
    {
        // Yes, we have, so purge the buffers, reset the device and then close it
        ftdi_usb_purge_buffers(&ftdic);
        usb_reset(ftdic.usb_dev);
        ftdi_usb_close(&ftdic);
    }
    else
    {
        // We don't seem to have a device open at this time
        ACE_OS::fprintf(stderr, "close():No device open to be be closed!\n");
        return false;
    }
    return true;
}

bool FSRTouchSensor::open(yarp::os::Searchable& config)
{
    // ensure single thread to access serial port
    mutex.wait();

    ftdi_init(&ftdic);

    strcpy(ftdiSetting.description, config.check("FTDI_DESCRIPTION",Value("FT232R USB UART"),"Ftdi device description").asString().c_str());
    strcpy(ftdiSetting.serial, config.check("FTDI_SERIAL",Value("A900af5E"),"Ftdi device serial").asString().c_str());
    strcpy(ftdiSetting.manufacturer, config.check("FTDI_MANUFACTURER",Value("FTDI"),"Ftdi device manufacturer").asString().c_str());
    ftdiSetting.baudrate = config.check("baudrate",Value(115200),"Specifies the baudrate at which the communication port operates.").asInt();
    ftdiSetting.vendor = config.check("ftdivendor",Value(0x0403),"USB device vendor. 0x0403 normally. Can be found by lsusb on linux").asInt();
    ftdiSetting.product = config.check("ftdiproduct",Value(0x6001),"USB device product number. 0x6001 normally. Can be found by lsusb on linux").asInt();
    ftdiSetting.flowctrol = config.check("flowctrl", Value(SIO_DISABLE_FLOW_CTRL), "flow control to use. Should be SIO_DISABLE_FLOW_CTRL = 0x0, SIO_RTS_CTS_HS = 0x1 << 8, SIO_DTR_DSR_HS = 0x2 << 8, or SIO_XON_XOFF_HS = 0x4 << 8").asInt();
    ftdiSetting.write_chunksize = 3;
    ftdiSetting.read_chunksize = 256;

    chan = config.check("channel",Value(16),"Number of sensor channels. Normally should be 16 here").asInt();

    if (chan <= 0)
    {
        ACE_OS::fprintf(stderr, "channel should be positive\n");
        return false;
    }

    ACE_TRACE("FSRTouchSensor::initialize");
    ACE_OS::printf("Opening FSRTouchSensor Device\n");

    int retCode;

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

    int i = 0;

    for (curdev = devlist; curdev != NULL; i++)
    {
        ACE_OS::printf("Checking device: %d\n", i);

        if ((retCode = ftdi_usb_get_strings(&ftdic, curdev->dev, manufacturer, 128, description, 128, serial, 128)) < 0)
        {
            ACE_OS::fprintf(stderr, "ftdi_usb_get_strings failed: %d (%s)\n", retCode, ftdi_get_error_string(&ftdic));
        }
        ACE_OS::printf("Manufacturer: %s, Description: %s, Serial %s\n\n", manufacturer, description, serial);

        // check if the current device is the right one
        if (strcmp(serial, ftdiSetting.serial) == 0)
        {
            break;
        }
        curdev = curdev->next;
    }

    if (curdev == NULL)
    {
        ACE_OS::fprintf(stderr, "No FTDI (Arduino/FSRTouchSensor) device is found. Check connection.\n");
        return EXIT_FAILURE;
    }

    // open and reset everything in case we messed everything up with a control-c..
    if ((retCode = ftdi_usb_open_dev(&ftdic,  curdev->dev)) < 0)
    {
        ACE_OS::fprintf(stderr, "unable to open ftdi device: %d (%s)\n", retCode, ftdi_get_error_string(&ftdic));
        return EXIT_FAILURE;
    }
    deviceOpen = true; // Only set to be able to do close()

    this->close();

    // Now open again
    if ((retCode = ftdi_usb_open_dev(&ftdic, curdev->dev)) < 0)
    {
        ACE_OS::fprintf(stderr, "unable to open ftdi device: %d (%s)\n", retCode, ftdi_get_error_string(&ftdic));
        return EXIT_FAILURE;
    }

    // To test connection, read out FTDIChip-ID of R type chips
    if (ftdic.type == TYPE_R)
    {
        unsigned int chipid;
        if (ftdi_read_chipid(&ftdic, &chipid) == 0)
        {
            ACE_OS::printf("FTDI Device open. FTDI chipid: %X\n", chipid);
        }
        else
        {
            ACE_OS::fprintf(stderr, "Unable to access FTDI device!\n");
            return EXIT_FAILURE;
        }
    }
    deviceOpen = true;

    // We have an open device and it is accessible, so set parameters

    if ((retCode = ftdi_set_baudrate(&ftdic, ftdiSetting.baudrate)) != 0)
        ACE_OS::fprintf(stderr, "Error setting baudrate, ret=%d\n", retCode);
    if (ftdi_set_line_property(&ftdic, BITS_8, STOP_BIT_1, NONE) == -1)
        ACE_OS::fprintf(stderr, "Error setting connection properties");
    if (ftdi_setflowctrl(&ftdic, SIO_DISABLE_FLOW_CTRL) == -1)
        ACE_OS::fprintf(stderr, "Error setting flow control");

    // Set chunk sizes for in and out
    ftdi_write_data_set_chunksize(&ftdic, ftdiSetting.write_chunksize);
    ftdi_read_data_set_chunksize(&ftdic, ftdiSetting.read_chunksize);

    // length of data received from microcontroller
    sizeOfReturn = chan*2;

    buffer = (unsigned char *)malloc(sizeof(unsigned char)*sizeOfReturn);

    mutex.post();

    return true;
}

int FSRTouchSensor::read(yarp::sig::Vector &out)
{
    mutex.wait();
    int retCode = 0;
    int k = 0;
    //bool badAnswer = false;
    // FSRData f(16);
    // purge serial port buffer

    // request serial port to read data
    // Write data to device
    retCode = ftdi_write_data(&ftdic, command, sizeof (command));
    do
    {
        // Now receive return packet
        //retCode stores the number of bytes read, which may not correspond to the number of bytes
        //one wants to read (the third parameter). This is a timing issue, so to learn how to fix
        //it, I should send a command and then continually printf the bytes that I get back. At
        //some point, I will get back the correct number of bytes. After doing this enough times,
        //I should work out the timing issue
        retCode = ftdi_read_data(&ftdic, buffer, sizeOfReturn);
    }
    // retCode: <0,: 	error code from usb_bulk_read()
    //          0: 	no data was available
    //          >0: 	number of bytes read
    while (retCode == 0); // not needed. as it will hang if no device is connected

    // read a line of data.

    // check if format is right.

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
            // printf("%d\n", buffer[k]);
            out[k] = ((double)(buffer[k*2]*0xFF + buffer[k*2 + 1]));
            // printf("%f\n", out[k]);
        }
    }
    // if so, return 0

    // else, try again (5 times most)
    // otherwise return 1;
    return 0;
}

int FSRTouchSensor::getState(int ch)
{
    // return the state of channel ch
    return AS_OK;
}

int FSRTouchSensor::getChannels()
{
    return chan;
}

/*
bool FSRTouchSensor::getData(FSRData &data)
{
    data = flag ? buf1 : buf2;
    return true;
}

void FSRTouchSensor::run()
{

}

bool FSRTouchSensor::threadInit()
{
    return true;
}

void FSRTouchSensor::threadRelease()
{
}

void FSRTouchSensor::beforeStart()
{
}

void FSRTouchSensor::afterStart()
{
}
*/
