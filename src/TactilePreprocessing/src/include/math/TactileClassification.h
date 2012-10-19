#ifndef __TACTILECLASSIFICATION_H__
#define __TACTILECLASSIFICATION_H__

#include "TactileData.h"
#include <stdio.h>
#include <stdlib.h>
#include "svm.h"
#include <iostream>
#include <yarp/os/Property.h>

using namespace yarp::os;

namespace zjMath
{
	class TactileClassification;
	const int NO_EVENT = -100;
	const int POKE = -1;
	const int TOUCH = 1;

	const int SHORT_TOUCH_DURATION = 3;
	const int LONG_TOUCH_DURATION = 30;

	const double THRESHOLD_SOFT_HARD = 150;
}


// mainly a wrapper of libsvm, but with additional functions
class TactileClassification
{
public:

	//enum touchType {poke = -1, touch = 1, none = -100};
    //TactileClassification();
    virtual ~TactileClassification()
    {
      std::cout << "Destructor -- TactileClassification" << std::endl;
    	if(svmnode != NULL)
    	{
    		free(svmnode);
    	}
    	if(model != 0)
        {
            svm_free_and_destroy_model(&model);
        }
    }
    
    //   bool initConfiguration(const char *file);
    virtual bool customConfiguration(yarp::os::Property &cfg) = 0;
	int performClassification();
	
    static int classify(svm_node *svmnode, svm_model *model, int predict_probability);
    
    //int performClassification(svm_node *svmnode);
    
    bool loadSVMModel(const char *modelFileName);

	virtual bool prepareSVMNode(TactileData &data, bool scale) = 0;

    //static TactileData scaleData(TactileData *tactileData, double lowerBound = 0, double higherBound = 1);
    static bool scaleData(TactileData &tactileData, TactileData &scaledData, double lowerBound = 0, double higherBound = 1);
    
//    static double min(TactileData *tactileData);
    //  static double max(TactileData *tactileData);

	

protected:
    svm_model *model;
    svm_node *svmnode; 
    int svmnodelength;  
    int dataDim;
    virtual bool prepareSVMNode(double *data) = 0;
};

#endif // __TACTILECLASSIFICATION_H__
