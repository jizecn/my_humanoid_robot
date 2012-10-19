#include "kasparServos.h"

#define BOOL_EXIT_FAILURE false

bool NOT_YET_IMPLEMENTED(const char *txt) {
	ACE_OS::fprintf(stderr, "%s not yet implemented for KasparServos\n", txt);
	return false;
}

KasparServos::KasparServos() {
	//port = new QextSerialPort(QextSerialPort::Polling);
	deviceOpen = false;
	numOfAxes = 16;

    unsigned char defaultIds[16] = {0x65, 0x74,0x84,0x66, 0x75, 0x85, 0x67, 0x76, 0x86, 0x6B, 0x77, 0x87, 0x6A, 0x68, 0x69, 0x6C};
    
    for(int i = 0; i < 16; i++)
    {
        Servo s;
        s.id = defaultIds[i];
        s.type = this->AX12;
        servoList.push_back(s);
    }
    
	positions = (double *)malloc(numOfAxes*sizeof(double));
	speeds = (double *)malloc(numOfAxes*sizeof(double));
	torques = (int *)malloc(numOfAxes*sizeof(int));
	initData();
}

KasparServos::~KasparServos() {
    if(positions)
	    free(positions);
    if(speeds)
    	free(speeds);
	if(torques)
		free(torques);
	for(int i = 0; i < portList.size(); i++)
	{
		if(portList[i])
			delete portList[i];
	}
}


bool KasparServos::configure(yarp::os::Searchable &config)
{
    yarp::os::Bottle servosSetting = config.findGroup("servos", "All servos for kaspar available").tail();
	bool ret;
    ret = initServos(servosSetting);
    if(ret)
	{
		initData();
	}
	return ret;
}

bool KasparServos::initServos(yarp::os::Bottle servosBottle)
{
    servoList.clear();
    
    int tempIndex = 0;//indexBottle.size();
    numOfAxes = servosBottle.size();

    //servos = (Servo *)malloc(sizeof(Servo)*numOfAxes);
    int count = 0;
    if(numOfAxes == 0)
    {
        printf("NumOfAxes is 0. Check the config file.\n");
        return false;
    }
    for(int i = 0; i < numOfAxes; i++)
    {
        Servo sv;
        yarp::os::Bottle servoConf(servosBottle.get(i).toString());
        tempIndex = servoConf.get(0).asInt();
        if(tempIndex != i)  // a strict condition
        //if(tempIndex >= numOfAxes)
        {
            printf("Wrong format of servo configuration. servo index is not allowed to be higher than numOfAxes\n");
            return false;
        }

        //sv.serialport = servoConf.get(1).asString().c_str();
        
        yarp::os::ConstString tmptype = servoConf.get(1).asString();
 
        if(tmptype == "AX12"){
            sv.type = this->AX12;
            sv.maxPos = 300;
            sv.minPos = 0;
            sv.maxSpeed = 114;
            sv.minSpeed = 1;
        }
        else if(tmptype == "SSC32"){
            sv.type = this->SSC32;
            sv.maxPos = 180;
            sv.minPos = 0;
        }
        else if(tmptype == "MINISSC"){
            sv.type = this->MINISSC;
            sv.maxPos = 180;
            sv.minPos = 0;
        }
        else if(tmptype == "POLOLU"){
            sv.type = this->POLOLU;
            sv.maxPos = 180;
            sv.minPos = 0;
        }
        else
        {   
            printf("Wrong type of servo (%s) (%d)\n", tmptype.c_str(), i);
            return false;
        }
        sv.id = (unsigned char)(servoConf.get(2).asInt());
    
        //////  TO CONTINUE........
        servoList.push_back(sv);
        printf("servo (%d) (%d) (%d)\n", servoList[i].type, servoList[i].id, i);
    }

    double *tmpp = NULL;
	tmpp = (double *) realloc(positions, numOfAxes * sizeof (double));
	if (tmpp) {
		positions = tmpp;
	} else {
		return false;
	}

	double *tmps = NULL;
	tmps = (double *) realloc(speeds, numOfAxes * sizeof (double));
	if (tmps) {
		speeds = tmps;
	} else {
		return false;
	}

	int *tmpt = NULL;
	tmpt = (int *) realloc(torques, numOfAxes * sizeof (int));
	if (tmpt) {
		torques = tmpt;
	} else {
		return false;
	}

    return true;
}

