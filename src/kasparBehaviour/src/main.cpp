#include <ace/OS_NS_stdio.h>
#include <ace/Vector_T.h>
#include <yarp/os/Network.h>
#include <yarp/os/Port.h>
#include <yarp/os/Bottle.h>
#include <yarp/os/Time.h>
#include <stdio.h>
#include <stdlib.h>
#include <yarp/dev/PolyDriver.h>
#include <yarp/dev/ControlBoardInterfaces.h>

#include "BehaviourManager.h"
#include "BehaviourMode.h"
#include "UserMode.h"

#include "gui/MainFrame.h"

// IMPORTANT
// TO BE ABLE TO FIND KASPAR Devices
#ifdef USE_KASPAR_MOD
#include "drivers.h"
#endif

using namespace yarp::os;
using namespace yarp::dev;

int main(int argc, char **argv) {
    ACE_OS::printf("... To start Kaspar GUI demo ...\n");

    //Network yarp;

    // IMPORTANT
    // TO BE ABLE TO FIND KASPAR Devices
#ifdef USE_KASPAR_MOD
    yarp::dev::DriverCollection dev;
#endif
/*
    Property config;

    // this line should be re-written
    // config.fromConfigFile("kaspar.ini");

    config.fromCommand(argc, argv);
    //config.fromConfigFile(config.find("file").asString());
    config.fromConfigFile(config.check("file", yarp::os::Value("kaspar.ini"), "specify conf file --file kaspar.ini").asString().c_str());
*/
    /*CAUTION::::: device here should be remote_controlboard, rather than dynamixelAX12FtdiDriver*/
    //http://wikiri.upc.es/index.php/YARP_devices
    //http://eris.liralab.it/wiki/YARP_Motor_Control
    //http://eris.liralab.it/yarpdoc/d1/d88/classyarp_1_1dev_1_1RemoteControlBoard.html
    //PolyDriver dd("(device remote_controlboard) (local /client) (remote /controlboard)");
    //PolyDriver dd("(device dynamixelAX12FtdiDriver)");
    /*
    PolyDriver dd;
    dd.open(config);

    if (!dd.isValid()) {
        printf("Device not available.\n");
        Network::fini();
        return 1;
    }

    // to manage what task karpar should respond to
    BehaviourManager *bm = new BehaviourManager();
    bm->initMotorDriver(&dd);
    BehaviourMode bmode;
    UserMode um;
    bmode = (BehaviourMode) um;
    bmode.start();
    */
    QApplication app(argc, argv);

	QString confFile = "D:/roboskin/coding/roboskin/app/KasparRoboSkinConf/config.ini";
	if(argc > 1)
	{
		confFile = argv[1];
	}

    MainFrame mf(confFile);
    mf.show();

    //  dd.close();
    return app.exec();
    // return 0;
}
