#include "convertKasparXML.h"

void printUsage()
{
	printf("Usage: convertKasparXML source.xml -o outputname(metric). Extensions of .xml and .cfg will be added automatically, and stored in folder ./output. \n");
}

bool makeMetricXML(string source, string xmloutput, string cfgfile)
{
    int counter = 0;
	printf("\n\nMake the XML file using real metric angle and speed units\n\n");
	pugi::xml_document doc;

	pugi::xml_parse_result result = doc.load_file(source.c_str());

    if(result)
        std::cout << "XML [" << source << "] parsed without errors, KASPAR/version attr value: [" << doc.child("KASPAR").attribute("version").value() << "]\n\n";
    else
    {
        std::cout << "XML [" << source << "] parsed with errors \n";
        std::cout << "Error description: " << result.description() << "\n";
        std::cout << "Error offset: " << result.offset << " (error at [..." << (source.c_str() + result.offset) << "]\n\n";
        return false;
    }
    pugi::xml_node servos = doc.child("KASPAR").child("SERVOLIST");
    for(pugi::xml_node servo = servos.child("SERVO"); servo; servo = servo.next_sibling("SERVO"))
    {

        //pugi::string_t type = servo.attribute("type").value();
        if(strcmp(servo.attribute("type").value(), "AX12") == 0)
        {
            cout << "SERVO: type:" << servo.attribute("type").value() << endl;
            cout << " id:" << servo.attribute("id").value() << endl;
            cout << servo.child_value("NAME") << endl;

            if(servo.attribute("id_yarp"))
            {
                cout << "PLEASE check if the file being edited is a converted version\n";
                return false;
            }
            servo.append_attribute("id_yarp").set_value(counter);
            counter++;

            int min = 0;
            int max = 0;
            double minMetric = 0;
            double maxMetric = 0;
            //pugi::xml_node limits = servo.child("LIMITS");
            pugi::xml_node limits = servo.find_child_by_attribute("LIMITS", "type", "pos");
			//if(strcmp(limits.attribute("type").value(), "pos") == 0)
            if(limits)
			{
                if(zjConvertToInt(limits.child_value("MIN"), min) && zjConvertToInt(limits.child_value("MAX"), max) )
                {
                    minMetric = convertToMetricAngle(min, "AX12");
                    maxMetric = convertToMetricAngle(max, "AX12");
                    cout << minMetric << "   " << maxMetric << endl;
                    pugi::xml_node temp = limits.append_child();
                    temp.set_name("MIN_METRIC");
                    temp.append_child(pugi::node_pcdata).set_value(to_string(minMetric).c_str());
                    pugi::xml_node temp1 = limits.append_child();
                    temp1.set_name("MAX_METRIC");
                    temp1.append_child(pugi::node_pcdata).set_value(to_string(maxMetric).c_str());
                }
                else
                {
                    cout << "Error with MIN and MAX values\n";
                    return false;
                }
            }
            pugi::xml_node default_setting = servo.child("DEFAULT");
            int pos = 0;
            double posMetric = 0;
            int speed = 0;
            double speedMetric = 0;
            if(zjConvertToInt(default_setting.child_value("POS"), pos))
            {
                posMetric = unscalePosition(maxMetric, minMetric, pos, "AX12", 1000);
                cout << "Position:   " << posMetric << "   " << pos << endl;
                pugi::xml_node posMetricNode = default_setting.append_child();
                posMetricNode.set_name("POS_METRIC");
                posMetricNode.append_child(pugi::node_pcdata).set_value(to_string(posMetric).c_str());
            }
            else
            {
                cout << "Error with POS value\n";
                return false;
            }
            if(zjConvertToInt(default_setting.child_value("SPEED"), speed))
            {
                speedMetric = unscaleSpeed(convertSpeedRPM(1023, "AX12"), convertSpeedRPM(0, "AX12"), speed, "AX12", 1000);
                cout << "Speed:   " << speedMetric << "   " << speed << endl;
                pugi::xml_node speedMetricNode = default_setting.append_child();
                speedMetricNode.set_name("SPEED_METRIC");
                speedMetricNode.append_child(pugi::node_pcdata).set_value(to_string(speedMetric).c_str());
            }
            else
            {
                cout << "Error with SPEED value\n";
                return false;
            }


        }
        else if(strcmp(servo.attribute("type").value(), "MINISSC") == 0)
        {
            cout << "SERVO: type:" << servo.attribute("type").value() << endl;
			const char *id = servo.attribute("id").value();
            cout << " id:" << id << endl;
            cout << servo.child_value("NAME") << endl;

            if(servo.attribute("id_yarp"))
            {
                cout << "PLEASE check if the file being edited is a converted version\n";
                return false;
            }
            servo.append_attribute("id_yarp").set_value(counter);
            counter++;

            int min = 0;
            int max = 0;
            double minMetric = 0;
            double maxMetric = 0;
            pugi::xml_node limits = servo.child("LIMITS");
            if(strcmp(limits.attribute("type").value(), "pos") == 0)
            {
                if(zjConvertToInt(limits.child_value("MIN"), min) && zjConvertToInt(limits.child_value("MAX"), max) )
                {
					int tmpId = 0;
					if(!zjConvertToInt(id, tmpId))
					{
						return false;
					}
					else
					{
						if(tmpId%16 < 8)
						{
							minMetric = convertToMetricAngle(min, "MINISSC_LOWER");
							maxMetric = convertToMetricAngle(max, "MINISSC_LOWER");
						}
						else if(tmpId%16 >= 8)
						{
							minMetric = convertToMetricAngle(min, "MINISSC_HIGHER");
							maxMetric = convertToMetricAngle(max, "MINISSC_HIGHER");
						}
					}
                    cout << minMetric << "   " << maxMetric << endl;
                    pugi::xml_node temp = limits.append_child();
                    temp.set_name("MIN_METRIC");
                    temp.append_child(pugi::node_pcdata).set_value(to_string(minMetric).c_str());
                    pugi::xml_node temp1 = limits.append_child();
                    temp1.set_name("MAX_METRIC");
                    temp1.append_child(pugi::node_pcdata).set_value(to_string(maxMetric).c_str());
                }
                else
                {
                    cout << "Error with MIN and MAX values\n";
                    return false;
                }
            }


            pugi::xml_node speedlimits = limits.next_sibling();//servo.find_child_by_attribute("LIMITS");
            //double speed = 0;
            double minSpeed = 0;
            double maxSpeed = 0;
            if(strcmp(speedlimits.attribute("type").value(), "speed") == 0)
            {
                if(zjConvertToInt(speedlimits.child_value("MIN"), min) && zjConvertToInt(speedlimits.child_value("MAX"), max) )
                {
                    minSpeed = convertSpeedRPM(min, "MINISSC");
                    maxSpeed = convertSpeedRPM(max, "MINISSC");
                    cout << minSpeed << "  = " << maxSpeed << endl;
                    pugi::xml_node temp = speedlimits.append_child();
                    temp.set_name("MIN_METRIC");
                    temp.append_child(pugi::node_pcdata).set_value(to_string(minSpeed).c_str());
                    pugi::xml_node temp1 = speedlimits.append_child();
                    temp1.set_name("MAX_METRIC");
                    temp1.append_child(pugi::node_pcdata).set_value(to_string(maxSpeed).c_str());
                }
                else
                {
                    cout << "Error with MIN and MAX values\n";
                    return false;
                }
            }
			
			pugi::xml_node default_setting = servo.child("DEFAULT");
            int pos = 0;
            double posMetric = 0;
            int speed = 0;
            double speedMetric = 0;
            if(default_setting.child_value("POS") && zjConvertToInt(default_setting.child_value("POS"), pos))
            {
                posMetric = unscalePosition(maxMetric, minMetric, pos, "MINISSC", 1000);
                cout << "Position:   " << posMetric << "   " << pos << endl;
                pugi::xml_node posMetricNode = default_setting.append_child();
                posMetricNode.set_name("POS_METRIC");
                posMetricNode.append_child(pugi::node_pcdata).set_value(to_string(posMetric).c_str());
            }
            else
            {
                cout << "Error with POS value\n";
                return false;
            }
            if(zjConvertToInt(default_setting.child_value("SPEED"), speed))
            {
                speedMetric = unscaleSpeed(convertSpeedRPM(maxSpeed, "MINISSC"), convertSpeedRPM(minSpeed, "MINISSC"), speed, "MINISSC", 1000);
                cout << "Speed:   " << speedMetric << "   " << speed << endl;
                pugi::xml_node speedMetricNode = default_setting.append_child();
                speedMetricNode.set_name("SPEED_METRIC");
                speedMetricNode.append_child(pugi::node_pcdata).set_value(to_string(speedMetric).c_str());
            }
            else
            {
                cout << "Error with SPEED value\n";
                //return false;
                speedMetric = unscaleSpeed(convertSpeedRPM(255, "MINISSC"), convertSpeedRPM(0, "MINISSC"), speed, "MINISSC", 1000);
                cout << "Speed:   " << speedMetric << "   " << speed << endl;
                pugi::xml_node speedMetricNode = default_setting.append_child();
                speedMetricNode.set_name("SPEED_METRIC");
                speedMetricNode.append_child(pugi::node_pcdata).set_value(to_string(speedMetric).c_str());

			}



        }
        else if(strcmp(servo.attribute("type").value(), "SSC32") == 0)
        {
            cout << "SERVO: type:" << servo.attribute("type").value() << endl;
            cout << " id:" << servo.attribute("id").value() << endl;


            servo.append_attribute("id_yarp").set_value(counter);
            counter++;
            int min = 0;
            int max = 0;
            double minMetric = 0;
            double maxMetric = 0;
            pugi::xml_node limits = servo.child("LIMITS");
            if(strcmp(limits.attribute("type").value(), "pos") == 0)
            {
                if(zjConvertToInt(limits.child_value("MIN"), min) && zjConvertToInt(limits.child_value("MAX"), max) )
                {
                    minMetric = convertToMetricAngle(min, "SSC32");
                    maxMetric = convertToMetricAngle(max, "SSC32");
                    cout << minMetric << "   " << maxMetric << endl;
                    pugi::xml_node temp = limits.append_child();
                    temp.set_name("MIN_METRIC");
                    temp.append_child(pugi::node_pcdata).set_value(to_string(minMetric).c_str());
                    pugi::xml_node temp1 = limits.append_child();
                    temp1.set_name("MAX_METRIC");
                    temp1.append_child(pugi::node_pcdata).set_value(to_string(maxMetric).c_str());
                }
                else
                {
                    cout << "Error with MIN and MAX values\n";
                    return false;
                }
            }

            pugi::xml_node speedlimits = limits.next_sibling();//servo.find_child_by_attribute("LIMITS");
            //double speed = 0;
            double minSpeed = 0;
            double maxSpeed = 0;
            if(strcmp(speedlimits.attribute("type").value(), "speed") == 0)
            {
                if(zjConvertToInt(speedlimits.child_value("MIN"), min) && zjConvertToInt(speedlimits.child_value("MAX"), max) )
                {
                    minSpeed = convertSpeedRPM(min, "SSC32");
                    maxSpeed = convertSpeedRPM(max, "SSC32");
                    cout << minSpeed << "  = " << maxSpeed << endl;
                    pugi::xml_node temp = speedlimits.append_child();
                    temp.set_name("MIN_METRIC");
                    temp.append_child(pugi::node_pcdata).set_value(to_string(minSpeed).c_str());
                    pugi::xml_node temp1 = speedlimits.append_child();
                    temp1.set_name("MAX_METRIC");
                    temp1.append_child(pugi::node_pcdata).set_value(to_string(maxSpeed).c_str());
                }
                else
                {
                    cout << "Error with MIN and MAX values\n";
                    return false;
                }
            }

            pugi::xml_node default_setting = servo.child("DEFAULT");
            int pos = 0;
            double posMetric = 0;
            int speed = 0;
            double speedMetric = 0;
            if(zjConvertToInt(default_setting.child_value("POS"), pos))
            {
                posMetric = unscalePosition(maxMetric, minMetric, pos, "SSC32", 1000);
                cout << "Position:   " << posMetric << "   " << pos << endl;
                pugi::xml_node posMetricNode = default_setting.append_child();
                posMetricNode.set_name("POS_METRIC");
                posMetricNode.append_child(pugi::node_pcdata).set_value(to_string(posMetric).c_str());
            }
            else
            {
                cout << "Error with POS value\n";
                return false;
            }
            if(zjConvertToInt(default_setting.child_value("SPEED"), speed))
            {
                speedMetric = unscaleSpeed(convertSpeedRPM(maxSpeed, "SSC32"), convertSpeedRPM(minSpeed, "SSC32"), speed, "SSC32", 1000);
                cout << "Speed:   " << speedMetric << "   " << speed << endl;
                pugi::xml_node speedMetricNode = default_setting.append_child();
                speedMetricNode.set_name("SPEED_METRIC");
                speedMetricNode.append_child(pugi::node_pcdata).set_value(to_string(speedMetric).c_str());
            }
            else
            {
                cout << "Error with SPEED value\n";
                return false;
            }


        }
        else if(strcmp(servo.attribute("type").value(), "POLOLU") == 0)
        {
            cout << "SERVO: type:" << servo.attribute("type").value() << endl;
            cout << " id:" << servo.attribute("id").value() << endl;

            servo.append_attribute("id_yarp").set_value(counter);
            counter++;

        }
        else
        {
            cout << "WARNING: UNKNOWN SERVO type ( " << servo.attribute("type").value() << endl;
        }
    }
	doc.save_file(xmloutput.c_str());
    makeYarpDeviceConfFromXML(&doc, cfgfile);
    return true;
}

