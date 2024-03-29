#include "dynamixelAx12.h"

#define BOOL_EXIT_FAILURE false

bool NOT_YET_IMPLEMENTED(const char *txt) {
	ACE_OS::fprintf(stderr, "%s not yet implemented for DynamixelAX12FtdiDriver\n", txt);
	return false;
}

DynamixelAX12::DynamixelAX12() {
	port = new QextSerialPort(QextSerialPort::Polling);
	//lock = false;     //by defauls, lock should be false
	deviceOpen = false;
	numOfAxes = 16;
	jointNumbers = (unsigned char *)malloc( 16*sizeof(unsigned char));
	// initialise
	jointNumbers[0]  = 0x65;
	jointNumbers[1]  = 0x74;
	jointNumbers[2]  = 0x84;
	jointNumbers[3]  = 0x66;
	jointNumbers[4]  = 0x75;
	jointNumbers[5]  = 0x85;
	jointNumbers[6]  = 0x67;
	jointNumbers[7]  = 0x76;
	jointNumbers[8]  = 0x86;
	jointNumbers[9]  = 0x6B;
	jointNumbers[10] = 0x77;
	jointNumbers[11] = 0x87;
	jointNumbers[12] = 0x6A;
	jointNumbers[13] = 0x68;
	jointNumbers[14] = 0x69;
	jointNumbers[15] = 0x6C;

	positions = (double *)malloc(numOfAxes*sizeof(double));
	speeds = (double *)malloc(numOfAxes*sizeof(double));
	torques = (int *)malloc(numOfAxes*sizeof(int));
}

DynamixelAX12::~DynamixelAX12() {
	free(jointNumbers);
	free(positions);
	free(speeds);
	free(torques);
	delete port;
}

bool DynamixelAX12::configure(yarp::os::Searchable &config)
{
	yarp::os::Value indexValue = config.find("SENSORINDEX");
	yarp::os::Bottle *indexBottle = indexValue.asList();
	this->initMotorIndex(indexBottle);
	return true;
}


bool DynamixelAX12::close() {

	// First check if we have an open device
	//if (deviceOpen) {
	if(port->isOpen())
	{
		// Yes, we have, so purge the buffers, reset the device and then close it
		port->flush();
		port->close();
		//ftdi_usb_purge_buffers(&ftdic);
		//usb_reset(ftdic.usb_dev);
		//ftdi_usb_close(&ftdic);
	} else {
		// We don't seem to have a device open at this time
		ACE_OS::fprintf(stderr, "close():No device open to be be closed!\n");
		return false;
	}
	return true;
}

/*
EBaudUnknown = -1,			// Unknown
EBaud110     = CBR_110,		// 110 bits/sec
EBaud300     = CBR_300,		// 300 bits/sec
EBaud600     = CBR_600,		// 600 bits/sec
EBaud1200    = CBR_1200,	// 1200 bits/sec
EBaud2400    = CBR_2400,	// 2400 bits/sec
EBaud4800    = CBR_4800,	// 4800 bits/sec
EBaud9600    = CBR_9600,	// 9600 bits/sec
EBaud14400   = CBR_14400,	// 14400 bits/sec
EBaud19200   = CBR_19200,	// 19200 bits/sec (default)
EBaud38400   = CBR_38400,	// 38400 bits/sec
EBaud56000   = CBR_56000,	// 56000 bits/sec
EBaud57600   = CBR_57600,	// 57600 bits/sec
EBaud115200  = CBR_115200,	// 115200 bits/sec
EBaud128000  = CBR_128000,	// 128000 bits/sec
EBaud256000  = CBR_256000,	// 256000 bits/sec
*/
bool DynamixelAX12::open(yarp::os::Searchable& config)
{
	char comport[100];

	strcpy(comport, config.check("comport",Value("COM10"),"name of the serial channel").asString().c_str());
	int _baudrate = config.check("baudrate", Value(57600), "Specifies the baudrate at which the communication port operates.").asInt();

	/*
	FtdiDeviceSettings ftdiSetting;
	strcpy(ftdiSetting.description, config.check("FTDI_DESCRIPTION", Value("FT232R USB UART"), "Ftdi device description").asString().c_str());
	strcpy(ftdiSetting.serial, config.check("FTDI_SERIAL", Value("A7003MhG"), "Ftdi device serial").asString().c_str());
	strcpy(ftdiSetting.manufacturer, config.check("FTDI_MANUFACTURER", Value("FTDI"), "Ftdi device manufacturer").asString().c_str());
	ftdiSetting.baudrate = config.check("baudrate", Value(57600), "Specifies the baudrate at which the communication port operates.").asInt();
	ftdiSetting.vendor = config.check("ftdivendor", Value(0x0403), "USB device vendor. 0x0403 normally. Can be found by lsusb on linux").asInt();
	ftdiSetting.product = config.check("ftdiproduct", Value(0x6001), "USB device product number. 0x6001 normally. Can be found by lsusb on linux").asInt();
	ftdiSetting.flowctrol = config.check("flowctrl", Value(SIO_DISABLE_FLOW_CTRL), "flow control to use. Should be SIO_DISABLE_FLOW_CTRL = 0x0, SIO_RTS_CTS_HS = 0x1 << 8, SIO_DTR_DSR_HS = 0x2 << 8, or SIO_XON_XOFF_HS = 0x4 << 8").asInt();
	ftdiSetting.write_chunksize = 3;
	ftdiSetting.read_chunksize = 256;
	*/

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

	ACE_TRACE("DynamixelAX12::initialize");
	ACE_OS::printf("Opening DynamixelAX12 Device\n");

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

	int retCode;

	if(port->isOpen())
	{
		ACE_OS::printf("OK. Port is open\n");
	}
	deviceOpen = true; // Only set to be able to do close()
	// Everything ready to rumble
	Time::delay(3); 
	return true;
}


