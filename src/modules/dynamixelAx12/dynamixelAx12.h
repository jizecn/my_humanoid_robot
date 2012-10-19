#ifndef _DYNAMIXELAX12_WIN_H_
#define _DYNAMIXELAX12_WIN_H_

#define __DEBUG__

#include <yarp/dev/DeviceDriver.h>
#include <yarp/dev/ControlBoardInterfaces.h>

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
#include "qextserialport.h"
#include <yarp/os/Semaphore.h>


#include <QString>
#define MOTION_COMPLETION_TOLERANCE 3

/** defines for AX12 instruction codes ********************************/

#define INST_PING         0x01
#define INST_READ         0x02
#define INST_WRITE        0x03
#define INST_REGWRITE     0x04
#define INST_ACTION       0x05
#define INST_RESET        0x06
#define INST_SYNCWRITE    0x83

/** defines for AX12 general ids **************************************/
#define AX12_BROADCAST_ID 0xFE

/** defines for device paramters - EEPROM *****************************
100 added if paramter has length 2 */

#define CT_MODEL_NUMBER         00
#define CT_FIRMWARE_VERSION      2
#define CT_RETURN_DELAY_TIME     5
#define CT_CW_ANGLE_LIMIT       06
#define CT_CWW_ANGLE_LIMIT      08
#define CT_LIMIT_TEMPERATURE_H  11
#define CT_LIMIT_VOLTAGE_L      12
#define CT_LIMIT_VOLTAGE_H      13
#define CT_MAX_TORQUE           14
#define CT_STATUS_RETURN_LEVEL  16

/** defines for device paramters - RAM ********************************/

#define CT_TORQUE_ENABLE	24
#define CT_CW_COMP_MARGIN	26
#define CT_CWW_COMP_MARGIN	27
#define CT_CW_COMP_SLOPE	28
#define CT_CWW_COMP_SLOPE	29
#define CT_GOAL_POSITION	30
#define CT_MOVING_SPEED		32
#define CT_TORQUE_LIMIT		34
#define CT_PRESENT_POSITION	0x24
#define CT_PRESENT_SPEED	0x26
#define CT_PRESENT_LOAD		0x28
#define CT_PRESENT_VOLTAGE	42
#define CT_PRESENT_TEMPERATURE	43
#define CT_REG_INSTRUCTION	44
#define CT_MOVING		46
#define CT_LOCK			47
#define CT_PUNCH		48



enum ErrorCode {
	VOLTAGE_ERROR,
	ANGLE_ERROR,
	OVERHEATING_ERROR,
	RANGE_ERROR,
	CHECKSUM_ERROR,
	OVERLOAD_ERROR,
	INSTRUCTION_ERROR,
	OK
};



namespace yarp {
	namespace dev {
		class DynamixelAX12;
		//class FtdiDeviceSettings;
	}
}

using namespace yarp::os;
using namespace yarp::dev;


class yarp::dev::DynamixelAX12 : public DeviceDriver, public IPositionControl {

public:
	DynamixelAX12();
	~DynamixelAX12();

	/** Open device
	Opens and configures the device.

	@param config Config file containing string pairs for parameters
	@return true on success
	*/
	virtual bool open(yarp::os::Searchable& config);

	/** Close device
	Closes the device and shuts down connection.

	@return true on success
	*/
	virtual bool close(void);

	/** Configure device online
	Configures parts of the device that can be configures online.

	@param config Config file containing string pairs for parameters
	@return true on success
	*/
	virtual bool configure(yarp::os::Searchable& config);

	/** Send instruction to device
	Send an instruction to a device of given ID. The instruction has to be a byte arry containing
	the AX12 instruction code beginning with the instruction, the address and the parameters.
	Header and checksum are written automatically.

	@param id The hex id of the device to be contacted
	@param inst Byte array containing the instruction body (instruction, address, parameters)
	@return The content of the return packet of the device
	*/
	virtual int sendCommand(unsigned char id, unsigned char inst[], int size, unsigned char ret[], int &retSize);


	virtual ErrorCode checkAnswerPacket(unsigned char* packet, const char*& message);

	/** Read parameter from motor
	Requests the value of a parameter from motor.

	@param id The id of the device to be contacted
	@param param encodes address in control table and size of parameter (2 Bytes => address + 100, 1 byte => address)
	@return value if read successfull or -1
	*/
	virtual int readParameter(unsigned char id, unsigned char param);

	bool getAxes(int *ax);
	/*
	* The only supported mode, position control mode
	*/
	bool setPositionMode();
	/**
	* @param refs should be in range [1 300]
	*/
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
	
	bool setTorqueMode();
	bool getRefTorques(double *t);
	bool getRefTorque(int j, double *t);
	bool setTorques(const double *t);
	bool setTorque(int j, double t);
	bool setTorquePid(int j, const Pid &pid);
	bool getTorque(int j, double *t);
	bool getTorques(double *t);
	bool setTorquePids(const Pid *pids);
	bool setTorqueErrorLimit(int j, double limit);
	bool setTorqueErrorLimits(const double *limits);
	bool getTorqueError(int j, double *err);
	bool getTorqueErrors(double *errs);
	bool getTorquePidOutput(int j, double *out);
	bool getTorquePidOutputs(double *outs);
	bool getTorquePid(int j, Pid *pid);
	bool getTorquePids(Pid *pids);
	bool getTorqueErrorLimit(int j, double *limit);
	bool getTorqueErrorLimits(double *limits);
	bool resetTorquePid(int j);
	bool disableTorquePid(int j);
	bool enableTorquePid(int j);
	bool setTorqueOffset(int j, double v);


	bool resetEncoder(int j);
	bool resetEncoders();

	bool setEncoder(int j, double val);
	bool setEncoders(const double *vals);

	bool getEncoder(int j, double *v);
	bool getEncoders(double *encs);
	bool getEncoderSpeed(int j, double *sp);
	bool getEncoderSpeeds(double *spds);
	bool getEncoderAcceleration(int j, double *spds);
	bool getEncoderAccelerations(double *accs);
	
private:

    double *positions;
    double *speeds;
	    int *torques;
    int normalisePosition(double position);
    int normaliseSpeed(double speed);

    yarp::os::Semaphore mutex;
	 int numOfAxes; // default = 16
	     unsigned char *jointNumbers;
    /**
     * Initialise motor index.
     * @param sensorIndex a bottle containing a list of values of sensor indexes
     * for kaspar, can use sensorindex.cfg as default file
     */
    bool initMotorIndex(yarp::os::Bottle *sensorIndex);

    int syncSendCommand(unsigned char id, unsigned char inst[], int size, unsigned char ret[], int &retSize);

    QextSerialPort *port;
  //CSerial serial;
	  /** Switch set if device is open and ready */
  bool deviceOpen;	

};

#endif  // _DYNAMIXELAX12_WIN_H_