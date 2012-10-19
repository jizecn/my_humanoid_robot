#ifndef __QTOUCHSENSORDATA_H__
#define __QTOUCHSENSORDATA_H__

const int DefaultLength = 50;
const int Channels = 16;

class QTouchSensorData
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
    QTouchSensorData(int len = DefaultLength, int chan = Channels)
    {
        //    int i = 0;
        length = len;
        chan = chan;
        //    allocateData();
    }

    ~QTouchSensorData()
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
