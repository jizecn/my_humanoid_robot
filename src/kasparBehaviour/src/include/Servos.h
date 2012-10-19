/*
 * Servos.h
 *
 *  Created on: 8 Jul 2010
 *      Author: ze
 */

#ifndef SERVOS_H_
#define SERVOS_H_
#include <ace/OS_NS_stdio.h>
#include <ace/Vector_T.h>
#include <ace/SString.h>

#include "Servo.h"

#include "utils/Resources.h"
#include "utils/NumberFormatException.h"
#include "utils/Utils.h"
#include "pugixml/pugixml.hpp"
#include <map>
#include <string>
#include <vector>
#include <iostream>
using namespace std;

class Servos
{
public:
	Servos();
	~Servos();

	bool getServo(string name, Servo &sv);
	//bool getServo(int id, Servo *sv);
	bool add(Servo sv);
	int getNumServos(); 
    Servo *getServo(string name);
	Servo *getServo(int yarpId);
    /*
	Old version.deprecated. Use initFromXML or initFromXMLFile where appropriate
	*/
    KasparFile::RetCode initFromFile(const char *filepath, const char *filename);

	KasparFile::RetCode initFromXMLFile(const char *filepath,const char *filename);
	KasparFile::RetCode initFromXML(pugi::xml_document *doc);
private:
	map<string, Servo *> servosByName;
	map<int, Servo *> servosById;
	ACE_Vector<Servo> servoList;
	//vector<Servo> servoList;
	Servo * findFromList(string name);

	std::vector<ServoGroup*> groups;
	
	KasparFile::RetCode initGroupsFromXML(pugi::xml_document *doc);
	KasparFile::RetCode initServosFromXML(pugi::xml_document *doc);
};

#endif /* SERVOS_H_ */
