#include "DataSample.h"

using namespace std;
//using namespace yarp::os;

DataSample::DataSample(const unsigned int dim)
{
	//this->data = NULL;
	//  this->dim = 0;
	if(dim < 1)
	{
		cerr << "WARNING: size allocated too small for DataSample\n";
	}
	this->dim = dim;
	this->data.resize(dim, 0);
	//this->init(dim);
	
  //cout << "This Object is not initialised here. Call init(dim) before use" << endl;
}


DataSample::~DataSample()
{
	// TODO:::: PROBLEM HERE (only in Debug mode, with iCubInterface. Using fakeSkin is Ok)..... WHY?????? 
 // cout << "to delete data\n";
/*
	if (data)
	{
	   delete[] data;
	}
	*/
//	cout << "OK delete data\n";
}

/*
bool DataSample::init(int dim)
{
  //if (data)
  //{
//	  delete[] data;
  //}

  this->dim = dim;
//  this->data = new (nothrow)double[dim];
  data.resize(dim);

  ////if(data == 0)
  ////{
////	  cout << "Error: memory could not be allocated" << endl;
////	  return false;
 //// }

  for(int i = 0; i < dim; i++)
  {
		data[i] = 0;
  }

  return true;
}
*/

unsigned int DataSample::getDim()
{
	return this->dim;
}

bool DataSample::assignData(double *sensorData, const unsigned int offset, const unsigned int size)
{
	if (size == 0)
	{
		cerr << "" << endl;
		return false;
	}
	
	if (offset + size > this->dim)
	{
		cout << "" << endl;
		return false;
	}

	for( int i = 0; i < size; i++)
	{
		// change it to memcpy would be more efficient... do later
		this->data[offset + i] = sensorData[i]; 
#ifdef DEBUG	 
		if(i % 10 == 1)
			cout << sensorData[i] << " " ;
#endif
	}
	
	return true;
}

double& DataSample::operator[](int i)
{
	return this->data[i];
}

bool DataSample::diff(DataSample &ds, std::vector<double> &result, bool ifAbs)
{
	//check if dimensions of two DataSample match (though it will be always ;) )
	if(ds.dim != this->dim)
	{
		cout << "Dimensions do not match:   " << ds.dim << "   "  << this->dim << endl;
		return false;
	}

	if(result.size() != this->dim)
	{
		cout << "WARNING. Dimension of result vector is different. Reset it" << endl;
		result.resize(this->dim);
	}

	if(ifAbs)
	{
		for(int i = 0; i < this->dim; i++)
		{
			result[i] = std::fabs(this->data[i] - ds[i]);
			//result.push_back(this->data[i] - ds[i]);
		}
	}
	else
	{
		for(int i = 0; i < this->dim; i++)
		{
			result[i] = this->data[i] - ds[i];
			//result.push_back(this->data[i] - ds[i]);
		}
	}
	return true;
}

int DataSample::classify(svm_model *model)
{
	
	return 0;
}

svm_model *DataSample::loadSVMModel(const char *modelFileName)
{
	//struct svm_model *svm_load_model(const char *model_file_name);
	return svm_load_model(modelFileName);
	//	svm_model sm;
	//return sm;
}