bool KasparServos::close() {

	// First check if we have an open device
	for (int i = 0; i < this->portList.size(); i++)
	{
		//if(port->isOpen())
		if(this->portList[i] != NULL)
		{
			if(this->portList[i]->isOpen())
			{
				// Yes, we have, so purge the buffers, reset the device and then close it
				this->portList[i]->flush();
				this->portList[i]->close();
			} 
			else {
				// We don't seem to have a device open at this time
				ACE_OS::fprintf(stderr, "close():No device open to be be closed!\n");
				//return false;
			}
		}
	}
	return true;
}

bool KasparServos::open(yarp::os::Searchable& config)
{
    //new
    yarp::os::Bottle servosSetting = config.findGroup("servos", "All servos for kaspar available");
    //tail();
    if(!servosSetting.isNull())
    {
        if(!initServos(servosSetting.tail()))
        {
            printf("Error initialising servos\n");
            return false;
        }
    }
    else{//continue with default setting
        printf("Null\n");
        return false;
    }
    printf("OK initilising servos\n");
/*
    char comport[100];

	strcpy(comport, config.check("comport",Value("COM11"),"name of the serial channel").asString().c_str());
	int _baudrate = config.check("baudrate", Value(115200), "Specifies the baudrate at which the communication port operates.").asInt();

	BaudRateType baudrate = BAUD115200;
	if(! getBaudrate(_baudrate, baudrate))
	{
		printf("ERROR in setting baudrate\n");
		return false;
	}

	ACE_TRACE("KasparServos::initialize");
	ACE_OS::printf("Opening KasparServos Device (%s) (%d)\n", comport, _baudrate);

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
	Time::delay(1); 

*/

	// NEW CONFIG
    yarp::os::Bottle serialports = config.findGroup("SERIALPORTS", "All serial ports for kaspar available");
	this->initTypePortMap(serialports.tail());
	
	return true;
}


int KasparServos::syncSendCommand(unsigned char id, unsigned char inst[], int size, unsigned char ret[], int &retSize, const ServoType type) {
	int r = 0;
	mutex.wait();
	this->port = this->typePortMap[type];
	if(this->port == NULL)
		return 0;
	port->flush();

    if(type == this->AX12)
    {
        if(id == AX12_BROADCAST_ID)
        {
            QByteArray byteArray((const char *)inst, size);
            port->write(byteArray);
	    	r = 1;
			printf("Broadcast\n");
			
			mutex.post();
			return 1;
        }
        else
        {
            r = sendCommand(id, inst, size, ret, retSize);
        }
    }
    else if(type == this->MINISSC || type == this->POLOLU)
    {
        QByteArray byteArray((const char *)inst, size);
#ifdef _DEBUG
        printf("%x %x %x\n", inst[0], inst[1], inst[2]);
#endif
        port->write(byteArray);
		r = 1;
    }
    else if(type == this->SSC32)
    {
        printf("SSC32 mode is not implemented yet");
        r = 0;
    }

	mutex.post();
	return r;
}