void makeYarpDeviceConfFromXML(pugi::xml_document *doc, string output)
{
	pugi::xml_node root = doc->root();
	ofstream myfile;
	myfile.open (output.c_str());
	myfile << "# This file is converted from original XMl file.\n";
	myfile << "# The COM setting may need to be adjusted.\n";
	myfile << "# author: Ze ji, University of Hertfordshire\n";

	myfile << "\n";
	myfile << "comport COM11\n";
	myfile << "baudrate 9600\n";

	myfile << "\n";
	myfile << "\n";

	myfile << "[servos]\n";
    pugi::xml_node servos = doc->child("KASPAR").child("SERVOLIST");
    for(pugi::xml_node servo = servos.child("SERVO"); servo; servo = servo.next_sibling("SERVO"))
    {
		myfile << servo.attribute("id_yarp").value() << " " << servo.attribute("type").value() << " " << servo.attribute("id").value() << " ";
		//myfile << servo.child
		
		pugi::xml_node limits = servo.find_child_by_attribute("LIMITS", "type", "pos");

		string minMetric; 
		string maxMetric;
		string minSpeed;
		string maxSpeed;
		minMetric = limits.child_value("MIN_METRIC");
		maxMetric = limits.child_value("MAX_METRIC");
		myfile << maxMetric << " " << minMetric << endl;;
	
	}
	myfile.close();
}

