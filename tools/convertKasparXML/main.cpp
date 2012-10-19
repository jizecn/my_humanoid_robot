#include "convertKasparXML.h"
#include <iostream>
#include <sstream>
#include <string>
using namespace std;

int main(int argc, char *argv[])
{
	printf("\n\n ======== INFORMATION =========\n");
	printf("This program is to convert the original XML conf file used by the Kaspar's Java program to\n");
	printf("--> a new XML file that uses metric units, such as position in angles (0-360) and speed (in RPM). \n");
	printf("--> another driver configuration file that is used by the KasparServos's driver. It also uses metric units, such as position in angles (0-360) and speed (in RPM). \n\n");
	printf("Both new files will introduce a new ID, because YARP motor driver does not accept Servo Names as the ID to access the drivers, and the IDs for Kaspar can be duplicated because of the use of different types of motors\n\n");
	printf("To use thses files, copy the XML file (by default: KasparRoboSkin.xml) for the Application. And Yarp driver config file should be used to provide properties for the Driver KasparServos.\n");
	printf(" ===== END OF INFORMATION ======\n\n\n");

	if(argc == 1)
	{
		printf("Please provide the XML source filename. \n");
		printUsage();
		return -1;
	}
	string filename = argv[1];
	cout << "File: " << filename << endl;
	string xmlfile = "KasparRoboSkin.xml";
	string cfgfile = "KasparRoboSkin.cfg";
	if(argc != 4)
	{
		printUsage();
		printf("In this case, default filenames are used, KasparRoboSkin.xml and KasparRoboSkin.cfg will be created, and stored in folder ./output. \n");
	}
	else 
	{
		if(strcmp(argv[2], "-o") != 0)
		{
			printUsage();
			return -1;
		}
		xmlfile = string(argv[3]) + ".xml";
		cfgfile = string(argv[3]) + ".cfg";
		cout << "Output XML file is ==> " << xmlfile << endl;
		cout << "Output CFG file is ==> " << cfgfile << endl;
	}
	
	// OK. start the program
	makeMetricXML(filename, xmlfile, cfgfile);
	//makeYarpDeviceConfFromXML(pugi::xml_document *doc);
	
	return 0;
}