int KasparServos::sendCommand(unsigned char id, unsigned char inst[], int size, unsigned char ret[], int &retSize)
{
	int count = 0; 
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
#ifdef _DEBUG
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
			count ++;
			
		}/// Sven's original code. Need to test

		// retCode: <0,: 	error code from usb_bulk_read()
		//          0: 	no data was available
		//          >0: 	number of bytes read
		/// TODO check if it works with Kaspar. Originally made by Sven. Need testing
		while (retCode == 0 && count < 2); // not needed. as it will hang if no answer

		if (retCode <= 0) {
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
ErrorCode KasparServos::checkAnswerPacket(unsigned char* packet, const char*& message) {
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

ErrorCode KasparServos::readParameter(unsigned char id, unsigned char param)
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
	syncSendCommand(id, packet, 3, answerPacket, answerSize, this->AX12);
	// Check for submitted error code
	if ((errCode = checkAnswerPacket(answerPacket, errorMessage)) != OK) {
		// We received an error code
		fprintf(stderr, "Error when checkAnswerPacket (%d)\n", errCode);

	} else {
		// No error
		//return 1;
	}
	return errCode;
}


bool KasparServos::getAxes(int *ax)
{
	*ax = numOfAxes;
	return true;
}


bool KasparServos::setPositionMode()
{
	return true;
}


bool KasparServos::positionMove(int j, double ref)
{
    if(!checkIndexRange(j))
    {
        return false;
    }
    int blankReturnSize = -1;
	unsigned char blankReturn[] = {0, 0, 0};
    if(servoList[j].type == this->AX12)
    {
        double speed;
	    getRefSpeed(j, &speed);
	    const int instl = 5 + 1;
	    //    unsigned char inst[instl] = {INST_WRITE, CT_GOAL_POSITION, (unsigned char) (normalisePosition(ref)&0xFF), (unsigned char) ((normalisePosition(ref) >> 8) & 0xFF), (unsigned char) (normaliseSpeed(speed)&0xFF), (unsigned char) ((normaliseSpeed(speed) >> 8)&0xFF00)};

	    unsigned char inst[instl] = {INST_WRITE, CT_GOAL_POSITION, (unsigned char) (normalisePosition(ref)&0xFF), (unsigned char) ((normalisePosition(ref) >> 8) & 0xFF), (unsigned char) (normaliseSpeed(speed)&0xFF), (unsigned char) ((normaliseSpeed(speed) >> 8)&0xFF)};

	    positions[j] = ref;
	    //return sendCommand(jointNumbers[j], inst, instl, blankReturn, blankReturnSize);
	    //return syncSendCommand(jointNumbers[j], inst, instl, blankReturn, blankReturnSize, this->AX12);
        return syncSendCommand(servoList[j].id, inst, instl, blankReturn, blankReturnSize, servoList[j].type);
    }
    else if(servoList[j].type == this->MINISSC)
    {
        unsigned char pos;
        //unsigned char pos=(unsigned char)((int)(positions[j]*1.411) + 127);
        
        if(servoList[j].id%16 >= 8)
        {
            if(ref > 180 || ref > servoList[j].maxPos)
            {
                ref = min(180, servoList[j].maxPos);
            }
            if(ref < 0 || ref < servoList[j].minPos)
            {
                ref = max(0, servoList[j].minPos);
            }
            pos = (unsigned char)((int)(ref*1.41));
#ifdef _DEBUG
            printf("higher range: position %f %d\n", ref, pos);
#endif
        }
        else if(servoList[j].id%16 < 8 && servoList[j].id%16 >= 0)
        {
            if(ref > 90 || ref > servoList[j].maxPos)
            {
                ref = min(90, servoList[j].maxPos);
            }
            if(ref < 0 || ref < servoList[j].minPos)
            {
                ref = max(0, servoList[j].minPos);
            }
            pos = (unsigned char)((int)(ref*2.82));
#ifdef _DEBUG
			printf("lower range: position %f %d ...\n", ref, pos);
#endif
        }
        unsigned char cmd[3];

        //ignore speed;
        cmd[0]=(unsigned char)0xFF; //sync byte
        cmd[1]=servoList[j].id; //servo number byte
        cmd[2]=pos; //position byte
	    positions[j] = ref;

        //serial->send(cmd, 3);
        this->syncSendCommand(servoList[j].id, cmd, 3, blankReturn, blankReturnSize, this->MINISSC);
        return true;    
    }
    else if(servoList[j].type == this->POLOLU)
    {
        //unsigned char pos=(unsigned char)((int)(positions[j]*1.411) + 127);
	    positions[j] = ref;
        
        if(ref > 180 || ref > servoList[j].maxPos)
        {
                ref = min(180, servoList[j].maxPos);
        }
        if(ref < 0 || ref < servoList[j].minPos)
        {
                ref = max(0, servoList[j].minPos);
        }
        //            pos = (unsigned char)((int)(ref*1.416));

 //       int pos=1500+round(ref*11.11);
    int pos = 500 + round(ref*27.77);
    unsigned char cmd[6];

    cmd[0]=0x80;
    cmd[1]=0x01;

    cmd[2]=0x04;
    cmd[3]=(unsigned char)servoList[j].id;

    cmd[4]=(unsigned char)(pos>>8); //high pos byte
    cmd[5]=(unsigned char)pos; //low pos byte

        //serial->send(cmd, 3);
    this->syncSendCommand(servoList[j].id, cmd, 6, blankReturn, blankReturnSize, this->POLOLU);
        return true;    
    }

    return false;
}

bool KasparServos::positionMove(const double *refs)
{
	bool t = true;
    std::vector<unsigned char> command; 
    command.push_back(0xFF);
    command.push_back(0xFF);
    command.push_back(AX12_BROADCAST_ID);
    command.push_back(0x00);  // temporary. length. will be calculated later
    command.push_back(INST_SYNCWRITE);
    command.push_back(CT_GOAL_POSITION);
    command.push_back(0x04);
    
    int count = 0;
	for (int k = 0; k < numOfAxes; k++) {
        if(refs[k] != -1)
        {
            if(this->servoList[k].type != AX12)
            {
                positionMove(k, refs[k]);
                //if (!this->positionMove(k, refs[k]))
	        	//	t = false;
            }
            else
            {
                count ++;

                // prepare sync command
                command.push_back((unsigned char)(this->servoList[k].id));
                command.push_back((unsigned char) (normalisePosition(refs[k])&0xFF));
                command.push_back((unsigned char) ((normalisePosition(refs[k]) >> 8) & 0xFF));
                double speed;
    	        getRefSpeed(k, &speed);
                command.push_back((unsigned char) (normaliseSpeed(speed)&0xFF));
                command.push_back((unsigned char) ((normaliseSpeed(speed) >> 8)&0xFF));

            }
        }
	}
    if(count > 0)
    { 
        unsigned char checksum = 0x00;

        int blankReturnSize = -1;
	    unsigned char blankReturn[] = {0, 0, 0}; // not used . just to fill the parameter brackets

        command[3] = (4 + 1) * count + 4;  // calculate length

        for (int i = 2; i < command.size(); i++)
        {
			//printf("%x ", command[i]);
            checksum += command[i];
        }
		//printf("\n");
        checksum = ~checksum;
        command.push_back(checksum);
        unsigned char *commandarray = &command[0];
		
		this->syncSendCommand(AX12_BROADCAST_ID, commandarray, command.size(), blankReturn, blankReturnSize, this->AX12);
    }

	return t;
}

bool KasparServos::relativeMove(int j, double delta)
{
    if(!checkIndexRange(j))
    {
        return false;
    }
	double v = positions[j];
    if(servoList[j].type == this->AX12)
    {
        if (!getEncoder(j, &v)) {
            return false;
        }
    }
	return this->positionMove(j, v + delta);
}

bool KasparServos::relativeMove(const double *deltas)
{
	bool t = true;
	for (int k = 0; k < numOfAxes; k++) {
		//if (!this->positionMove(k, positions[k] + deltas[k]))
        if(!this->relativeMove(k, deltas[k]))		
            t = false;
	}
	return t;
}

bool KasparServos::checkMotionDone(int j, bool *flag)
{
    if(!checkIndexRange(j))
    {
        return false;
    }
	double v = 0;
	bool t = true;
    if(servoList[j].type = this->AX12){
    	t = getEncoder(j, &v);
	    *flag = (std::fabs(v - positions[j]) < MOTION_COMPLETION_TOLERANCE);
    }
    else{
        printf("Not implemented. Servo has no positoin feedback\n");
        t = true;
    }
    return t;
}

bool KasparServos::checkMotionDone(bool *flag)
{
	for (int k = 0; k < numOfAxes; k++) {
		if (!this->checkMotionDone(k, &flag[k]))
			return false;  // no need to go through all servos
	}
	return true;
}

// TODO: change the conditions
bool KasparServos::setRefSpeed(int j, double sp)
{
    if(!checkIndexRange(j))
    {
        return false;
    }
    if(servoList[j].type == this->AX12)
    {
        if (sp < 1 || sp < servoList[j].minSpeed) {
		    printf("Invalid speed value, should be from 1 to 114. Axis %d\n", j);
            //speeds[j] = max(1, servoList[j].minSpeed);
		    speeds[j] = 1;
			//return false;
        } else if (sp > 114 || sp > servoList[j].maxSpeed) {
		    printf("Invalid speed value, should be from 1 to 114. Axis %d\n", j);
		    speeds[j] = 114;
		    //return false;
	    } else {
		    speeds[j] = sp;
		    return true;
	    }
    }
    else if(servoList[j].type == this->MINISSC)
    {
        speeds[j] = sp;
        printf("Sorry, Minissc protocol does not support speed control\n");
        return true;
    }
    else if(servoList[j].type = this->POLOLU)
    {
        speeds[j] = sp;
        // set pololu speed. command 1
        ACE_OS::fprintf(stderr, "Pololu speed mode is not implemented\n");
        return true;
    }
    else
    {
        speeds[j] = sp;
        printf("Sorry, Unknown type. Not implemented\n");
        return false;
    }
}

bool KasparServos::setRefSpeeds(const double *spds)
{
	bool t = true;
	for (int k = 0; k < numOfAxes; k++) {
		if (!setRefSpeed(k, spds[k]))
			t = false;
	}
    // here always return true to make it the main program run more smoothly. although return t seems more reasoable.
	return true;
}

bool KasparServos::setRefAcceleration(int j, double acc)
{
	return NOT_YET_IMPLEMENTED("setRefAcceleration");
}

bool KasparServos::setRefAccelerations(const double *accs)
{
	return NOT_YET_IMPLEMENTED("setRefAccelerations");
}

bool KasparServos::getRefSpeed(int j, double *ref)
{
    if(!checkIndexRange(j))
    {
        return false;
    }
	*ref = speeds[j];
    if(servoList[j].type == this->MINISSC)
    {
        printf("MINISSC does not support speed control\n");
        return false;
    }
	return true;
}

bool KasparServos::getRefSpeeds(double *spds)
{
	for (int k = 0; k < numOfAxes; k++) {
		spds[k] = speeds[k];
	}
	return true;
}

bool KasparServos::getRefAcceleration(int j, double *acc)
{
	return NOT_YET_IMPLEMENTED("getRefAcceleration");
}

bool KasparServos::getRefAccelerations(double *accs)
{
	return NOT_YET_IMPLEMENTED("getRefAccelerations");
}

bool KasparServos::stop(int j)
{
    if(!checkIndexRange(j))
    {
        return false;
    }
	double v = 0;
	bool t = true;
    if(servoList[j].type == this->AX12) {
    	t = getEncoder(j, &v);
        if(t)
        {
            this->positionMove(j, v);
        }
    }
    else{
        printf("Not implemented. Servo has no positoin feedback %d\n", j);
        t = true;
    }
    return t;
}


bool KasparServos::stop() {
	for (int i = 0; i < numOfAxes; i++) {
		if (!stop(i))
			return false;
	}
	return true;
}

bool KasparServos::resetEncoder(int j) {
	return NOT_YET_IMPLEMENTED("resetEncoder");
}

bool KasparServos::resetEncoders() {
	return NOT_YET_IMPLEMENTED("resetEncoders");
}

bool KasparServos::setEncoder(int j, double val) {
	return NOT_YET_IMPLEMENTED("setEncoder");
}

bool KasparServos::setEncoders(const double *vals) {
	return NOT_YET_IMPLEMENTED("setEncoders");
}

bool KasparServos::getEncoder(int j, double *v) {
#ifdef _DEBUG
    printf("GetEncoder %d %f\n", j, *v);
#endif
    if(!checkIndexRange(j))
    {
        return false;
    }
    // Only ax12 servos have feedback
    if(servoList[j].type != this->AX12)
    {
        *v = -1;
        return NOT_YET_IMPLEMENTED("getEncoder for non-AX12 motors");
    }

	*v = -1;     // invalid value for encoder of AX12 motor
	int ret = 1;

	int blankReturnSize = -1;
	unsigned char blankReturn[] = {0, 0, 0};

	const int instl = 3;
	unsigned char inst[instl] = {INST_READ, (unsigned char) CT_PRESENT_POSITION, (unsigned char) 2};
	//ret = sendCommand(jointNumbers[j], inst, instl, blankReturn, blankReturnSize);
    ret = syncSendCommand(servoList[j].id, inst, instl, blankReturn, blankReturnSize, this->AX12);

#ifdef _DEBUG
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

/*
although it may return false, because some mortors do not support this, valid speeds are still returned. invalid values are assigned with -1, that can be identified by checking this 
*/
bool KasparServos::getEncoders(double *encs) {
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

bool KasparServos::getEncoderSpeed(int j, double *sp) {
    if(!checkIndexRange(j))
    {
        return false;
    }
    if(servoList[j].type != this->AX12)
    {
        *sp = -1;
        return NOT_YET_IMPLEMENTED("getEncoderSpeed for non-AX12 motors");
    }

    bool ret;
	int numBytes;
	int blankReturnSize = -1;
	unsigned char blankReturn[] = {0, 0, 0};
	const int instl = 3;
	unsigned char inst[instl] = {INST_READ, (unsigned char) CT_PRESENT_SPEED, (unsigned char) 2};
	//ret = sendCommand(jointNumbers[j], inst, instl, blankReturn, blankReturnSize);
	ret = syncSendCommand(servoList[j].id, inst, instl, blankReturn, blankReturnSize, this->AX12);
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

/*
although it may return false, because some mortors do not support this, valid speeds are still returned. invalid values are assigned with -1, that can be identified by checking this 
*/
bool KasparServos::getEncoderSpeeds(double *spds) {
	int k = 0;
	bool tt = true;
	for (k = 0; k < numOfAxes; k++) {
		if (!getEncoderSpeed(k, &spds[k]))
			tt = false;
	}
    // although it may return false, valid speeds are still returned. invalid values are assigned with -1, that can be identified by checking this 
    return tt;
} 

bool KasparServos::getEncoderAcceleration(int j, double *spds) {
	return NOT_YET_IMPLEMENTED("getEncoderAcceleration");
}

bool KasparServos::getEncoderAccelerations(double *accs) {
	return NOT_YET_IMPLEMENTED("getEncoderAccelerations");
}


bool KasparServos::setTorqueMode()
{
	ACE_OS::fprintf(stderr, "Note: AX12 does not support torque control mode. This is only used to get torque feedback.\n");
	return NOT_YET_IMPLEMENTED("setTorqueMode");
}

bool KasparServos::getRefTorques(double *t)
{
    for (int k = 0; k < numOfAxes; k++) {
		//t[k] = torques[k];
	    getRefTorque(k, &t[k]);
    }
	ACE_OS::fprintf(stderr, "Note: AX12 does not support torque control mode. This is just the MAX tourque set for the ax12 servo.\n");
	return true;
}

bool KasparServos::getRefTorque(int j, double *t)
{
    if(!checkIndexRange(j))
    {
        return false;
    }
    if(servoList[j].type != this->AX12)
    {
        ACE_OS::fprintf(stderr, "WARNING: NON-AX12 servos are not supported for torque mode. \n");
    }
    else{
#ifndef _DEBUG
//    	ACE_OS::fprintf(stderr, "Note: AX12 does not support torque control mode. This is just the MAX tourque set for the ax12 servo.\n");
#endif
	}
    *t = (int)(torques[j]);
	return true;
}
/*
although it may return false, because some motors do not support this, valid torques are still set.  
*/
bool KasparServos::setRefTorques(const double *t)
{
	bool tt = true;//
	for (int k = 0; k < numOfAxes; k++) {
		if (!this->setRefTorque(k, t[k]))
			tt = false;
	}
	return tt;
}

bool KasparServos::setRefTorque(int j, double t)
{
    if(!checkIndexRange(j))
    {
        return false;
    }
    if(servoList[j].type != this->AX12)
    {
        return NOT_YET_IMPLEMENTED("setTroque with non-AX12 motors");
    }

	if (t < 0) {
		ACE_OS::fprintf(stderr, "torque (%d) should in range [0 1023] or [0 0x3FF]. t is set to 0 here\n", t);
		t = 0;
	} else if (t > 1023) {
		ACE_OS::fprintf(stderr, "torque (%d) should in range [0 1023] or [0 0x3FF]. t is set to 1023 here\n", t);
		t = 1023;
	}

	int blankReturnSize = -1;
	unsigned char blankReturn[] = {0, 0, 0};
	const int instl = 3;
	
	if( t >= 1)
	{
		t = 1;
	}
    torques[j] = t;
	
	unsigned char inst[instl] = {INST_WRITE, CT_TORQUE_ENABLE, (unsigned char) ((int)t)};   // simply set torque on and off. 
    return syncSendCommand(servoList[j].id, inst, instl, blankReturn, blankReturnSize, this->AX12);
	//unsigned char inst[instl] = {INST_WRITE, CT_TORQUE_LIMIT, (unsigned char) ((int) t & 0xFF), (unsigned char) (((int) t << 8) & 0x03)};
	//return sendCommand(jointNumbers[j], inst, instl, blankReturn, blankReturnSize);
    //return syncSendCommand(servoList[j].id, inst, instl, blankReturn, blankReturnSize, this->AX12);
}

bool KasparServos::setTorquePid(int j, const Pid &pid)
{
	ACE_OS::fprintf(stderr, "Note: AX12 does not support torque control mode. This is only used to get torque feedback.\n");
	return NOT_YET_IMPLEMENTED("setTorquePid");
}

bool KasparServos::getTorque(int j, double *t)
{
    if(!checkIndexRange(j))
    {
        return false;
    }
    if(servoList[j].type != this->AX12)
    {
        *t = -1;
        return NOT_YET_IMPLEMENTED("getTroque with non-AX12 motors");
    }
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
	ret = syncSendCommand(servoList[j].id, inst, instl, blankReturn, blankReturnSize, this->AX12);

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
/*
although it may return false, because some mortors do not support this, valid torques are still returned. invalid values are assigned with -1, that can be identified by checking this 
*/
bool KasparServos::getTorques(double *t)
{
	int k = 0;
	bool tt = true;
	for (k = 0; k < numOfAxes; k++) {
		if (!getTorque(k, &t[k]))
			tt = false;
	}
	return tt;
}

bool KasparServos::setTorquePids(const Pid *pids)
{
	ACE_OS::fprintf(stderr, "Note: AX12 does not support torque control mode. This is only used to get torque feedback.\n");
	return NOT_YET_IMPLEMENTED("setTorquePids");
}

bool KasparServos::setTorqueErrorLimit(int j, double limit)
{
	ACE_OS::fprintf(stderr, "Note: AX12 does not support torque control mode. This is only used to get torque feedback.\n");
	return NOT_YET_IMPLEMENTED("setTorqueErrorLimit");
}

bool KasparServos::setTorqueErrorLimits(const double *limits)
{
	ACE_OS::fprintf(stderr, "Note: AX12 does not support torque control mode. This is only used to get torque feedback.\n");
	return NOT_YET_IMPLEMENTED("setTorqueErrorLimits");
}

bool KasparServos::getTorqueError(int j, double *err)
{
	ACE_OS::fprintf(stderr, "Note: AX12 does not support torque control mode. This is only used to get torque feedback.\n");
	return NOT_YET_IMPLEMENTED("getTorqueError");
}

bool KasparServos::getTorqueErrors(double *errs)
{
	ACE_OS::fprintf(stderr, "Note: AX12 does not support torque control mode. This is only used to get torque feedback.\n");
	return NOT_YET_IMPLEMENTED("getTorqueErrors");
}

bool KasparServos::getTorquePidOutput(int j, double *out)
{
	ACE_OS::fprintf(stderr, "Note: AX12 does not support torque control mode. This is only used to get torque feedback.\n");
	return NOT_YET_IMPLEMENTED("getTorquePidOutput");
}

bool KasparServos::getTorquePidOutputs(double *outs)
{
	ACE_OS::fprintf(stderr, "Note: AX12 does not support torque control mode. This is only used to get torque feedback.\n");
	return NOT_YET_IMPLEMENTED("getTorquePidOutputs");
}

bool KasparServos::getTorquePid(int j, Pid *pid)
{
	ACE_OS::fprintf(stderr, "Note: AX12 does not support torque control mode. This is only used to get torque feedback.\n");
	return NOT_YET_IMPLEMENTED("getTorquePid");
}

bool KasparServos::getTorquePids(Pid *pids)
{
	ACE_OS::fprintf(stderr, "Note: AX12 does not support torque control mode. This is only used to get torque feedback.\n");
	return NOT_YET_IMPLEMENTED("getTorquePids");
}

bool KasparServos::getTorqueErrorLimit(int j, double *limit)
{

	ACE_OS::fprintf(stderr, "Note: AX12 does not support torque control mode. This is only used to get torque feedback.\n");
	return NOT_YET_IMPLEMENTED("getTorqueErrorLimit");
}

bool KasparServos::getTorqueErrorLimits(double *limits)
{
	ACE_OS::fprintf(stderr, "Note: AX12 does not support torque control mode. This is only used to get torque feedback.\n");
	return NOT_YET_IMPLEMENTED("getTorqueErrorLimits");
}

bool KasparServos::resetTorquePid(int j)
{
	ACE_OS::fprintf(stderr, "Note: AX12 does not support torque control mode. This is only used to get torque feedback.\n");
	return NOT_YET_IMPLEMENTED("resetTorquePid");
}

bool KasparServos::disableTorquePid(int j)
{
	ACE_OS::fprintf(stderr, "Note: AX12 does not support torque control mode. This is only used to get torque feedback.\n");
	return NOT_YET_IMPLEMENTED("disableTorquePid");
}

bool KasparServos::enableTorquePid(int j)
{

	ACE_OS::fprintf(stderr, "Note: AX12 does not support torque control mode. This is only used to get torque feedback.\n");
	return NOT_YET_IMPLEMENTED("enableTorquePid");
}

bool KasparServos::setTorqueOffset(int j, double v)
{
	ACE_OS::fprintf(stderr, "Note: AX12 does not support torque control mode. This is only used to get torque feedback.\n");
	return NOT_YET_IMPLEMENTED("setTorqueOffset");
}

/*
bool KasparServos::setRefTorques(const double *t)
{
	return NOT_YET_IMPLEMENTED("setRefTorques");
}

bool KasparServos::setRefTorque(int j,double t)
{
	return NOT_YET_IMPLEMENTED("setRefTorque");
}*/

bool KasparServos::getTorqueRange(int j,double *min,double *max)
{
	return NOT_YET_IMPLEMENTED("getTorqueRange");
}

bool KasparServos::getTorqueRanges(double *min,double *max)
{
	return NOT_YET_IMPLEMENTED("getTorqueRanges");
}


/*
bool KasparServos::initMotorIndex(yarp::os::Bottle *sensorIndex) {
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
*/

int KasparServos::normalisePosition(double position) {
	if (position < 0) {
		position = 0;
		printf("Invalid position value, should be from 0 to 300\n");
	} else if (position > 300) {
		position = 300;
		printf("Invalid position value, should be from 0 to 300\n");
	}
	return (int) (1023 * position / 300);
	//return (int) (1024 * position / 301);
}

int KasparServos::normaliseSpeed(double speed) {
	// speed -> [1 114] RPM
	// when speed is 0, it is maximum possible speed, no speed control. so it is not useful here

	if (speed < 1) {
		printf("Invalid speed value, should be from 1 to 114 (%f)\n", speed);
		speed = 1;
		return speed;
	} else if (speed > 114) {
		printf("Invalid speed value, should be from 1 to 114\n");
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


bool KasparServos::checkIndexRange(int j)
{
    if(j >= this->numOfAxes || j < 0)
    {
        fprintf(stderr, "ERROR: Axis index (%d) should be positive and lower than num of axes %d\n", j, this->numOfAxes);
        return false;
    }
    return true;
}

bool KasparServos::getBaudrate(int _baudrate, BaudRateType &baudrate)
{
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
		printf("WRONG BAUDRATE. USE 9600 as DEFAULT %d\n", _baudrate);
		return false;	
	}
	return true;
}

bool KasparServos::initTypePortMap(Bottle serialports)
{
	int numOfPorts = serialports.size();

    int count = 0;
    if(numOfPorts == 0)
    {
        printf("NumOfPorts is 0. Check the config file.\n");
        return false;
    }
	//portList.resize(numOfPorts);

	for(int i = 0; i < numOfPorts; i++)
    {
		QextSerialPort *qsp = new QextSerialPort(QextSerialPort::Polling); 

        yarp::os::Bottle portconf(serialports.get(i).toString());
		std::string type = portconf.get(0).asString().c_str();
		enum ServoType tp;

		if(type == "AX12")
		{
			tp = this->AX12;
		}
		else if(type == "MINISSC")
		{
			tp = this->MINISSC;
		}
		else if(type == "SSC32")
		{
			tp = this->SSC32;
		}
		else if(type == "POLOLU")
		{
			tp = this->POLOLU;
		}
		if(!this->initSerialPort(portconf, qsp))
		{
			return false;
		}
		
		portList.push_back(qsp);
		typePortMap[tp] = qsp;
	}
	return true;
}

bool KasparServos::initSerialPort(Bottle &b, QextSerialPort *port)
{
	//yarp::os::Bottle portconf(b.get(i).toString());
	std::string type = b.get(0).asString().c_str();
	std::string comport = b.get(1).asString().c_str();
	int _baudrate = b.get(2).asInt();

	BaudRateType baudrate = BAUD115200;
	if(! getBaudrate(_baudrate, baudrate))
	{
		printf("ERROR in setting baudrate\n");
		return false;
	}

	ACE_TRACE("KasparServos::initialize");
	ACE_OS::printf("Opening KasparServos Device (%s) (%d)\n", comport.c_str(), _baudrate);

	port->setPortName(QString(comport.c_str()));
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

	return true;
}

void KasparServos::initData()
{
    for(int i = 0; i < this->numOfAxes; i++)
    {
        this->positions[i] = -1;
        this->torques[i] = 1;
        this->speeds[i] = 5;
    }
}
