#ifndef __MAIN_FRAME_H__
#define __MAIN_FRAME_H__

#include <QApplication>
#include <QFont>
#include <QLCDNumber>
#include <QPushButton>
#include <QSlider>
#include <QVBoxLayout>
#include <QWidget>
#include <QGroupBox>
#include <QRadioButton>
#include <QLabel>
#include <QCheckBox>
#include <QComboBox>
#include <QDir>
#include <QGridLayout>
#include <QFileDialog>
#include <QLineEdit>
#include <QKeyEvent>
#include <yarp/os/all.h>
#include <yarp/sig/all.h>
#include <yarp/os/RFModule.h>
#include <yarp/os/ResourceFinder.h>
#include <yarp/os/RpcClient.h>
#include <iostream>

//#include "ModeRadioButton.h"

#define USER_MODE_KEY ")"
#define HYBRID_MODE_KEY "("
#define AUTO_MODE_KEY "TO_DEFINE_LATER"

class MainFrame : public QWidget {
    Q_OBJECT

public:
    MainFrame(QWidget *parent = 0);
    MainFrame(QString configfile, QWidget *parent = 0);
    ~MainFrame();
    void initConf();

    //public slots:
    //void setMode(BehaviourManager::BehaviourModeType);
	void setOutPort(std::string keyEventPort)
	{
		this->keyEventPortName = keyEventPort;
		cmdPort.open(keyEventPortName.c_str());
	}

private slots:
    void chooseConfFile();
    void chooseBehFile();
    void setConfFile();
    void setBehFile();
    void keyPressEvent(QKeyEvent * event);
    void enableKeypadControl(int state);
    void enableSkinReaction(int state);
signals:

private:

    yarp::os::BufferedPort<yarp::os::Bottle> cmdPort;
    QCheckBox *keypadButton;
    QCheckBox *tactileSenseButton;
    QComboBox *createComboBox(const QString &text);
    void initLayout(QString configfile);
    QGroupBox * createModeRadioButtons();
    //QGroupBox * createControlPanel();
    // BehaviourManager *bManager;
    // BehaviourMode *bMode;
    //UserMode *uMode;
    //APFMode *aMode;
    //AutonomousMode *auMode;
    //QRadioButton *userRButton;    
    //  ModeRadioButton *userRButton;
    //ModeRadioButton *autoRButton;
    //ModeRadioButton *hybridRButton;
    
    QComboBox *directoryComboBox;
    QLineEdit *confFileText;
    QLineEdit *behFileText;
    
    static const int texteditwidth = 300;
    std::string keyEventPortName;

    
    std::string keypadOutPort;
    std::string actionDecRecPort;
    std::string skinLeftHandOut;
    std::string skinRightHandOut;
    std::string skinLeftArmOut;
    std::string skinRightArmOut;
    std::string skinLeftFootOut;
    std::string skinRightFootOut;
    std::string skinCheeksOut;
    std::string skinTorsoOut;

    void initPortConnection()
    {
      keypadOutPort = "/kaspar/keyevent";
      actionDecRecPort = "/worker";
      skinLeftHandOut = "/tactileclassification/lefthand";
      skinRightHandOut = "/tactileclassification/righthand";
      skinLeftArmOut = "/tactileclassification/leftarm";
      skinRightArmOut = "/tactileclassification/rightarm";
      skinLeftFootOut = "/tactileclassification/leftfoot";
      skinRightFootOut = "/tactileclassification/rightfoot";
      skinCheeksOut = "/tactileclassification/cheeks";
      skinTorsoOut = "/tactileclassification/torso";
    }
};

#endif
