#ifndef __TACTILE_DATA_H__
#define __TACTILE_DATA_H__

#include <yarp/sig/Vector.h>
#include <cmath>
#include <iostream>
#include <vector>
#include <yarp/os/Semaphore.h>
#include "svm.h"

#include "math/Math.h"

//#include "math/TactileClassification.h"
//#include "math/TactileClassificationRawData.h"
//#include "math/TactileClassificationHist.h"

#define THRESHOLD_MAX 20
#define THRESHOLD_SUM 300
#define THRESHOLD_COUNTING_NUM_TAXEL 5 // if there are THRESHOLD_COUNTING_NUM_TAXEL taxels above THRESHOLD_COUNTING_FORCE
#define THRESHOLD_COUNTING_FORCE 30

class TactileData : public yarp::sig::Vector
{
public:
    enum threshold_method {thres_max, thres_sum, thres_counting};
    
    struct threshold_method_setting {
        threshold_method method;
        double threshold;
        int counting; // Only for method thres_counting.
    threshold_method_setting() : counting(THRESHOLD_COUNTING_NUM_TAXEL) {}  // by default, counting is set as... 
    };
    
    TactileData();
    TactileData(size_t dim);
    TactileData(const yarp::sig::Vector &vec);
    // ~TactileData(); // won't work as the base class is non-virtual destructor
    //unsigned int getDim();
    bool assignData(double *sensorData, const size_t offset, const size_t size, bool useCalibrate = true);
    
    /*both ds and result should be of the same dimension */
    bool diff(TactileData &ds, yarp::sig::Vector &result, bool ifAbs = false); 
    static yarp::sig::Vector diff(TactileData &d1, TactileData &d2, bool ifAbs);
  
    //int classify(svm_model *model, int predict_probability);
    //int performClassification();
    
    //bool loadSVMModel(const char *modelFileName);
    
    double max();
    double min();
    double sum();
  
    void setThreshold(threshold_method_setting thresMethodStruct);
    bool aboveThreshold();
    //bool updateSVMNode();
    
    //svm_node *getSVMNode();
    
    // svmnode is one more longer than data array.
    // length: length of data;
    // svmnode has length + 1 items, and must be initialised in advance 
    //static bool prepareSVMNode(svm_node *svmnode, double *data, int length);
    
private:
    //svm_model model;
    //svm_node *svmnode; 
    //int svmnodelength;
    
    threshold_method_setting defaultThresholdMethod;
    bool aboveThresholdMax();
    bool aboveThresholdSum();
    bool aboveThresholdCounting();
};

#endif // TactileData
