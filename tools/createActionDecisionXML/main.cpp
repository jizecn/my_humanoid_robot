//#include "convertKasparXML.h"
#include <iostream>
#include <sstream>
#include <string>
#include "createKeymapXML.h"

using namespace std;

void printUsage()
{
  printf("Usage: convertKasparXML source.map -o outputname(metric). Extensions of .xml will be added automatically, and stored in folder ./output. \n");
}

int main(int argc, char *argv[])
{
  printf("\n\n ======== INFORMATION =========\n");
  printf("This program is to convert the original KASPARGui keymap file into XML, together with more attributes\n");
  printf(" ===== END OF INFORMATION ======\n\n\n");
  
  if(argc == 1)
    {
      printf("Please provide the source filename. \n");
      printUsage();
      return -1;
    }
  string filename = argv[1];
  cout << "File: " << filename << endl;
  string xmlfile = "KasparRoboSkin.xml";
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
      cout << "Output XML file is ==> " << xmlfile << endl;
    }
  
  // OK. start the program
  //makeMetricXML(filename, xmlfile);
  //makeYarpDeviceConfFromXML(pugi::xml_document *doc);
  
  return 0;
}
