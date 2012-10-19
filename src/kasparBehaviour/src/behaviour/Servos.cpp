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

}

Servo * Servos::findFromList(string name)
{
	for (int i = 0; i < this->servoList.size(); i++ )
	{
		if(this->servoList[i].getName().compare(name) == 0)
		{
			return &this->servoList[i];
		}
	}
	for(int j = 0; j < this->groups.size(); j++)
	{
		if(groups[j])
		{
			delete groups[j];
		}
	}
}

bool Servos::add(Servo sv)
{
	servoList.push_back(sv);

	//**servosByName should be initialised first....***//
	map<string, Servo *>::iterator iter = servosByName.find(sv.getName());
	if (iter != servosByName.end())
	{
		servosByName.erase(iter); // rarely happen. but just in case, and a good practice
	}
	//servosByName[sv.getName()] =  &sv;
	servosByName[sv.getName()] =  findFromList(sv.getName());
	map<int, Servo *>::iterator iter1 = servosById.find(sv.getYarpId());
	if (iter1 != servosById.end())
	{
		servosById.erase(iter1); // rarely happen. but just in case, and a good practice
	}

	//servosById[sv.getId()] = &sv;
	servosById[sv.getYarpId()] = servosByName[sv.getName()];
	return true;
}

bool Servos::getServo(string name, Servo &sv)
{
	map<string, Servo *>::iterator it;
	it = servosByName.find(name);
	if(it == servosByName.end())
	{
		ACE_OS::printf("Key %s has no associated value\n", name.c_str());
		return false;
	}

	sv = *(it->second);

	return true;
}

Servo *Servos::getServo(string name)
{
	map<string, Servo *>::iterator it;
	it = servosByName.find(name);
	if(it == servosByName.end())
	{
		ACE_OS::printf("Key %s has no associated value\n", name.c_str());
		return NULL;
	}

	return it->second;
}

/*Use YarpID. Hardware ID can be duplicate, because of different types of servo used.*/
Servo *Servos::getServo(int yarpId)
{
	map<int, Servo *>::iterator it;
	it = servosById.find(yarpId);
	if(it == servosById.end())
	{
		ACE_OS::printf("Key %d has no associated value\n", yarpId);
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
	string fullpath = string(filepath) + FILESEPARATOR + string(filename);
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(fullpath.c_str());

    if(result)
        std::cout << "XML No error\n";
    else
    {
        std::cout << "XML parsing error :: " << result.description() << endl;;
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
        string name = servo.child_value("NAME");
		int id = servo.attribute("id").as_int();
		int id_yarp = servo.attribute("id_yarp").as_int();
		Servo::Type type;
		string _type = servo.attribute("type").value();
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

		string _minMetric; 
		string _maxMetric;
		string _minSpeed = "1";
		string _maxSpeed = "114";
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
        cout << sv.toString();
		//  	Servo1(string name, Type type, int hardware_id, int yarp_id, double maxPos, double minPos, double defaultPos, double maxSpeed, double minSpeed, double defaultSpeed)
	}
	return KasparFile::OK;
}

KasparFile::RetCode Servos::initGroupsFromXML(pugi::xml_document *doc)
{
	pugi::xml_node root = doc->root();
	pugi::xml_node servos = doc->child("KASPAR").child("SERVOLIST");
    for(pugi::xml_node servoGroup = servos.child("SERVOGROUP"); servoGroup; servoGroup = servoGroup.next_sibling("SERVOGROUP"))
    {
		ServoGroup *sg = new ServoGroup(); 
        string name = servoGroup.child_value("NAME");
		sg->setName(name);
		for(pugi::xml_node member = servoGroup.child("MEMBER"); member; member = member.next_sibling("MEMBER"))
		{
			string memberName = member.child_value();
			
            Servo *sv = NULL;
            sv = this->getServo(memberName);
            if(sv != NULL)
            {
                sg->addServo(sv);
                
                std::cout << "SERVO ADDED " << memberName << "  "  << sv->getName() << std::endl;
				sv->setGroup(sg);
			}
		}
        string compliantEnable = servoGroup.child_value("COMPLIANT_ENABLE");
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
            std::cout << j << " " << groups[i]->getServo(j)->getName() <<  "   "   << endl;
		}
	}
#endif
	return KasparFile::OK;
}