double convertToMetricAngle(int pos, string type)
{
    double range = 0.0;
    int lower_bound = 0;
    int higher_bound = 0;
    if(type == "AX12")
    {
        range = 300;
        higher_bound = 1023;
    }
    else if(type == "MINISSC_HIGHER")
    {
        range = 180;
        higher_bound = 254;
		printf("HERE\n");
		printf("%f %f\n", (double)pos, (double)lower_bound);
	}
    else if(type == "MINISSC_LOWER")
    {
        range = 90;
        higher_bound = 254;
		printf("HERE\n");
    }
	else if(type == "SSC32")
    {
        range = 180;
        lower_bound = 500;
        higher_bound = 2500; //????
    }
    else if(type == "POLOLU")
    {
        range = 180;
        higher_bound = 1023; //?????
    }
    else{
        return -1;
    }

    return ((double)(pos-lower_bound)/(double)(higher_bound - lower_bound))*range;
}

double convertSpeedRPM(int speed, string type)
{
    double range = 0.0;
    int lower_bound = 0;
    int higher_bound = 0;
    if(type == "AX12")
    {
        if (speed == 0)
            speed = 1;
        lower_bound = 0;
        range = 114;
        higher_bound = 1023;
        return ((double)speed/(double)(higher_bound - lower_bound))*range;
    }
    else if(type == "MINISSC")
    {
        return (double)speed;  // no implementation. MINISSC does not speed option.
        //range = 180;
        //resolution_end = 255;
    }
    else if(type == "SSC32")
    {
        return (double)speed;  // no implementation. Depends on individual motors
    }
    else if(type == "POLOLU")
    {
        return (double)speed;  // no implementation. Depends on individual motors
    }
    else{
        return -1;
    }

}


