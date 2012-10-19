#ifndef __CONVERT_KASPAR_XML__
#define __CONVERT_KASPAR_XML__

#include "pugixml/pugixml.hpp"
#include "pugixml/pugiconfig.hpp"
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

using namespace std;

void printUsage();

bool makeMetricXML(string source, string xmloutput = "kasparnew.xml", string cfgfile = "kasparconf.cfg");

void makeYarpDeviceConfFromXML(pugi::xml_document *doc, string output);

//double unscalePosition(int maxPos, int minPos, int pos, char *type, double resolution = 1000);
double unscalePosition(double maxPos, double minPos, int pos, char *type, int resolution);

//double unscaleSpeed(int maxSpeed, int minSpeed, int speed, char *type, double resolution = 1000);
double unscaleSpeed(double maxSpeed, double minSpeed, int speed, char *type, int resolution);

bool zjConvertToInt(const pugi::char_t* str, int &t);


double convertToMetricAngle(int pos, string type);

double convertSpeedRPM(int speed, string type);

template <typename T>
std::string to_string(T const& value);



#endif