int DynamixelAX12::syncSendCommand(unsigned char id, unsigned char inst[], int size, unsigned char ret[], int &retSize) {
	int r = 0;
	mutex.wait();
	//if (!ftdi_usb_purge_buffers(&ftdic)) {
	port->flush();
	r = sendCommand(id, inst, size, ret, retSize);
	//}

	mutex.post();
	return r;
}


int DynamixelAX12::sendCommand(unsigned char id, unsigned char inst[], int size, unsigned char ret[], int &retSize)
{
	int i;
	int retCode = 0;
	bool badAnswer = false;
	//unsigned char command[5 + size];
	unsigned char *command = (unsigned char *)malloc(sizeof(unsigned char)*(5+size));
	unsigned char chksum = 0x00;
	unsigned char header[4];
	unsigned char body[256];

	// Build packet header

	command[0] = 0xFF; // Header bytes
	command[1] = 0xFF;
	command[2] = id; // Id of device
	command[3] = size + 1; // Size of instruction plus checksum byte

	// Add instruction
	for (i = 0; i < size; i++) {
		command[4 + i] = inst[i];
		chksum += inst[i];
	}

	// Calculate checksum
	//command[sizeof (command) - 1] = ~(chksum + command[2] + command[3]);
	command[ 5 + size - 1] = ~(chksum + command[2] + command[3]);
#ifdef __DEBUG__
	printf("Packet sent: ");
	for (i = 0; i < 5 + size; i++) {
		printf("%X ", command[i]);
	}
	printf("\n");
#endif
	// Write data to device
	//retCode = ftdi_write_data(&ftdic, command, sizeof (command));
	//QByteArray byteArray((const char *)command, sizeof(command));
	QByteArray byteArray((const char *)command, 5 + size);
	port->write(byteArray);

	// Now receive return packet
	if (id != AX12_BROADCAST_ID) {
		do { /// Sven's original code. Need to test
			//retCode stores the number of bytes read, which may not correspond to the number of bytes
			//one wants to read (the third parameter). This is a timing issue, so to learn how to fix
			//it, I should send a command and then continually printf the bytes that I get back. At
			//some point, I will get back the correct number of bytes. After doing this enough times,
			//I should work out the timing issue
			//retCode = ftdi_read_data(&ftdic, header, 4);
			retCode = port->read((char *)header, 4);
		}/// Sven's original code. Need to test

		// retCode: <0,: 	error code from usb_bulk_read()
		//          0: 	no data was available
		//          >0: 	number of bytes read
		/// TODO check if it works with Kaspar. Originally made by Sven. Need testing
		while (retCode == 0); // not needed. as it will hang if no answer

		if (retCode < 0) {
			ACE_OS::fprintf(stderr, "Error while reading header of status packet!(%d)\n", retCode);
			return 0;
		} else if (retCode < 4) {
			ACE_OS::fprintf(stderr, "Error while reading header of status packet! (%d)\n", retCode);
			return 0;
		} else {
			retCode = 0;
			if (header[0] != 0xFF || header[1] != 0xFF) {
				badAnswer = true;
				printf("Received data without header bytes: ");
			} else if (header[2] != id) {
				badAnswer = true;
				printf("Received status from wrong device (expected %d): ", id);
			} else {
				// Now that we know a well formed packet is arriving, read remaining bytes
				do {
					retCode = port->read((char *)body, header[3]);
					//retCode = ftdi_read_data(&ftdic, body, header[3]);
				} while (retCode == 0);

				if (retCode != header[3]) {
					badAnswer = true;
					printf("Received data with wrong length: ");
				} else {
					//check checksum
					chksum = 0;
					for (i = 0; i < retCode - 1; i++) chksum += body[i];
					if (body[retCode - 1] != (unsigned char) ~(chksum + header[2] + header[3])) {
						badAnswer = true;
						printf("Received data with wrong checksum (%X != %X): ", body[retCode - 1], (unsigned char) ~(chksum + header[2] + header[3]));
					} else {
						// Packet ok, so return the instruction part without checksum
						for (i = 0; i < retCode - 1; i++) {
							ret[i] = body[i];
						}
						// retSize = retCode;     // Sven's original code. 
						retSize = retCode - 1; // retSize should be retCode - 1, as checksum is not included
						return 1;
					}
				}
			}
		}

		// Print packet if something was wrong
		if (badAnswer) {
			for (i = 0; i < 4; i++) {
				printf("%X ", header[i]);
			}
			for (i = 0; i < retCode; i++) {
				printf("%X ", body[i]);
			}
			printf("\n");
			return 0;
		}
	}

	free(command);
	return 1;
}



