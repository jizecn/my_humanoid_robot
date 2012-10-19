#ifndef __SKIN_PART__
#define __SKIN_PART__
#include <stdio.h>
#include <string>

#include <yarp/os/RateThread.h>
#include <yarp/os/Semaphore.h>
#include <yarp/dev/ControlBoardInterfaces.h>
#include <yarp/dev/PolyDriver.h>
#include <yarp/dev/CanBusInterface.h>
#include <yarp/sig/Vector.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_statistics.h>
#include <yarp/os/Bottle.h>
//#include "Triangle.h"
//#include "Fingertip.h"
#include <sstream>
#include <yarp/os/Time.h>

const int CAN_DRIVER_BUFFER_SIZE=2047;

using namespace yarp::os;
using namespace yarp::dev;

#include "TouchSensor.h"
#include <vector>
using namespace std;

class SkinPart : public yarp::os::RateThread {
public:
    SkinPart(Searchable& config,int period=80) : RateThread(period),mutex(1),threshold(20){
        sensorsNum=0;

        for (int t=0; t<16; ++t)  {
            sensor[t]=NULL;
        }

        part="/skinGui/";
        robotPart = config.find("robotPart").asString();

        part.append(robotPart);
        part.append(":i");
        printf("SkinPart %s (%s)is created\n", part.c_str(), this->robotPart.c_str());
        skin_port.open(part.c_str());
        int width =config.find("width" ).asInt();
        int height=config.find("height").asInt();
        bool useCalibration = config.check("useCalibration");
        //useCalibration = true;
        if (useCalibration==true) {
            printf("Using calibrated skin values (0-255)\n");
        }
        else {
            printf("Using raw skin values (255-0)\n");
        }

        yarp::os::Bottle sensorSetConfig=config.findGroup("SENSORS").tail();

        for (int t=0; t<sensorSetConfig.size(); ++t) {       
            yarp::os::Bottle sensorConfig(sensorSetConfig.get(t).toString());

            std::string type(sensorConfig.get(0).asString());
            if (type=="triangle" || type=="fingertip") {
                int id=sensorConfig.get(1).asInt();
                double xc=sensorConfig.get(2).asDouble();
                double yc=sensorConfig.get(3).asDouble();
                double th=sensorConfig.get(4).asDouble();
                double gain=sensorConfig.get(5).asDouble();
                int    lrMirror=sensorConfig.get(6).asInt();
                int    layoutNum=sensorConfig.get(7).asInt();

                printf("%d %f\n",id,gain);

                if (id>=0 && id<16) {
                    if (sensor[id]) {
                        printf("WARNING: triangle %d already exists.\n",id);
                    }
                    else {
                        /*
                        if (type=="triangle") {
                        sensor[id]=new Triangle(xc,yc,th,gain,layoutNum,lrMirror);
                        sensor[id]->setCalibrationFlag(useCalibration);
                        }
                        else {
                        sensor[id]=new Fingertip(xc,yc,th,gain,layoutNum,lrMirror);
                        sensor[id]->setCalibrationFlag(useCalibration);
                        }*/
                        sensor[id] = new TouchSensor();
                        //printf("%d %s\n", id, this->robotPart.c_str());
                        sensor[id]->setCalibrationFlag(useCalibration);
                        ++sensorsNum;
                    }
                }
                else {
                    printf("WARNING: %d is invalid triangle Id [0:15].\n",id);
                }
            }
            else {
                printf("WARNING: sensor type %s unknown, discarded.\n",type.c_str());
            }
        }

        initIndexArray();

        //data = gsl_vector_alloc( sensorsNum * 12);
        sensorsData = (double *)malloc(sizeof(double)*sensorsNum*12);

        //    resize(width,height);
    }

    ~SkinPart(){
        for (int t=0; t<16; ++t) {
            if (sensor[t]) delete sensor[t];
        }
        free(indexArray);
        //gsl_vector_free(data);
        free(sensorsData);
    }

    virtual void run()
    {
        mutex.wait();
        //printf("Running skinpart %s\n", this->part.c_str());

        Bottle *input = skin_port.read();

        yarp::sig::Vector skin_value;
        int sensorId =0;
        if(input != NULL)
        {
            skin_value.resize(input->size());
            for (int i=0; i<input->size(); i++)  {
                skin_value[i] = input->get(i).asDouble();      
                sensorId=i/12;
                if (sensorId<16 && sensor[sensorId]) {
                    sensor[sensorId]->setActivationFromPortData(skin_value[i],i%12);
					//printf("%f\n",skin_value[i]);
                }
            }
        }

     //   for(int k = 0; k < sensorsNum; k++)
     //   {
         //   sensor[indexArray[k]]->getData(sensorsData, k);
     //   }

        mutex.post();
    }

	int getSensorsNum()
	{
		return sensorsNum;
	}

	void writeToFile(FILE *fp)
	{
	    mutex.wait();
        for(int k = 0; k < sensorsNum; k++)
        {
            sensor[indexArray[k]]->getData(sensorsData, k);
        }
		for(int i = 0; i < sensorsNum*12; i++)
		{
			fprintf(fp, "%4.1f ", sensorsData[i]);
		}
		mutex.post();
	}

