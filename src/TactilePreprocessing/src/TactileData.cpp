#include "TactileData.h"

using namespace std;

TactileData::TactileData() : yarp::sig::Vector()
{
    this->defaultThresholdMethod.method = TactileData::thres_max;
    this->defaultThresholdMethod.threshold = THRESHOLD_MAX;
    //svmnodelength = this->size() + 1;
    //svmnode = (struct svm_node *)malloc(svmnodelength*sizeof(svm_node));

}

TactileData::TactileData(size_t dim) : yarp::sig::Vector(dim)
{
    this->defaultThresholdMethod.method = TactileData::thres_max;
    this->defaultThresholdMethod.threshold = THRESHOLD_MAX;
    //svmnodelength = this->size() + 1;
    //svmnode = (struct svm_node *)malloc(svmnodelength*sizeof(svm_node));
}

TactileData::TactileData(const yarp::sig::Vector &vec) : yarp::sig::Vector(vec)
{
    this->defaultThresholdMethod.method = TactileData::thres_max;
    this->defaultThresholdMethod.threshold = THRESHOLD_MAX;
    //svmnodelength = this->size() + 1;
    //svmnode = (struct svm_node *)malloc(svmnodelength*sizeof(svm_node));
}

bool TactileData::assignData(double *sensorData, const size_t offset, const size_t size, bool useCalibrate)
{
    if (size == 0)
    {
        cout << "ERROR: size is zero" << endl;
        return false;
    }
	
    if (offset + size > this->size())
    {
        cout << "ERROR: size exceeds the limit" << endl;
        return false;
    }

    for( int i = 0; i < size; i++)
    {
		//cout << "...... >>>> " << i << endl;
        // change it to memcpy would be more efficient... do later
		if(useCalibrate)
		{			
			//if(sensorData[i] > THRESHOLD_MAX)
			//cout << " ----" << sensorData[i] << endl;
			(*this)[offset + i] = sensorData[i]; 
		}
		else
		{
			(*this)[offset + i] = sensorData[i]<=244?double(244-sensorData[i]):0.0; 
		}
//#ifdef DEBUG	 
//        if(i % 10 == 1)
//            cout << sensorData[i] << " " ;
//#endif
    }

    return true;
}
  
// if wrong, return vector of 0 length
yarp::sig::Vector TactileData::diff(TactileData &d1, TactileData &d2, bool ifAbs)
{
    //check if dimensions of two DataSample match (though it will be always ;))
    if(d1.size() != d2.size())
    {
        cout << "Dimensions do not match:   " << d1.size() << "   "  << d2.size() << endl;
        return yarp::sig::Vector(0);
    }

    yarp::sig::Vector result(d1.size());

    if(ifAbs)
    {
        for(int i = 0; i < d1.size(); i++)
        {
            result[i] = std::fabs(d1[i] - d2[i]);
        }
    }
    else
    {
        for(int i = 0; i < d1.size(); i++)
        {
            result[i] = d1[i] - d2[i];
        }
    }
    
    return result;
}

