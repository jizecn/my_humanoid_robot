/*
 * Servos.cpp
 *
 *  Created on: 8 Jul 2010
 *      Author: ze
 */
#include <iostream>
#include <sstream>
#include <string>

#include "Servos.h"

Servos::Servos()
{
}

Servos::~Servos()
{
	for(int j = 0; j < this->groups.size(); j++)
	{
		if(groups[j])
		{
			delete groups[j];
		}
	}
}

Servo * Servos::findFromList(std::string name)
{
	for (int i = 0; i < this->servoList.size(); i++ )
	{
		if(this->servoList[i].getName() ==name)
		{
			return &(this->servoList[i]);
		}
	}
		for(int j = 0; j < this->groups.size(); j++)
	{
		if(groups[j])
		{
			delete groups[j];
		}
	}
   return NULL;
}

bool Servos::add(Servo sv)
{
	servoList.push_back(sv);

	//**servosByName should be initialised first....***//
	std::map<std::string, Servo *>::iterator iter = servosByName.find(sv.getName());
	if (iter != servosByName.end())
	{
		servosByName.erase(iter); // rarely happen. but just in case, and a good practice
	}
	//servosByName[sv.getName()] =  &sv;
	servosByName[sv.getName()] =  findFromList(sv.getName());
	
	std::map<int, Servo *>::iterator iter1 = servosById.find(sv.getYarpId());
	if (iter1 != servosById.end())
	{
		servosById.erase(iter1); // rarely happen. but just in case, and a good practice
	}

	//servosById[sv.getId()] = &sv;
	servosById[sv.getYarpId()] = servosByName[sv.getName()];

	return true;
}

bool Servos::getServo(std::string name, Servo &sv)
{
	std::map<std::string, Servo *>::iterator it;
	it = servosByName.find(name);
	if(it == servosByName.end())
	{
		printf("Key %s has no associated value\n", name.c_str());
		return false;
	}

	sv = *(it->second);

	return true;
}

Servo *Servos::getServo(std::string name)
{
	std::map<std::string, Servo *>::iterator it;
	it = servosByName.find(name);
	if(it == servosByName.end())
	{
		printf("Key %s has no associated value\n", name.c_str());
		return NULL;
	}

	// TODO: WHY DOES IT NOT WORK ANYMORE... IT USED TO BE WORKING... 
	//return it->second;    
	return findFromList(name);
}

/*Use YarpID. Hardware ID can be duplicate, because of different types of servo used.*/
Servo *Servos::getServo(int yarpId)
{
	std::map<int, Servo *>::iterator it;
	it = servosById.find(yarpId);
	if(it == servosById.end())
	{
		printf("Key %d has no associated value\n", yarpId);
		return NULL;
	}

	return it->second;
}

int Servos::getNumServos()
{
	return servoList.size();
}

KasparFile::RetCode Servos::initFromXMLFile(const char *filepath,const char *filename)
{
	std::string fullpath = string(filepath) + FILESEPARATOR + string(filename);
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(fullpath.c_str());

    if(result)
        std::cout << "XML No error\n";
    else
    {
        std::cout << "XML parsing error :: " << result.description() << std::endl;;
        return KasparFile::FileFormatError;
    }
    return initFromXML(&doc);

    return KasparFile::OK;
}

KasparFile::RetCode Servos::initFromXML(pugi::xml_document *doc)
{
	KasparFile::RetCode ret = KasparFile::OK;

	ret = this->initServosFromXML(doc);
	if(ret != KasparFile::OK)
	{
		fprintf(stderr, "Error in Servo initialisation\n");
		return ret;
	}
	
	ret = this->initGroupsFromXML(doc);
	if(ret != KasparFile::OK)
	{
		fprintf(stderr, "Error in ServoGroup initialisation\n");
		return ret;
	}
	return ret;
}