/***************************************************************************************************************
*/
ErrorCode DynamixelAX12::checkAnswerPacket(unsigned char* packet, const char*& message) {
	if (packet[0] & 1) {
		message = "Voltage out of operating voltage range!";
		return VOLTAGE_ERROR;
	}
	if (packet[0] & 2) {
		message = "Goal position outside angle limits!";
		return ANGLE_ERROR;
	}
	if (packet[0] & 4) {
		message = "Current temperature outside operating temperature!";
		return OVERHEATING_ERROR;
	}
	if (packet[0] & 8) {
		message = "Instruction out of defined range!";
		return RANGE_ERROR;
	}
	if (packet[0] & 16) {
		message = "Checksum of instruction package incorrect!";
		return CHECKSUM_ERROR;
	}
	if (packet[0] & 32) {
		message = "Specified torque can't control the applied load!";
		return OVERLOAD_ERROR;
	}
	if (packet[0] & 64) {
		message = "Undefined instruction or missing Reg_Write instrcution!";
		return INSTRUCTION_ERROR;
	}
	message = "";
	return OK;
}

int DynamixelAX12::readParameter(unsigned char id, unsigned char param)
{
	// Read instruction has 2 parameters: The starting address of the parameter and the number of bytes to read
	// Both are encoded in param (> 100 = 2 bytes)
	unsigned char packet[] = {INST_READ, 0, 1};

	unsigned char answerPacket[256];
	int answerSize;
	const char* errorMessage;
	ErrorCode errCode;

	if (param > 99) {
		// We have to read 2 bytes
		packet[1] = param - 100;
		packet[2] = 2;
	} else {
		// We have to read 1 bytes
		packet[1] = param;
	}

	// Send request
	//sendCommand(id, packet, 3, answerPacket, answerSize);
	syncSendCommand(id, packet, 3, answerPacket, answerSize);
	// Check for submitted error code
	if ((errCode = checkAnswerPacket(answerPacket, errorMessage)) != OK) {
		// We received an error code

	} else {
		// No error
		//return 1;
	}
	return errCode;
}


bool DynamixelAX12::getAxes(int *ax)
{
	*ax = numOfAxes;
	return true;
}


bool DynamixelAX12::setPositionMode()
{
	return true;
}