/* Both ds and result should be of the same dimension */
bool TactileData::diff(TactileData &ds, yarp::sig::Vector &result, bool ifAbs)
{
	double *data = this->data();
	int size = this->size();
	//check if dimensions of two DataSample match (though it will be always ;) )
	if(ds.size() != size)
	{
		cout << "Dimensions do not match:   " << ds.size() << "   "  << size << endl;
		return false;
	}
	
	if(result.size() != size)
	{
		cout << "WARNING. Dimension of result vector is different. Reset it" << endl;
		result.resize(size);
	}
		
	if(ifAbs)
	{
		for(int i = 0; i < size; i++)
		{
			result[i] = std::fabs(data[i] - ds[i]);
			//result.push_back(this->data[i] - ds[i]);
		}
	}
	else
	{
		for(int i = 0; i < size; i++)
		{
			result[i] = data[i] - ds[i];
			//result.push_back(this->data[i] - ds[i]);
		}
	}
	return true;
}
/*
int TactileData::performClassification()
{
    return this->classify(&this->model, 0);
}
*/
/*
int TactileData::classify(svm_model *model, int predict_probability)
{
	int correct = 0;
	int total = 0;
	double error = 0;
	double sump = 0, sumt = 0, sumpp = 0, sumtt = 0, sumpt = 0;

	int svm_type=svm_get_svm_type(model);
	int nr_class=svm_get_nr_class(model);
	double *prob_estimates=NULL;
	int j;
	double predict_label;

	if(predict_probability)
	{
		if (svm_type==NU_SVR || svm_type==EPSILON_SVR)
		{
			printf("Prob. model for test data: target value = predicted value + z,\nz: Laplace distribution e^(-|z|/sigma)/(2sigma),sigma=%g\n",svm_get_svr_probability(model));
		}
		else
		{
			int *labels=(int *) malloc(nr_class*sizeof(int));
			svm_get_labels(model,labels);
			prob_estimates = (double *) malloc(nr_class*sizeof(double));
			
			printf("labels");
			for(j = 0; j < nr_class; j++)
			{
				printf(" %d", labels[j]);
			}
			printf("\n");
			//fprintf(output,"labels");		
			//for(j=0;j<nr_class;j++)
			//	fprintf(output," %d",labels[j]);
			//fprintf(output,"\n");
			free(labels);
		}
	}

	if (predict_probability && (svm_type==C_SVC || svm_type==NU_SVC))
	{
		predict_label = svm_predict_probability(model,svmnode,prob_estimates);
		printf("%g", predict_label);
		//fprintf(output,"%g",predict_label);
		for(j=0;j<nr_class;j++)
		{
			printf(" %g", prob_estimates[j]);
			//fprintf(output," %g",prob_estimates[j]);
		}
		printf("\n");
		//		fprintf(output,"\n");
	}
	else
	{
		predict_label = svm_predict(model,svmnode);
		printf("%g\n", predict_label);
		//fprintf(output,"%g\n",predict_label);
	}
	
	// double predict_label = svm_predict(model, this->svmnode);
	// printf("%g\n", predict_label);

	return predict_label;
}
*/
/*
bool TactileData::loadSVMModel(const char *modelFileName)
{
    svm_model *tempmodel = svm_load_model(modelFileName);   
	if(tempmodel == 0)
    {
        cerr << "can't open model file " << modelFileName << endl;
        return false;
    }
    this->model = *tempmodel;
    svm_free_and_destroy_model(&tempmodel);

    return false;
}
*/
double TactileData::max()
{
    
  gsl_vector *gv = (gsl_vector*)getGslVector();
  double m = gsl_vector_max(gv);
  return m;
    /*
    double *data = this->data();
    double temp = data[0];
    for (int i = 1; i < this->size(); i++)
    {
        if(data[i] > temp)
        {
            //TODO: have not used swap() method before... better to test
            swap(data[i], temp);
        }
    }
    return temp;
*/
}

double TactileData::min()
{
    
      gsl_vector *gv = (gsl_vector*)getGslVector();
      double m = gsl_vector_min(gv);
      return m;
    
	/*
    double *data = this->data();
    double temp = data[0];
    for (int i = 1; i < this->size(); i++)
    {
        if(data[i] < temp)
        {
            //TODO: have not used swap() method before... better to test
            swap(data[i], temp);
        }
    }
    return temp;
	*/
}


double TactileData::sum()
{
  double temp = 0;
  double *d = data();
  for (int i = 0; i < this->size(); i++)
    {
      temp += d[i];
    }
  return temp;
}
void TactileData::setThreshold(threshold_method_setting thresMethodStruct)
{
  this->defaultThresholdMethod = thresMethodStruct;
  //   enum threshold_method {thres_max, thres_sum, thres_counting};
}

bool TactileData::aboveThreshold()
{
  // think of using pointers to functions. Why not working??? :(
    switch(defaultThresholdMethod.method)
    {
    case TactileData::thres_max:
        return aboveThresholdMax();
    case TactileData::thres_sum:
        return aboveThresholdSum();
    case TactileData::thres_counting:
        return aboveThresholdCounting();
    default:
        // something wrong with the options. 
        cerr << "ERROR: no appropriate method for calculating threshold " << endl;   
        break;
    }
}

bool TactileData::aboveThresholdMax()
{
  double m = this->max();
  //if(m > this->defaultThresholdMethod.threshold)
  //std::cout << "MAX is ===>  " << m << std::endl;
  return m > this->defaultThresholdMethod.threshold ? true : false;
}

bool TactileData::aboveThresholdSum()
{
  double m = this->sum();
  return m > this->defaultThresholdMethod.threshold ? true : false;
}

bool TactileData::aboveThresholdCounting()
{
  int total = this->size();
  int count = 0;
  double *d = this->data();
  for (int i = 0; i < total; i++)
    {
      count +=  d[i] > THRESHOLD_COUNTING_FORCE ? 1 : 0;
    }
  return count > THRESHOLD_COUNTING_NUM_TAXEL ? true : false;
}

/*
bool TactileData::updateSVMNode()
{
	if(svmnodelength < this->size() + 1)
	{
		// rare to happen in this case, as it is updated when creating the object in constructor
		svmnodelength = this->size() + 1;
		free(svmnode);
		svmnode = (struct svm_node *)malloc(svmnodelength*sizeof(svm_node));	
		// could also use realloc(svmnode, svmnodelength*sizeof(svm_node));
	}
	return prepareSVMNode(svmnode, this->data(), svmnodelength);
}

svm_node *TactileData::getSVMNode()
{
	return this->svmnode;
}

bool TactileData::prepareSVMNode(svm_node *svmnode, double *data, int length)
{
	int i = 0;
	for(i = 0; i < length; i++)
	{
		svmnode[i].index = i;
		svmnode[i].value = data[i];
	}
	svmnode[i].index = -1;
	return true;
}
*/
