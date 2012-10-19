#ifndef __TACTILECLASSIFICATIONRAWDATA_H__
#define  __TACTILECLASSIFICATIONRAWDATA_H__

#include "math/TactileClassification.h"
#include <iostream>
using namespace std;

namespace zjMath
{
	class TactileClassificationRawData;
}


class TactileClassificationRawData : public TactileClassification
{
public:
    TactileClassificationRawData(int dataDim)
    {
        model = 0;
        //svmnode = NULL;
        svmnodelength = dataDim + 1;
        this->dataDim = dataDim;
        svmnode = (svm_node *)malloc(svmnodelength*sizeof(svm_node));
		for (int i = 0; i < this->dataDim; i++) {
			svmnode[i].index = i;
			svmnode[i].value = 0;
		}
		svmnode[this->dataDim].index = -1;
    }
    /*
    ~TactileClassificationRawData()
    {
    	cout << "Destructor -- TactileClassificationRawData" << endl;
    }
    */

    bool customConfiguration(yarp::os::Property &cfg)
    {
        return true;
    }

    bool prepareSVMNode(double *data)
    {
        int i = 0;
        for(i = 0; i < dataDim; i++)
        {
            svmnode[i].index = i+1;
            svmnode[i].value = data[i];
        }
        svmnode[svmnodelength].index = -1;
        return true;
    }
	
	bool prepareSVMNode(TactileData &data, bool scale)
	{
	  if(data.length() != this->dataDim)
	    {
	      cout << "Data dimension does not match, when prepareSVM Node   " << this->dataDim << " != " << data.length() << endl;
	      cout << "SVM node length is --> " << svmnodelength << endl;
	      return false;
	    }

      TactileData newData(this->dataDim);
      if(scale)
      {
          this->scaleData(data, newData, 0, 1);
	      return this->prepareSVMNode(newData.data()); 
      }
	  else
	  {
  	      return this->prepareSVMNode(data.data()); 
	  }
	}
	
private:
    
};

#endif //  __TACTILECLASSIFICATIONRAWDATA_H__
