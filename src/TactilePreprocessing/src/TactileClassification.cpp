#include "math/TactileClassification.h"

using namespace std;
using namespace yarp::os;

/*
 TactileClassification::TactileClassification()
 {
 model = 0;
 }
 */
/*
 TactileClassification::~TactileClassification()
 {
 if(model != 0)
 {
 svm_free_and_destroy_model(&model);
 }
 }
 */
/*
bool TactileClassification::initConfiguration(const char *confFile)
{
    Property config;
	if (!config.fromConfigFile(confFile))
	{
		cerr
				<< "ERROR: Configuratio initialisation failed. (TactileClassification)"
				<< endl;
		return false;
	}

	yarp::os::ConstString path =
			config.check("svm_model_path", yarp::os::Value(""),
					"specify file path for libsvm model file").asString();
	yarp::os::ConstString
			svmmethod =
					config.check(
							"svm_method",
							yarp::os::Value("RBF"),
							"Speficy the svm method (the kernel type) for classification -- RBF, HISTOGRAM, etc").asString();
	yarp::os::ConstString svmmodelfile = config.check("svm_model_file",
			yarp::os::Value(""), "Specify the svm model file").asString();

	// load svm model file
	string fullmodelpath = path.c_str();
	if (path == "")
	{
		cout << "WARNING: path not set. use current location/n";
		fullmodelpath = "";
	}
	else
	{
		fullmodelpath = fullmodelpath + "/";
	}
	fullmodelpath = fullmodelpath + svmmodelfile.c_str();
	if (!this->loadSVMModel(fullmodelpath.c_str()))
	{
		return false;
	}
	
	return true;
}
*/


int TactileClassification::performClassification()
{
	//svmnode
	//for(int i = 0; i < 20; i ++)
	//{
	//	cout << svmnode[i].index << "   " << svmnode[i].value << "  ::::   " ;
	//}
	//cout << endl;
	return TactileClassification::classify(this->svmnode, this->model, 0);
}

int TactileClassification::classify(svm_node *svmnode, svm_model *model,
		int predict_probability)
{
	//int correct = 0;
	//int total = 0;
	//double error = 0;
	//double sump = 0, sumt = 0, sumpp = 0, sumtt = 0, sumpt = 0;

	int svm_type = svm_get_svm_type(model);
	int nr_class = svm_get_nr_class(model);

	double *prob_estimates = NULL;
	int j;
	double predict_label;

	if (predict_probability)
	{
		if (svm_type == NU_SVR || svm_type == EPSILON_SVR)
		{
			printf(
					"Prob. model for test data: target value = predicted value + z,\nz: Laplace distribution e^(-|z|/sigma)/(2sigma),sigma=%g\n",
					svm_get_svr_probability(model));
		}
		else
		{
			int *labels = (int *) malloc(nr_class * sizeof(int));
			svm_get_labels(model, labels);

			prob_estimates = (double *) malloc(nr_class * sizeof(double));

			printf("labels");
			for (j = 0; j < nr_class; j++)
			{
				printf(" %d", labels[j]);
			}
			printf("\n");

			free(labels);
		}
	}

	if (predict_probability && (svm_type == C_SVC || svm_type == NU_SVC))
	{
		predict_label = svm_predict_probability(model, svmnode, prob_estimates);
		printf("%g", predict_label);
		//fprintf(output,"%g",predict_label);
		for (j = 0; j < nr_class; j++)
		{
			printf(" %g", prob_estimates[j]);
		}
		printf("\n");
	}
	else
	{
		predict_label = svm_predict(model, svmnode);
		printf("%g \n", predict_label);
		//std::cout << model->param.kernel_type << "  " <<model->label[0] << "   " << model->label[1] << endl;
	}

	return predict_label;
}

bool TactileClassification::loadSVMModel(const char *modelFileName)
{
	model = svm_load_model(modelFileName);
	if (model == 0)
	{
		cerr << "can't open model file " << modelFileName << endl;
		return false;
	}
	else
	{
		//cout << model->
	}
	return true;
}

bool TactileClassification::scaleData(TactileData &tactileData, TactileData &scaledData, double lowerBound, double higherBound)
{
    int size = tactileData.length();
    if(size != scaledData.length())
    {
        scaledData.resize(size);
        cout << "It is recommended to initialise the size of scaledData in advance. Here resized to " << size << endl;
    }

    double range = higherBound - lowerBound;
    if(range <= 0)
    {
        cerr << "ERROR: higherBound should be greater than lowerBound" << endl;
        return false;
    }

    double mi = tactileData.min();
    //double ma = tactileData.max();
	//cout << "Min is => " << mi << ". And Max is => " << ma << std::endl;
    /*
	ma = ma - mi;
    for (int i = 0; i < size; i++)
    {
        scaledData[i] = tactileData[i] - mi;
        scaledData[i] = range*scaledData[i]/ma;   // this is why ma = ma - mi above
        scaledData[i] += lowerBound;
    }*/
	for(int i = 0; i < size; i++)
	{
        scaledData[i] = tactileData[i] - mi;
	} 

    //double nmi = scaledData.min();
    double nma = scaledData.max();
	//cout << "Min is => " << nmi << ". And Max is => " << nma << std::endl;
	for(int i = 0; i < size; i++)
	{
        scaledData[i] = range*scaledData[i]/nma;   // this is why ma = ma - mi above
        scaledData[i] += lowerBound;
	}
    double nmi = scaledData.min();
    nma = scaledData.max();
	cout << "Min is => " << nmi << ". And Max is => " << nma << std::endl;
	
	return true;
}