bool DynamixelAX12::positionMove(int j, double ref)
{
	double speed;
	int blankReturnSize = -1;
	unsigned char blankReturn[] = {0, 0, 0};
	getRefSpeed(j, &speed);
	const int instl = 5 + 1;
	//    unsigned char inst[instl] = {INST_WRITE, CT_GOAL_POSITION, (unsigned char) (normalisePosition(ref)&0xFF), (unsigned char) ((normalisePosition(ref) >> 8) & 0xFF), (unsigned char) (normaliseSpeed(speed)&0xFF), (unsigned char) ((normaliseSpeed(speed) >> 8)&0xFF00)};

	unsigned char inst[instl] = {INST_WRITE, CT_GOAL_POSITION, (unsigned char) (normalisePosition(ref)&0xFF), (unsigned char) ((normalisePosition(ref) >> 8) & 0xFF), (unsigned char) (normaliseSpeed(speed)&0xFF), (unsigned char) ((normaliseSpeed(speed) >> 8)&0xFF)};
	// unsigned char inst[instl] = {INST_WRITE, CT_GOAL_POSITION, 0x36, 0x02, 0x62, 0x00};

	// unsigned char inst[instl] = {WRITE_DATA, GOAL_POSITION_L, (unsigned char)0x90,  (unsigned char)0x1, (unsigned char)0x40,  (unsigned char)0x00};

	//  unsigned char cmd[] = {INST_READ, 0x24, 2};

	//  ret = sendCommand(0x74, cmd, 3, blankReturn, blankReturnSize);
	//printf("Vec: %d, sizeOfBlank: %d\n returnVector: ", ret.size(), blankReturnSize);
	//  for (numBytes = 0; numBytes < blankReturnSize-1; numBytes++)
	//  printf("%X ", blankReturn[numBytes]);
	//  printf("\n");
	positions[j] = ref;
	//return true;
	//return sendCommand(jointNumbers[j], inst, instl, blankReturn, blankReturnSize);
	return syncSendCommand(jointNumbers[j], inst, instl, blankReturn, blankReturnSize);
}

bool DynamixelAX12::positionMove(const double *refs)
{
	bool t = true;
	for (int k = 0; k < numOfAxes; k++) {
		if (!this->positionMove(k, refs[k]))
			t = false;
	}
	return t;
}

bool DynamixelAX12::relativeMove(int j, double delta)
{
	double v = positions[j];
	if (getEncoder(j, &v)) {
		return this->positionMove(j, v + delta);
	} else
		return false;
}

bool DynamixelAX12::relativeMove(const double *deltas)
{
	bool t = true;
	for (int k = 0; k < numOfAxes; k++) {
		if (!this->positionMove(k, positions[k] + deltas[k]))
			t = false;
	}
	return t;
}

bool DynamixelAX12::checkMotionDone(int j, bool *flag)
{
	double v = 0;
	bool t = true;

	t = getEncoder(j, &v);
	*flag = (std::fabs(v - positions[j]) < MOTION_COMPLETION_TOLERANCE);
	return t;
}

bool DynamixelAX12::checkMotionDone(bool *flag)
{
	bool t = true;
	for (int k = 0; k < numOfAxes; k++) {
		if (!this->checkMotionDone(k, &flag[k]))
			t = false;
	}
	return t;
}

bool DynamixelAX12::setRefSpeed(int j, double sp)
{
	if (sp < 1) {
		printf("Invalid speed value, should be from 1 to 114");
		speeds[j] = 1;
		return false;
	} else if (sp > 114) {
		printf("Invalid speed value, should be from 1 to 114");
		speeds[j] = 114;
		return false;
	} else {
		speeds[j] = sp;
		return true;
	}
}

bool DynamixelAX12::setRefSpeeds(const double *spds)
{
	bool t = true;
	for (int k = 0; k < numOfAxes; k++) {
		if (!setRefSpeed(k, spds[k]))
			t = false;
	}
	return true;
}

bool DynamixelAX12::setRefAcceleration(int j, double acc)
{
	return NOT_YET_IMPLEMENTED("setRefAcceleration");
}

bool DynamixelAX12::setRefAccelerations(const double *accs)
{
	return NOT_YET_IMPLEMENTED("setRefAccelerations");
}

bool DynamixelAX12::getRefSpeed(int j, double *ref)
{
	*ref = speeds[j];
	return true;
}

