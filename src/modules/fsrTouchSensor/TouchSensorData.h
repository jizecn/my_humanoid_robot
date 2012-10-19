#ifndef __TOUCHSENSORDATA_H__
#define __TOUCHSENSORDATA_H__

const int DefaultLength = 50;
const int Channels = 16;

class TouchSensorData
{
private:
    void allocateData()
    {
        int i = 0;
        data = new int*[Channels];
        data[0] = new int[Channels*DefaultLength];
        for (i=0; i<chan; i++)
        {
            data[i] = data[i-1] + DefaultLength;
        }
    }

public:
    TouchSensorData(int len = DefaultLength, int chan = Channels)
    {
        //    int i = 0;
        length = len;
        chan = chan;
        //    allocateData();
    }

    ~TouchSensorData()
    {
        delete [] data[0];
        delete [] data;
    }

    void getData(int **d)
    {
        d = data;
    }

    int& operator()(int i, int j)
    {
        return data[i][j];
    }

    int length;
    int **data;
    int chan;
    int timeMS; // time in millsecond

    /*
    void setLength(int l)



    {
    length = l;
    }
    */

};

#endif
