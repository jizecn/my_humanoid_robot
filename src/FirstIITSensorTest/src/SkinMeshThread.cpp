// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include <iCub/SkinMeshThread.h>

#include <yarp/os/Time.h>

#include <iostream>
#include <fstream>

const int CAN_DRIVER_BUFFER_SIZE=2047;

std::ofstream logFile;

bool SkinMeshThread::threadInit()
{
	printf("Image Thread initialising...\n");

	Property prop;
    prop.put("device", "ecan");
    prop.put("CanTxTimeout", 500);
    prop.put("CanRxTimeout", 500);
    prop.put("CanDeviceNum", 0); //7 is the left_hand, 8 is the right_hand
    prop.put("CanMyAddress", 0);
    prop.put("CanTxQueueSize", CAN_DRIVER_BUFFER_SIZE);
    prop.put("CanRxQueueSize", CAN_DRIVER_BUFFER_SIZE);

    pCanBus=NULL;
    pCanBufferFactory=NULL;

    driver.open(prop);
    if (!driver.isValid())
    {
        fprintf(stderr, "Error opening PolyDriver check parameters\n");
        return false;
    }

    driver.view(pCanBus);
    
    if (!pCanBus)
    {
        fprintf(stderr, "Error opening /ecan device not available\n");
        return false;
    }

    driver.view(pCanBufferFactory);
    pCanBus->canSetBaudRate(0); //default 1MB/s

    for (int id=0; id<16; ++id)
    {
        if (sensor[id])
        {
            pCanBus->canIdAdd(cardId+id);
        }
    }

    canBuffer=pCanBufferFactory->createBuffer(4*sensorsNum);

    // log file preparation
    logFile.open("tmp.log");
    if(!logFile) {
        fprintf(stderr, "Error: cannot open file\n!");
    }

    printf("... done!\n");

    return true;
}

void SkinMeshThread::run()
{	
    mutex.wait();
    unsigned int canMessages=0;

    bool res=pCanBus->canRead(canBuffer,4*sensorsNum,&canMessages,true);

    for (unsigned int i=0; i<canMessages; i++)
    {
        CanMessage &msg=canBuffer[i];

        if ((msg.getId() & 0xFFFFFFF0) == cardId)
        {
            int sensorId=msg.getId() & 0x0F;

            if (sensor[sensorId])
            {
                if (msg.getData()[0] & 0x80)
                {
                    sensor[sensorId]->setActivationLast5(msg.getData()); // last 5 bytes
                }
                else
                {
                    sensor[sensorId]->setActivationFirst7(msg.getData()); // first 7 bytes
                }
            }
        }
    }
    
    // write data into a file tmp.log
    
    
    if(logFile.is_open()) {
        
        for(int sensorId = 0; sensorId < 6; sensorId++) {
            logFile << "sensor id " << sensorId << " ";
            for (int l = 0; l < 12; l++) {
                logFile << sensor[sensorId]->activation[l] << " ";
                if(sensorId == 0){
                    std::cout << sensor[sensorId]->activation[l] << " ";
                    
                }
            }
            logFile << "\n";
            if(sensorId == 0)
                std::cout << std::endl;
        }
    }
    

    mutex.post();
}

void SkinMeshThread::threadRelease()
{
	printf("Skin Mesh Thread releasing...\n");	
	if (pCanBufferFactory) 
    {
        pCanBufferFactory->destroyBuffer(canBuffer);
    }
    driver.close();

    // release log file
    if(logFile.is_open()){
        logFile.close();
    }

    printf("... done.\n");
}