bool DynamixelAX12::getRefSpeeds(double *spds)
{
	for (int k = 0; k < numOfAxes; k++) {
		spds[k] = speeds[k];
	}
	return true;
}

bool DynamixelAX12::getRefAcceleration(int j, double *acc)
{
	return NOT_YET_IMPLEMENTED("getRefAcceleration");

}

bool DynamixelAX12::getRefAccelerations(double *accs)
{
	return NOT_YET_IMPLEMENTED("getRefAccelerations");
}

bool DynamixelAX12::stop(int j)
{
	return NOT_YET_IMPLEMENTED("stop");
}


bool DynamixelAX12::stop() {
	for (int i = 0; i < numOfAxes; i++) {
		if (!stop(i))
			return false;
	}
	return true;
}

bool DynamixelAX12::resetEncoder(int j) {
	return NOT_YET_IMPLEMENTED("resetEncoder");
}

bool DynamixelAX12::resetEncoders() {
	return NOT_YET_IMPLEMENTED("resetEncoders");
}

bool DynamixelAX12::setEncoder(int j, double val) {
	return NOT_YET_IMPLEMENTED("setEncoder");
}

bool DynamixelAX12::setEncoders(const double *vals) {
	return NOT_YET_IMPLEMENTED("setEncoders");
}

bool DynamixelAX12::getEncoder(int j, double *v) {
	*v = -1;     // invalid value for encoder of AX12 motor
	int ret = 1;

	int blankReturnSize = -1;
	unsigned char blankReturn[] = {0, 0, 0};

	const int instl = 3;
	unsigned char inst[instl] = {INST_READ, (unsigned char) CT_PRESENT_POSITION, (unsigned char) 2};
	//ret = sendCommand(jointNumbers[j], inst, instl, blankReturn, blankReturnSize);
	ret = syncSendCommand(jointNumbers[j], inst, instl, blankReturn, blankReturnSize);

#ifdef __DEBUG__
	printf("BLANKRETURN SIZE is ---- %d\n", blankReturnSize);
	int numBytes = 0;
	for (numBytes = 0; numBytes < blankReturnSize; numBytes++) {
		printf("%X ", blankReturn[numBytes]);
		printf("\n");
	}
#endif
	int pos = 0;
	if(ret == 1)
	{
		const char* message = "";
		if (checkAnswerPacket(blankReturn, message) == OK) {
			pos = ((int) blankReturn[2])*256 + blankReturn[1];
			*v = ((double) pos)*300.0 / 1024.0;
			return true;
		}
		else
		{
			ACE_OS::fprintf(stderr, message);
			return false;
		}

	}
	// pos = (blankReturn[1]&0b00000011)*0x100 + blankReturn[0];
	else
		return false;
}

bool DynamixelAX12::getEncoders(double *encs) {
	int k = 0;
	bool tt = true;
	double temp = 0;
	bool t = true;
	for (k = 0; k < numOfAxes; k++) {
		if (!getEncoder(k, &encs[k]))
		{
			tt = false;
		}
	}
	return tt;
}

bool DynamixelAX12::getEncoderSpeed(int j, double *sp) {
	bool ret;
	int numBytes;
	int blankReturnSize = -1;
	unsigned char blankReturn[] = {0, 0, 0};
	const int instl = 3;
	unsigned char inst[instl] = {INST_READ, (unsigned char) CT_PRESENT_SPEED, (unsigned char) 2};
	//ret = sendCommand(jointNumbers[j], inst, instl, blankReturn, blankReturnSize);
	ret = syncSendCommand(jointNumbers[j], inst, instl, blankReturn, blankReturnSize);
	/*
	for (numBytes = 0; numBytes < blankReturnSize - 1; numBytes++) {
	printf("%X ", blankReturn[numBytes]);
	printf("\n");
	}
	*/

	double speed = 0;
	const char* message = "";
	if (checkAnswerPacket(blankReturn, message) == OK)
	{
		//speed = (blankReturn[2]&0b00000011)*0xFF + blankReturn[1];
		speed = (blankReturn[2]&0X03)*0xFF + blankReturn[1];

		//speed *= (((blankReturn[1] >> 2)&0b000001) ? -1 : 1);
		speed *= (((blankReturn[1] >> 2)&0X01) ? -1 : 1);

		*sp = speed * 113 / 1024 + 1; /// TODO should be changed. not very accurate, though close
	}
	else
	{
		ACE_OS::fprintf(stderr, message);
		return false;
	}
	return ret;
}

