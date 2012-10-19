/*
 * Copyright (C) 2011 Ze Ji
 * University of Hertfordshire. 
 * EU FP7 RoboSkin
 * CopyPolicy: Released under the terms of the GPL v2.0, see GPL.TXT
 *
 */

#ifndef _FAKE_SKIN_H_
#define _FAKE_SKIN_H_

#include <yarp/os/Searchable.h>
#include <yarp/os/Port.h>
#include <yarp/os/Bottle.h>
#include <yarp/os/RateThread.h>
#include <yarp/dev/PolyDriver.h>
#include <yarp/os/Semaphore.h>
#include <yarp/sig/Vector.h>
#include <yarp/os/Random.h>
#include <yarp/os/Time.h>

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

#include "Tokenizer.h"

using namespace yarp::dev;
using namespace yarp::os;
using namespace std;

#define DATA_DIMENSION 192

class FakeSkin: public yarp::os::RateThread {

public:
	enum DataSource {
		RandomData, Recorded
	};

	FakeSkin(const char *portname, int period = 50) :
		yarp::os::RateThread(period) {
		this->portname = portname;
		this->period = period;
		datasource = RandomData;
		currentIndex = 0;
		dataLength = 0;

	}

	~FakeSkin() {
		out.close();
	}

	bool init() {
		if (!bufOut.open(portname.c_str())) {
			return false;
		}
		return true;
	}

	void prepareData(yarp::os::Bottle &b) {
		for (int i = 0; i < DATA_DIMENSION; i++) {
			b.addInt(0);
		}
	}

	bool lineToVector(const std::string &line, yarp::sig::Vector &v) {
		double d;
		int inc = 0;
		Tokenizer s(line);
		v.clear();
		while (inc < this->taxelIds.size() && s.NextToken()) {
			if (this->stringToDouble(s.GetToken(), d)) {
				inc++;
				v.push_back(d);
			}
		}
		if (v.size() != this->taxelIds.size()) {
			v.clear();
			cerr << "WARNING: this line has wrong format of data to load:  "
					<< line << endl;
			return false;
		}
		//cout << line << v.size() << endl;
		return true;
	}

	bool vectorInPlace(const yarp::sig::Vector &v, yarp::sig::Vector &vAll) {
		vAll.resize(DATA_DIMENSION, 0);
		//cout << this->taxelIds.size() << endl;
		for (int i = 0; i < this->taxelIds.size(); i++) {
			//cout << taxelIds[i] << endl;
			vAll[taxelIds[i]] = v[i];
		}
		//cout << this->portname << endl;
		return true;
	}

	void setDataSource(DataSource type) {
		datasource = type;
	}

	// dim should be initialised by setDataIds
	bool loadData(const char *filepath) {
		if (this->taxelIds.size() == 0) {
			cout << "dataDim is not initialized. Run setDataIds first" << endl;
			return false;
		}

		string line;
		ifstream datafile(filepath);

		if (datafile.is_open()) {
			while (datafile.good()) {
				yarp::sig::Vector v;

				yarp::sig::Vector vAll(DATA_DIMENSION);
				vAll.zero();

				getline(datafile, line);

				if (lineToVector(line, v)) {

					if (vectorInPlace(v, vAll)) {
						//cout << line << endl;
						data.push_back(vAll);
					}
				}
			}

			datafile.close();
		}
		if (data.size() == 0) {
			cerr << "ERROR: No data has been loaded." << endl;
			return false;
		}
		this->dataLength = data.size();
		return true;
	}

	void run() {
		mutex.wait();

		if (datasource == RandomData) {
			sendRandData();
		} else if (datasource == Recorded) {
			if (this->data.size() == 0) {
				cerr
						<< "DATA is Empty. Check if you have loaded the data properly"
						<< endl;
				//sendRandData();
				this->stop();
			} else {
				sendRecordedData();
			}
		}
		mutex.post();
	}

	void sendRecordedData() {
		// prepare a message
		// send the message
		yarp::sig::Vector& outBot2 = bufOut.prepare();

		outBot2.resize(DATA_DIMENSION, 100);
		double temp = 0;
		for (int i = 0; i < outBot2.size(); i++) {
			temp = (this->data[currentIndex])[i];
			outBot2[i] = temp;
		}

		bufOut.write();

		currentIndex = ++currentIndex % dataLength;
	}

	void sendRandData() {
		// prepare a message
		// send the message
		yarp::sig::Vector& outBot2 = bufOut.prepare();

		outBot2.resize(DATA_DIMENSION, 100);

		for (int i = 0; i < outBot2.size(); i++) {
			yarp::os::Random r;
			outBot2[i] = r.uniform();
		}

		bufOut.write();
	}

	bool setDataIds(string file) {
		yarp::os::Property config;
		config.fromConfigFile(file.c_str());
		yarp::os::Bottle ids = config.findGroup("ids").tail();
		cout << file.c_str() << endl;

		int s = ids.size();
		for (int i = 0; i < s; i++) {
			int si;
			istringstream buffer(ids.get(i).toString().c_str());
			buffer >> si;
			//string sis = ids.get(i).toString().c_str();
			for (int j = 0; j < 12; j++) {
				//cout << sis.c_str() << " --- " << i << "  " << s << "  " << si << "  "<< si*12 + j << endl;
				taxelIds.push_back(si * 12 + j);
			}
		}

		//this->dataDim = taxelIds.size();
		return true;
	}

private:
	yarp::os::Port out;
	yarp::os::BufferedPort<yarp::sig::Vector> bufOut;

	yarp::os::Semaphore mutex;
	yarp::os::Bottle bot;
	yarp::sig::Vector pBot;

	std::string portname;
	int period;
	std::vector<yarp::sig::Vector> data;
	DataSource datasource;

	int currentIndex;
	int dataLength;

	vector<int> taxelIds;

	bool stringToDouble(const std::string& s, double &x) {
		std::istringstream i(s);

		if (!(i >> x))
			return false;
		return true;
	}
};

#endif // _FAKE_SKIN_H_