	void getSensorData(double *&data, int offset)
	{
	    mutex.wait();
		for(int k = 0; k < sensorsNum; k++)
		{
			sensor[indexArray[k]]->getData(data + offset, k); 
			//printf("%f\n", *(data+offset));
		}
		mutex.post();
		//return sensorsData;
	}

    /*
    return: result of data processing. Using Bottle is to make it flexible to different methods. Although currently it will only contain mean, max, min, std, etc, and text description 
    e.g. threshold method: [typeOfTouch (text), mean(double), variance (double)]
    */
    Bottle processData()
    {
        //copying vectors from each sensor to a matrix might not be efficient, but safer. need to see if there is a better and simple way to handle data

        mutex.wait();

        Bottle b; 

        for(int k = 0; k < sensorsNum; k++)
        {
            sensor[indexArray[k]]->getData(sensorsData, k);
        }
        /* 
        for (int i = 0; i < 16; i ++) {
        if(sensor[i])
        {
        //printf("%s %d of %d\n", this->robotPart.c_str(), i, sensorsNum);
        sensor[i]->getData(sensorsData, i);
        //printf(" sensor[ %d ] %s of %d is %f\n",  i, this->robotPart.c_str(),sensorsNum, sensorsData[0]);	
        }
        }
        */      // TODO: process data here... temporary code below
        // mean value here

        double mean, variance, largest, smallest;
        //mean = gsl_stats_mean(sensorsData, 1, 12*sensorsNum);
        //variance = gsl_stats_variance(sensorsData, 1, 12*sensorsNum);

        largest = gsl_stats_max(sensorsData, 1, 12*sensorsNum);
        smallest = gsl_stats_min(sensorsData, 1, 12*sensorsNum);

        if(largest > 20 && largest <= 50)
        {
            printf("max is %f %f\n", largest, smallest);
            b.addString((this->robotPart + "_soft").c_str());
        }
        else if(largest > 50)
        {
            printf("max is %f %f\n", largest, smallest);
            b.addString((this->robotPart + "_strong").c_str());
        }
        else
        {
            b.addString("None");
            //return NULL;
        }
        //b.addDouble(mean);
        //b.addDouble(variance);
        //b.addDouble(largest);
        //b.addDouble(smallest);
        mutex.post();

        return b;
    }

    virtual bool threadInit()
    {
        printf("SkinMeshThreadPort initialising...\n");
        printf("... done!\n");

        printf("Waiting for port connection\n");
        return true;
    }  

    virtual void threadRelease()
    {
        printf("SkinMeshThreadPort releasing...\n");	

        printf("... done.\n");
    }
    /*
    void resize(int width,int height)
    {
    mutex.wait();
    for (int t=0; t<16; ++t) {
    if (sensor[t]) sensor[t]->resize(width,height,40);
    }
    mutex.post();
    }*/

    /*
    void eval(double *image)
    {
    mutex.wait();

    for (int t=0; t<16; ++t) {
    if (sensor[t]) sensor[t]->eval(image);
    }

    mutex.post();
    }

    void draw(unsigned char *image)
    {
    for (int t=0; t<16; ++t) {
    if (sensor[t]) sensor[t]->draw(image);
    }        
    }
    */
    void setPortToConnect(const char *toPort)
    {
        this->toPort = toPort;
    }

    bool connect()
    {
        if(toPort.length() == 0)
        {
            return false;

        }
        if(!yarp::os::Network::isConnected(toPort.c_str(), part.c_str()))
		{
			printf("TRYING TO CONNECT DATALOGGER TO SKINSENSOR %s\n", part.c_str());        
			return yarp::os::Network::connect(toPort.c_str(), part.c_str());
		
		}
		return true;
    }

    bool disconnect()
    {
        if(toPort.length() == 0)
            return false;
        if(yarp::os::Network::isConnected(toPort.c_str(), part.c_str()))
            return yarp::os::Network::disconnect(toPort.c_str(), part.c_str());
        return true;
    }

    bool isConnected()
    {
        return yarp::os::Network::isConnected(toPort.c_str(), part.c_str());
    }

    string getPartName()
    {
        return robotPart;
    }

    void setThreadhold(double threshold)
    {
        this->threshold = threshold;
    }

    std::string infoToString()
    {
        std::string s;
        std::stringstream out;
        out << this->sensorsNum*12;
        s = out.str();
        string ss = this->robotPart;
        ss.append(" TOTAL_SENSOR_NUMBER ");
        ss.append(s);
        ss.append("\n");
        return ss;
    }

private:
    //  int numOfSensors;
    std::vector<TouchSensor> sensorList;
    std::string  part;
    std::string robotPart;
    BufferedPort<Bottle> skin_port;						
    TouchSensor *sensor[16];  
    yarp::os::Semaphore mutex;  
    int sensorsNum;
    int *indexArray;

    double threshold; 
    //yarp::os::Network yarp;
    //gsl_vector *data; // store data of sensors of this part [sensor1; sensor2;...] each is a vector of 12 items
    double *sensorsData;
    std::string toPort; 
    void initIndexArray()
    {
        // to keep index of available sensor ids
        indexArray = (int *)malloc(sizeof(int)*sensorsNum);
        int k = 0;
        // very basic, no error checking
        for(int i = 0; i < 16; i++)
        {
            if(sensor[i])
            {
                indexArray[k] = i;
                k++;
            }	
        } 
    }

};

#endif
