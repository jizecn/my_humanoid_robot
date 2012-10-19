#ifndef __TACTILECLASSIFICATIONHIST_H__
#define __TACTILECLASSIFICATIONHIST_H__

#include "math/TactileClassification.h"
#include "gsl/gsl_histogram.h"
namespace zjMath
{
	class TactileClassificationHist;
}

struct histgramSetting
{
/*
  histgramSetting(int numBins, double lowBound, double upBound)
  {
  numOfBins = numBins;
  lowerBound = lowBound;
  upperBound = upBound;
  }
*/
    int numOfBins;
    double lowerBound;
    double upperBound;
};

class TactileClassificationHist: public TactileClassification {
public:
	TactileClassificationHist(int dataDim) {
		model = 0;
		svmnode = NULL;
		svmnodelength = dataDim + 1;
		this->dataDim = dataDim;

        normalisation = "normaliseByMax"; //normaliseByMax (divide by max(data) -- range 0 - 1), normaliseByFullRange (divide by 244 -- range 0 - 1), none (raw data -- range 0 - 244)... 
        histCfg.numOfBins = 10;
        histCfg.lowerBound = 0;
        histCfg.upperBound = 1.01;

        h = gsl_histogram_alloc(histCfg.numOfBins);
        gsl_histogram_set_ranges_uniform (h, histCfg.lowerBound, histCfg.upperBound);        

        svmnode = (svm_node *)malloc((histCfg.numOfBins + 1)*sizeof(svm_node));
	
		for (int i = 0; i <= histCfg.numOfBins; i++) {
			svmnode[i].index = i;
			svmnode[i].value = 0;
		}
		svmnode[histCfg.numOfBins].index = -1;
    }

	
	~TactileClassificationHist() {
    	if(svmnode != NULL)
    	{
    		free(svmnode);
    	}

		if (model != 0) {
			svm_free_and_destroy_model(&model);
		}
        gsl_histogram_free (h);
	}
	

    bool customConfiguration(yarp::os::Property &cfg)
    {
        // customise setting for histogram method       
        normalisation = cfg.check("normalisation", yarp::os::Value("normaliseByMax"), "").asString().c_str(); //normaliseByMax, normaliseByFullRange, none

        histCfg.numOfBins = cfg.check("hist_num_bins", yarp::os::Value(10), "Specify number of bins for the histogram").asInt();
        
        if(normalisation == "normaliseByMax")
        {
            histCfg.lowerBound = 0;
            histCfg.upperBound = 1.01;
        }
        else if(normalisation == "normaliseByFullRange")
        {
            histCfg.lowerBound = 0;
            histCfg.upperBound = 1.01;
        }
        else if(normalisation == "none")
        {
            histCfg.lowerBound = 0;
            histCfg.upperBound = 244 + 1;
        }
        else
        {
            // no valid options
            std::cout << "No valid option for configuring histogram" << std::endl;
            return false;
        }
        h = gsl_histogram_alloc(histCfg.numOfBins);
        gsl_histogram_set_ranges_uniform (h, histCfg.lowerBound, histCfg.upperBound);        

        return true;
    }

	bool prepareSVMNode( double *data) {

		gsl_histogram_reset(h);


        svmnode = (svm_node *)realloc(svmnode, (histCfg.numOfBins + 1)*sizeof(svm_node));


		for(int i = 0; i < this->dataDim; i++)
        {
			//std::cout << data[i] << "  ";
			//if(data[i] < 0.3)
			//data[i] = 0;
			gsl_histogram_increment(h, data[i]);
        }
		std::cout << std::endl;
		for (int i = 0; i < histCfg.numOfBins; i++) {
			svmnode[i].index = i + 1;
			svmnode[i].value = gsl_histogram_get(h, i);

			//std::cout << svmnode[i].value << " -- " << i << std::endl;
		}
		std::cout << std::endl;
		svmnode[histCfg.numOfBins].index = -1;

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
        
        if(scale)
        {
	        TactileData newData(this->dataDim);
			this->scaleData(data, newData, 0, 1);
			return this->prepareSVMNode(newData.data()); 
        }
		else
		{
			return this->prepareSVMNode(data.data()); 
		}
	}

private:
    // int numOfBins;
    std::string normalisation;
    histgramSetting histCfg;    
    gsl_histogram * h;
};

#endif //  __TACTILECLASSIFICATIONHIST_H__
