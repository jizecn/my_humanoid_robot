#include <iostream>
#include "gui/MainFrame.h"

#include <yarp/os/all.h>
#include <yarp/sig/all.h>
#include <yarp/os/RFModule.h>
#include <yarp/os/ResourceFinder.h>
#include <yarp/os/RpcClient.h>

using namespace std;

int main(int argc, char* argv[])
{
  QApplication app(argc, argv);
  
  QString confFile = "D:/roboskin/coding/roboskin/app/KasparRoboSkinConf/config.ini";
  if(argc > 1)
    {
      confFile = argv[1];
    }
  
  cout << "GUI Manager. RoboSkin project. " << endl;
  yarp::os::Network yarp;
  // main function: receives events, and decide action for Kaspar

  MainFrame mf(confFile);
  yarp::os::ResourceFinder rf;
  rf.setVerbose(true);
  rf.configure("KASPAR_ROOT",argc, argv);
  mf.setOutPort(rf.check("keyeventport", yarp::os::Value("/kaspar/keyevent"), "Keyboard event").asString().c_str());

  mf.show();

  return app.exec();
}