bool DynamixelAX12::getEncoderSpeeds(double *spds) {
	int k = 0;
	bool tt = true;
	for (k = 0; k < numOfAxes; k++) {
		if (!getEncoderSpeed(k, &spds[k]))
			tt = false;
	}
	return tt;
} 

bool DynamixelAX12::getEncoderAcceleration(int j, double *spds) {
	return NOT_YET_IMPLEMENTED("getEncoderAcceleration");
}

bool DynamixelAX12::getEncoderAccelerations(double *accs) {
	return NOT_YET_IMPLEMENTED("getEncoderAccelerations");
}


bool DynamixelAX12::setTorqueMode()
{

	ACE_OS::fprintf(stderr, "Note: AX12 does not support torque control mode. This is only used to get torque feedback.\n");
	return NOT_YET_IMPLEMENTED("setTorqueMode");
}

bool DynamixelAX12::getRefTorques(double *t)
{
	for (int k = 0; k < numOfAxes; k++) {
		t[k] = torques[k];
	}
	ACE_OS::fprintf(stderr, "Note: AX12 does not support torque control mode. This is just the MAX tourque set for the ax12 servo.\n");
	return true;
}

bool DynamixelAX12::getRefTorque(int j, double *t)
{
	*t = torques[j];
	ACE_OS::fprintf(stderr, "Note: AX12 does not support torque control mode. This is just the MAX tourque set for the ax12 servo.\n");
	return true;
}

bool DynamixelAX12::setTorques(const double *t)
{
	bool tt = true;
	for (int k = 0; k < numOfAxes; k++) {
		if (!this->setTorque(k, t[k]))
			tt = false;
	}
	return tt;
}

bool DynamixelAX12::setTorque(int j, double t)
{
	if (t < 0) {
		ACE_OS::fprintf(stderr, "torque (%d) should in range [0 1023] or [0 0x3FF]. t is set to 0 here\n", t);
		t = 0;
	} else if (t > 1023) {
		ACE_OS::fprintf(stderr, "torque (%d) should in range [0 1023] or [0 0x3FF]. t is set to 1023 here\n", t);
		t = 1023;
	}

	int blankReturnSize = -1;
	unsigned char blankReturn[] = {0, 0, 0};
	const int instl = 3 + 1;

	unsigned char inst[instl] = {INST_WRITE, CT_MAX_TORQUE, (unsigned char) ((int) t & 0xFF), (unsigned char) (((int) t << 8) & 0x03)};
	//return sendCommand(jointNumbers[j], inst, instl, blankReturn, blankReturnSize);
	return syncSendCommand(jointNumbers[j], inst, instl, blankReturn, blankReturnSize);
	//    ACE_OS::fprintf(stderr, "Note: AX12 does not support torque control mode. This is only used to get torque feedback.\n");
	//return NOT_YET_IMPLEMENTED("setTorque");
}

bool DynamixelAX12::setTorquePid(int j, const Pid &pid)
{
	ACE_OS::fprintf(stderr, "Note: AX12 does not support torque control mode. This is only used to get torque feedback.\n");
	return NOT_YET_IMPLEMENTED("setTorquePid");
}