KasparFile::RetCode Servos::initServosFromXML(pugi::xml_document *doc)
{
	pugi::xml_node root = doc->root();
    pugi::xml_node servos = doc->child("KASPAR").child("SERVOLIST");
    for(pugi::xml_node servo = servos.child("SERVO"); servo; servo = servo.next_sibling("SERVO"))
    {
        std::string name = servo.child_value("NAME");
		int id = servo.attribute("id").as_int();
		int id_yarp = servo.attribute("id_yarp").as_int();
		Servo::Type type;
		std::string _type = servo.attribute("type").value();
		if(_type == "AX12")
		{
			type = Servo::AX12;
		}
		else if(_type == "MINISSC")
		{
			type = Servo::MINISSC;
		}
		else if(_type == "SSC32")
		{
			type = Servo::SSC32;
		}
		else if(_type == "POLOLU")
		{
			type = Servo::POLOLU;
		}
		else
		{
			return KasparFile::FileFormatError;
		}

		pugi::xml_node limits = servo.find_child_by_attribute("LIMITS", "type", "pos");

		std::string _minMetric; 
		std::string _maxMetric;
		std::string _minSpeed = "1";
		std::string _maxSpeed = "114";
        double minMetric = 0;
        double maxMetric = 300;
        double minSpeed = 1;
        double maxSpeed = 114;
		_minMetric = limits.child_value("MIN_METRIC");
		_maxMetric = limits.child_value("MAX_METRIC");
        
        std::istringstream stm;
        stm.str(_minMetric);
        stm >> minMetric;
        stm.clear();
        stm.str(_maxMetric);
        stm >> maxMetric;
        stm.clear();

        cout << _maxMetric << "   "  << maxMetric << "   " << minMetric << "\n";

        //printf(">>> %d\n", t);
        //myfile << maxMetric << " " << minMetric << endl;;
		pugi::xml_node speedlimits = servo.find_child_by_attribute("LIMITS", "type", "speed");

        if(speedlimits)
        {
            _minSpeed = speedlimits.child_value("MIN_METRIC");
            _maxSpeed = speedlimits.child_value("MAX_METRIC");
        }
        else
        {
            if( type != Servo::AX12 && type != Servo::MINISSC)
            {
                printf("No MAXSPEED and MINSPEED information, and default values are not available too. Cannot proceed any more. \nPlease check the file\n");
                return KasparFile::FileFormatError;
            }
        }
        stm.str(_minSpeed);
        stm >> minSpeed;
        stm.clear();

        stm.str(_maxSpeed);
        stm >> maxSpeed;
        stm.clear();

        // default setting
        pugi::xml_node default_setting = servo.child("DEFAULT");
        if(!default_setting)
        {
            printf("ERROR No default setting found\n");
            return KasparFile::FileFormatError;
        }
        double posMetric = 150;
        double speedMetric = 1;
        
        stm.str(default_setting.child_value("POS_METRIC"));
        stm >> posMetric; 
        stm.clear();

        stm.str(default_setting.child_value("SPEED_METRIC"));
        stm >> speedMetric;
        stm.clear();
	    Servo sv(name, type, id, id_yarp, maxMetric, minMetric, posMetric, maxSpeed, minSpeed,speedMetric); 
        this->add(sv);
		//cout << ">>>> " << sv.toString() << servosByName[sv.getName()]->getName() << endl;
		//  	Servo1(string name, Type type, int hardware_id, int yarp_id, double maxPos, double minPos, double defaultPos, double maxSpeed, double minSpeed, double defaultSpeed)
	}
	cout << "Number of servos:   " << this->servoList.size() << endl;
	return KasparFile::OK;
}

KasparFile::RetCode Servos::initGroupsFromXML(pugi::xml_document *doc)
{
	pugi::xml_node root = doc->root();
	pugi::xml_node servos = doc->child("KASPAR").child("SERVOLIST");
    for(pugi::xml_node servoGroup = servos.child("SERVOGROUP"); servoGroup; servoGroup = servoGroup.next_sibling("SERVOGROUP"))
    {
		ServoGroup *sg = new ServoGroup(); 
        std::string name = servoGroup.child_value("NAME");
		sg->setName(name);
		for(pugi::xml_node member = servoGroup.child("MEMBER"); member; member = member.next_sibling("MEMBER"))
		{
			std::string memberName = member.child_value();
			
            Servo *sv = NULL;
            sv = this->getServo(memberName);
            if(sv != NULL)
            {
                sg->addServo(sv);
                
                std::cout << "SERVO ADDED " << memberName << "  "  << sv->getName() << std::endl;
				sv->setGroup(sg);
			}
		}
        std::string compliantEnable = servoGroup.child_value("COMPLIANT_ENABLE");
        if(compliantEnable == "true")
        {
            sg->enableCompliantMode(true);
        }
        else if (compliantEnable == "false")
        {
            sg->enableCompliantMode(false);
        }
        else
        {
            std::cerr << "Compliant_enable format error. Disable compliant by default here\n";
            sg->enableCompliantMode(false);
        }
		this->groups.push_back(sg);
	}

#ifdef _DEBUG
	// test
	for (int i = 0; i < this->groups.size(); i++)
	{
		std::cout << "GROUP  " << i << " IS " << groups[i]->getName() << std::endl;
		for(int j = 0; j < groups[i]->numOfServos(); j++)
		{
            std::cout << j << " " << groups[i]->getServo(j)->getName() <<  "   "   << std::endl;
		}
	}
#endif
	return KasparFile::OK;
}

