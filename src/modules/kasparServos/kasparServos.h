#ifndef _KASPAR_SERVOS_H_
#define _KASPAR_SERVOS_H_

#define __DEBUG__

#include <yarp/dev/DeviceDriver.h>
#include <yarp/dev/ControlBoardInterfaces.h>
#include <yarp/os/Semaphore.h>
#include <yarp/os/Time.h>

#include <ace/OS_NS_stdio.h>
//#include <ace/Vector_T.h>
//#include <ace/DEV_Connector.h>
//#include <ace/TTY_IO.h>
//#include <ace/OS_NS_stdio.h>

#include <stdio.h> 
#include <stdlib.h>
#include <cmath>
#include <string>
#include <iostream>

#include "qextserialport.h"

#include <vector>
#include <map>
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

#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b)) 
#define round(x) ((x)>=0?(int)((x)+0.5):(int)((x)-0.5))

namespace yarp {
	namespace dev {
		class KasparServos;
	}
}

using namespace yarp::os;
using namespace yarp::dev;


class yarp::dev::KasparServos : public DeviceDriver, public IPositionControl, public ITorqueControl, public IEncoders {

public:
	KasparServos();
	~KasparServos();

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

	virtual bool getAxes(int *ax);
	/*
	* The only supported mode, position control mode
	*/
	bool setPositionMode();
	/**
	* @param refs should be in range [1 300]
	*/
	virtual bool positionMove(int j, double ref);
	virtual bool positionMove(const double *refs);
	virtual bool relativeMove(int j, double delta);
	virtual bool relativeMove(const double *deltas);
	virtual bool checkMotionDone(int j, bool *flag);
	virtual bool checkMotionDone(bool *flag);
	virtual bool setRefSpeed(int j, double sp);
	virtual bool setRefSpeeds(const double *spds);
	virtual bool setRefAcceleration(int j, double acc);
	virtual bool setRefAccelerations(const double *accs);
	virtual bool getRefSpeed(int j, double *ref);
	virtual bool getRefSpeeds(double *spds);
	virtual bool getRefAcceleration(int j, double *acc);
	virtual bool getRefAccelerations(double *accs);
	virtual bool stop(int j);
	virtual bool stop();

	virtual bool setTorqueMode();
	virtual bool getRefTorques(double *t);
	virtual bool getRefTorque(int j, double *t);
	//	virtual bool setTorques(const double *t);
	//	virtual bool setTorque(int j, double t);
	virtual bool setTorquePid(int j, const Pid &pid);
	virtual bool getTorque(int j, double *t);
	virtual bool getTorques(double *t);
	virtual bool setTorquePids(const Pid *pids);
	virtual bool setTorqueErrorLimit(int j, double limit);
	virtual bool setTorqueErrorLimits(const double *limits);
	virtual bool getTorqueError(int j, double *err);
	virtual bool getTorqueErrors(double *errs);
	virtual bool getTorquePidOutput(int j, double *out);
	virtual bool getTorquePidOutputs(double *outs);
	virtual bool getTorquePid(int j, Pid *pid);
	virtual bool getTorquePids(Pid *pids);
	virtual bool getTorqueErrorLimit(int j, double *limit);
	virtual bool getTorqueErrorLimits(double *limits);
	virtual bool resetTorquePid(int j);
	virtual bool disableTorquePid(int j);
	virtual bool enableTorquePid(int j);
	virtual bool setTorqueOffset(int j, double v);
    virtual bool setRefTorques(const double *t);
    virtual bool setRefTorque(int j,double t);
    virtual bool getTorqueRange(int j,double *min,double *max);
    virtual bool getTorqueRanges(double *min,double *max);
    virtual bool resetEncoder(int j);
	virtual bool resetEncoders();

	virtual bool setEncoder(int j, double val);
	virtual bool setEncoders(const double *vals);

	virtual bool getEncoder(int j, double *v);
	virtual bool getEncoders(double *encs);
	virtual bool getEncoderSpeed(int j, double *sp);
	virtual bool getEncoderSpeeds(double *spds);
	virtual bool getEncoderAcceleration(int j, double *spds);
	virtual bool getEncoderAccelerations(double *accs);
	
private:
	enum ServoType{AX12, SSC32, MINISSC, POLOLU};
    
    double *positions;
    double *speeds;
	int *torques;
    int normalisePosition(double position);
    int normaliseSpeed(double speed);

    yarp::os::Semaphore mutex;
	int numOfAxes; // default = 16
    
    QextSerialPort *port;
	/** Switch set if device is open and ready */
	bool deviceOpen;	
	std::vector<QextSerialPort> ports;

	struct Servo {   
		unsigned char id; 
		double maxPos;
		double minPos;
		double defaultPos;
		double maxSpeed;
		double minSpeed;
		double defaultSpeed;

		yarp::dev::KasparServos::ServoType type;
		std::string serialport;
		QextSerialPort *port; 
	};

	bool initServos(yarp::os::Bottle sensorIndex);
	std::vector<Servo> servoList;
	
	bool checkIndexRange(int i);

	std::map<ServoType, QextSerialPort *> typePortMap;
	std::vector<QextSerialPort*> portList; 
	//QextSerialPort *portArray[];

    /**
     * Initialise motor index.
     * @param sensorIndex a bottle containing a list of values of sensor indexes
     * for kaspar, can use sensorindex.cfg as default file
     */
    // bool initMotorIndex(yarp::os::Bottle *sensorIndex);

    int syncSendCommand(unsigned char id, unsigned char inst[], int size, unsigned char ret[], int &retSize, ServoType t);
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
	virtual ErrorCode readParameter(unsigned char id, unsigned char param);

	bool initTypePortMap(Bottle serialports);
    void initData();
	bool getBaudrate(int _baudrate, BaudRateType &baudrate);
	bool initSerialPort(Bottle &b, QextSerialPort *sp);
};

#endif  // _DYNAMIXELAX12_WIN_H_