bool DynamixelAX12::getTorque(int j, double *t)
{
	bool ret;
	int numBytes;
	int blankReturnSize = -1;
	unsigned char blankReturn[] = {0, 0, 0};
	//getRefSpeed(j, &speed);
	//const int instl = 5 + 1;
	//unsigned char inst[instl] = {INST_WRITE, GOAL_POSITION_L, (unsigned char)(normalisePosition(ref)&0xFF), (unsigned char)((normalisePosition(ref) >> 8) & 0xFF),(unsigned char)(normaliseSpeed(speed)&0xFF), (unsigned char)((normaliseSpeed(speed) >> 8)&0xFF00)};
	//  unsigned char inst[instl] = {WRITE_DATA, GOAL_POSITION_L, (unsigned char)0x90,  (unsigned char)0x1, (unsigned char)0x40,  (unsigned char)0x00};

	//  unsigned char cmd[] = {INST_READ, 0x24, 2};

	const int instl = 3;
	unsigned char inst[instl] = {INST_READ, (unsigned char) CT_PRESENT_LOAD, (unsigned char) 2};
	//ret = sendCommand(jointNumbers[j], inst, instl, blankReturn, blankReturnSize);
	ret = syncSendCommand(jointNumbers[j], inst, instl, blankReturn, blankReturnSize);

	//  ret = sendCommand(0x74, cmd, 3, blankReturn, blankReturnSize);
	//printf("Vec: %d, sizeOfBlank: %d\n returnVector: ", ret.size(), blankReturnSize);
#ifdef __DEBUG__
	for (numBytes = 0; numBytes < blankReturnSize - 1; numBytes++) {
		printf("%X ", blankReturn[numBytes]);
		printf("\n");
	}
#endif
	const char* message = "";
	if (checkAnswerPacket(blankReturn, message) == OK) {
		//int load = (blankReturn[2]&0b00000011)*256 + blankReturn[1];
		int load = (blankReturn[2]&0X03)*256 + blankReturn[1];        
		//load *= (((blankReturn[2] >> 2)&0b000001) ? -1 : 1);
		load *= (((blankReturn[2] >> 2)&0X01) ? -1 : 1);
		*t = load;
	}
	else
	{
		ACE_OS::fprintf(stderr, message);
		return false;
	}
	return ret;

}

bool DynamixelAX12::getTorques(double *t)
{
	int k = 0;
	bool tt = true;
	for (k = 0; k < numOfAxes; k++) {
		if (!getTorque(k, &t[k]))
			tt = false;
	}
	return tt;
}

bool DynamixelAX12::setTorquePids(const Pid *pids)
{
	ACE_OS::fprintf(stderr, "Note: AX12 does not support torque control mode. This is only used to get torque feedback.\n");
	return NOT_YET_IMPLEMENTED("setTorquePids");
}

bool DynamixelAX12::setTorqueErrorLimit(int j, double limit)
{
	ACE_OS::fprintf(stderr, "Note: AX12 does not support torque control mode. This is only used to get torque feedback.\n");
	return NOT_YET_IMPLEMENTED("setTorqueErrorLimit");
}

bool DynamixelAX12::setTorqueErrorLimits(const double *limits)
{
	ACE_OS::fprintf(stderr, "Note: AX12 does not support torque control mode. This is only used to get torque feedback.\n");
	return NOT_YET_IMPLEMENTED("setTorqueErrorLimits");
}

bool DynamixelAX12::getTorqueError(int j, double *err)
{
	ACE_OS::fprintf(stderr, "Note: AX12 does not support torque control mode. This is only used to get torque feedback.\n");
	return NOT_YET_IMPLEMENTED("getTorqueError");
}

bool DynamixelAX12::getTorqueErrors(double *errs)
{
	ACE_OS::fprintf(stderr, "Note: AX12 does not support torque control mode. This is only used to get torque feedback.\n");
	return NOT_YET_IMPLEMENTED("getTorqueErrors");
}

bool DynamixelAX12::getTorquePidOutput(int j, double *out)
{
	ACE_OS::fprintf(stderr, "Note: AX12 does not support torque control mode. This is only used to get torque feedback.\n");
	return NOT_YET_IMPLEMENTED("getTorquePidOutput");
}

bool DynamixelAX12::getTorquePidOutputs(double *outs)
{
	ACE_OS::fprintf(stderr, "Note: AX12 does not support torque control mode. This is only used to get torque feedback.\n");
	return NOT_YET_IMPLEMENTED("getTorquePidOutputs");
}

bool DynamixelAX12::getTorquePid(int j, Pid *pid)
{
	ACE_OS::fprintf(stderr, "Note: AX12 does not support torque control mode. This is only used to get torque feedback.\n");
	return NOT_YET_IMPLEMENTED("getTorquePid");
}

bool DynamixelAX12::getTorquePids(Pid *pids)
{
	ACE_OS::fprintf(stderr, "Note: AX12 does not support torque control mode. This is only used to get torque feedback.\n");
	return NOT_YET_IMPLEMENTED("getTorquePids");
}

bool DynamixelAX12::getTorqueErrorLimit(int j, double *limit)
{

	ACE_OS::fprintf(stderr, "Note: AX12 does not support torque control mode. This is only used to get torque feedback.\n");
	return NOT_YET_IMPLEMENTED("getTorqueErrorLimit");
}