/*
maxPos minPos are not in metric unit, they are the values for the servo protocols
pos is the scaled value, or normalised value, ranging from 0 - 1000
*/
double unscalePosition(double maxPos, double minPos, int pos, char *type, int resolution)
{
	double unscaledPos = 0;
	double l = maxPos - minPos;
    cout << "MAX MIN -=-- " << maxPos << "  " << minPos << "   " << pos << endl;
	//ACE_OS::printf("max %d - min %d: %d\n",this->servo.getMaxPos(), this->servo.getMinPos(), l);
	unscaledPos = l * (double)pos / (double)resolution + minPos;	
	
	return unscaledPos;
    //ACE_OS::printf("unscaledPos is : %f\n", this->unscaledPos);
}
/*
maxSpeed minSpeed are not in metric unit, they are the values for the servo protocols
speed is the scaled value, or normalised value, ranging from 0 - 1000
*/
double unscaleSpeed(double maxSpeed, double minSpeed, int speed, char *type, int resolution)
{
	double unscaledSpeed = 0;
	double l = maxSpeed - minSpeed;
    cout << "MAX SPEED --- " << maxSpeed << "   " << minSpeed << endl;
	unscaledSpeed = l * (double)speed / (double)resolution + minSpeed;

//	double unscaledSpeed = (temp / 1023.0) * 114.0;
    return unscaledSpeed;
    //ACE_OS::printf("unscaledSpeed is : %f\n", this->unscaledSpeed);
}

bool zjConvertToInt(const pugi::char_t* str, int &t) {
    //int t = 0;
    int i = sscanf(str, "%d", &t);
    //printf(">>> %d\n", t);
    return i != EOF;
}

template <typename T>
std::string to_string(T const& value) {
    stringstream sstr;
    sstr << value;
    return sstr.str();
}