/*
KasparFile::RetCode Servos::initFromFile(const char *filepath,const char *filename)
{
    //Servos servos; 
    FILE *file = ACE_OS::fopen((string(filepath) + FILESEPARATOR + filename).c_str(), "r");

    if (!file) {
        ACE_OS::fprintf(stderr, "Failed to open configuration file %s", filename);
        return KasparFile::FileIOError;
    }

    int ret = 0;
    // how long? 100 is enough for here
    char buf[150]; // = (char *)malloc(sizeof(char)*100);
    char *tmpTok;
    int i = 0;

    do {
        char tmpBuf[150];
        if (fgets(buf, 150, file) == NULL) {
            break;
        }
        strcpy(tmpBuf, buf);
        zjStripNewline(tmpBuf);
        ACE_OS::printf("READ %d LINE: %s\n", i + 1, tmpBuf);
        if (strlen(tmpBuf) == 0) {
            ACE_OS::fprintf(stderr, "String length is 0\n");
            continue;
        } else if (tmpBuf[0] == ';') {
            continue;
        }

        // process buf

        //;<NAME>,<SERVO_TYPE>,<minPos>,<maxPos>,<defaultPos>,<minSpeed>,<maxSpeed>,<defaultSpeed>
        //;
        //; Dynamixels (AX12) HEAD_ROT,AX12,101,200,740,500,0,1023,150
        // content part
        // string tokenizer ......
        ACE_Tokenizer tok(tmpBuf);
        tok.delimiter_replace(',', 0);

        tmpTok = tok.next();
		
        if (!tmpTok) 
		{
			ACE_OS::fprintf(stderr, "Servo name format is incorrect\n");
			return KasparFile::FileFormatError;
		}
        string name = tmpTok;

        tmpTok = tok.next();
		
        if (!tmpTok)
		{
			ACE_OS::fprintf(stderr, "Servo type format is incorrect\n");
			return KasparFile::FileFormatError;
		}
        Servo::Type type;
        if(strcmp(tmpTok, "AX12") == 0)
        {
        	type = Servo::AX12;
        }
        else if(strcmp(tmpTok, "SSC32") == 0)
        {
        	type = Servo::SSC32;
        }

        int id;
        tmpTok = tok.next();
	
        if (!tmpTok)
		{
			ACE_OS::fprintf(stderr, "Servo ID format is incorrect\n");
			return KasparFile::FileFormatError;
		}
		if(!zjConvertToInt(tmpTok, id))
		{
			return KasparFile::NumberFormatError;
		}

		int minPos;
        tmpTok = tok.next();

        if (!tmpTok)
		{
			ACE_OS::fprintf(stderr, "Servo minPos format is incorrect\n");
			return KasparFile::FileFormatError;
		}
		
		if( !zjConvertToInt(tmpTok, minPos))
		{
			ACE_OS::printf("MINPOS IS %d\n", minPos);
			return KasparFile::NumberFormatError;
		}
        int maxPos;
        tmpTok = tok.next();
        if (!tmpTok)
		{
			ACE_OS::fprintf(stderr, "Servo maxPos format is incorrect\n");
			return KasparFile::FileFormatError;
		}
		if(!zjConvertToInt(tmpTok, maxPos))
		{
			return KasparFile::NumberFormatError;
		}

        int defaultPos;
        tmpTok = tok.next();
		
		if (!tmpTok)
		{
			ACE_OS::fprintf(stderr, "Servo defaultPos format is incorrect\n");
			return KasparFile::FileFormatError;
		}
		if( !zjConvertToInt(tmpTok, defaultPos))
		{
			return KasparFile::NumberFormatError;
		}

        int minSpeed;
        tmpTok = tok.next();
					
        if (!tmpTok)
		{
			ACE_OS::fprintf(stderr, "Servo minSpeed format is incorrect\n");
			return KasparFile::FileFormatError;
		}
		if(!zjConvertToInt(tmpTok, minSpeed))
		{
			return KasparFile::NumberFormatError;
		}

        int maxSpeed;
        tmpTok = tok.next();
							
        if (!tmpTok)
		{ 
			ACE_OS::fprintf(stderr, "Servo maxSpeed format is incorrect\n");
			return KasparFile::FileFormatError;
		}
		if(!zjConvertToInt(tmpTok, maxSpeed))
		{
			return KasparFile::NumberFormatError;
		}
        int defaultSpeed;
        tmpTok = tok.next();
							
        if (!tmpTok)
		{
			ACE_OS::fprintf(stderr, "Servo defaultSpeed format is incorrect\n");
			return KasparFile::FileFormatError;
		}
		if(!zjConvertToInt(tmpTok, defaultSpeed))
		{
			return KasparFile::NumberFormatError;
		}

        Servo sv(name, type, id, maxPos, minPos, defaultPos, maxSpeed, minSpeed, defaultSpeed);
		int size = this->getNumServos();
		sv.setYarpId(size);  // the index id is the location in the servo list. Thus, here to initialise, just increment the number 
		ACE_OS::printf("servo %s created\n", name.c_str());
        this->add(sv);

        i++; // line number increments

    } while (true);

    if (ferror(file) || !feof(file)) {
        ACE_OS::fprintf(stderr, "Failed to read configuration file %s", filename);
		return KasparFile::FileIOError;
    }
    ret = ACE_OS::fclose(file);
    if (ret == EOF && ret != 0) {
        ACE_OS::fprintf(stderr, "Failed to close configuration file %s", filename);
		return KasparFile::FileIOError;
    } else {
        // no problem
    }
    return KasparFile::OK;
}*/