bool DynamixelAX12::getTorqueErrorLimits(double *limits)
{
	ACE_OS::fprintf(stderr, "Note: AX12 does not support torque control mode. This is only used to get torque feedback.\n");
	return NOT_YET_IMPLEMENTED("getTorqueErrorLimits");
}

bool DynamixelAX12::resetTorquePid(int j)
{

	ACE_OS::fprintf(stderr, "Note: AX12 does not support torque control mode. This is only used to get torque feedback.\n");
	return NOT_YET_IMPLEMENTED("resetTorquePid");
}

bool DynamixelAX12::disableTorquePid(int j)
{

	ACE_OS::fprintf(stderr, "Note: AX12 does not support torque control mode. This is only used to get torque feedback.\n");
	return NOT_YET_IMPLEMENTED("disableTorquePid");
}

bool DynamixelAX12::enableTorquePid(int j)
{

	ACE_OS::fprintf(stderr, "Note: AX12 does not support torque control mode. This is only used to get torque feedback.\n");
	return NOT_YET_IMPLEMENTED("enableTorquePid");
}

bool DynamixelAX12::setTorqueOffset(int j, double v)
{
	ACE_OS::fprintf(stderr, "Note: AX12 does not support torque control mode. This is only used to get torque feedback.\n");
	return NOT_YET_IMPLEMENTED("setTorqueOffset");
}

bool DynamixelAX12::initMotorIndex(yarp::os::Bottle *sensorIndex) {
	int s = sensorIndex->size() - 1;
	if (s != sensorIndex->get(0).asInt()) {
		ACE_OS::fprintf(stderr, "sensor number does not match the real number in configuration file\n");
		return false;
	}
	numOfAxes = s;
	unsigned char *tmp = NULL;
	tmp = (unsigned char *) realloc(jointNumbers, numOfAxes * sizeof (unsigned char));
	if (tmp != NULL) {
		//	jointNumbers = {0x65, 0x74, 0x84, 0x66, 0x75, 0x85, 0x67, 0x76, 0x86, 0x6B, 0x77, 0x87, 0x6A, 0x68, 0x69, 0x6C};
		jointNumbers = tmp;
	} else {
		return false;
	}
	double *tmpp = NULL;
	tmpp = (double *) realloc(positions, numOfAxes * sizeof (double));
	if (tmpp != NULL) {
		positions = tmpp;
	} else {
		return false;
	}

	double *tmps = NULL;
	tmps = (double *) realloc(speeds, numOfAxes * sizeof (double));
	if (tmps != NULL) {
		speeds = tmps;
	} else {
		return false;
	}

	int *tmpt = NULL;
	tmpt = (int *) realloc(torques, numOfAxes * sizeof (int));
	if (tmpt != NULL) {
		torques = tmpt;
	} else {
		return false;
	}

	for (int i = 0; i < numOfAxes; i++) {
		jointNumbers[i] = (unsigned char) (sensorIndex->get(i + 1).asInt());
	}
	return true;
}


int DynamixelAX12::normalisePosition(double position) {
	if (position < 0) {
		position = 0;
		printf("Invalid position value, should be from 0 to 300");
	} else if (position > 300) {
		position = 300;
		printf("Invalid position value, should be from 0 to 300");
	}
	return (int) (1023 * position / 300);
	//return (int) (1024 * position / 301);
}

int DynamixelAX12::normaliseSpeed(double speed) {
	// speed -> [1 114] RPM
	// when speed is 0, it is maximum possible speed, no speed control. so it is not useful here

	if (speed < 1) {
		printf("Invalid speed value, should be from 1 to 114 (%f)", speed);
		speed = 1;
		return speed;
	} else if (speed > 114) {
		printf("Invalid speed value, should be from 1 to 114");
		speed = 114;
	}
	return (int) (1024 * speed / 114 - 1);
/*
	int temp = (int) (1023 * speed / 114);
	if (temp < 1) {
		printf("Invalid speed value, should be from 1 to 114 (%d)", temp);
		//speed = 1;
		return 1;
	} else if (temp > 114) {
		printf("Invalid speed value, should be from 1 to 114 (%d)", temp);
		//speed = 114;
		return 114;
	}
	return temp;*/
}
