#include "gui/MainFrame.h"
//#include "BehaviourManager.h"

MainFrame::MainFrame(QWidget *parent) : QWidget(parent) {
  
  initLayout(QDir::currentPath() + "/" + "config.ini");
  initConf();
  initPortConnection();
}

MainFrame::MainFrame(QString configfile, QWidget *parent) : QWidget(parent) {

    initLayout(configfile);
    initConf();
    initPortConnection();
}

MainFrame::~MainFrame() {

  this->cmdPort.close();
    // no need to delete, because it is responsible by BehaviourManager
    //    if(bMode) {
    //        delete bMode;
    //    }

  if(keypadButton){ delete keypadButton;}
  if(tactileSenseButton){delete tactileSenseButton;}
  // if(autoRButton){ delete autoRButton;}
  //  if(hybridRButton){ delete hybridRButton;}
  // if(confFileText){ delete confFileText;}
  //if(behFileText){ delete behFileText;}
}

void MainFrame::initConf() {
}

QComboBox *MainFrame::createComboBox(const QString &text) {
    QComboBox *comboBox = new QComboBox;
    comboBox->setEditable(true);
    comboBox->addItem(text);
    comboBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    return comboBox;
}

void MainFrame::initLayout(QString configfile) {
    // radio buttons to choose working mode
    QGroupBox *gbox = createModeRadioButtons();

    // quit button
    QPushButton *quit = new QPushButton(tr("Stop && Quit"));
    quit->setFont(QFont("Times", 14, QFont::Bold));
    
    //QHBoxLayout *confLayout = new QHBoxLayout;
    // QGroupBox *cbox = new QGroupBox(tr("System Setting File"));
    //confFileText = new QLineEdit(configfile);
    //confFileText->setMinimumWidth(texteditwidth);
    //QPushButton *cButton = new QPushButton(tr("Choose File"));

    //connect(cButton, SIGNAL(clicked()), this, SLOT(chooseConfFile()));

    //   QPushButton *setButton = new QPushButton(tr("Set"));
    //connect(setButton, SIGNAL(clicked()), this, SLOT(setConfFile()));
    //confLayout->addWidget(cButton);
    //confLayout->addWidget(setButton);
    //cbox->setLayout(confLayout);

    connect(quit, SIGNAL(clicked()), qApp, SLOT(quit()));

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(gbox, 0, 0, 2, 1);
    //layout->addWidget(cbox, 2, 0, 1, 2);
    //layout->addWidget(behbox, 3, 0, 1, 2);
    //layout->addLayout(batteryBox, 4, 1);
    layout->addWidget(quit, 5, 1);
    setLayout(layout);
}
/*
QGroupBox * MainFrame::createControlPanel() {
    QGroupBox * gbox = new QGroupBox(tr("Behaviour Modes"));
    //userRButton = new QRadioButton(tr("&User Manual Mode"));
    keypadButton = new QCheckBox(tr("&Keypad Enable"));
    QVBoxLayout *vBox = new QVBoxLayout;
    vBox->addWidget(keypadButton);
    vBox->addStretch(1);
    gbox->setLayout(vBox);

    return gbox;
}
*/
void MainFrame::enableKeypadControl(int state)
{
  std::cout << "State Changed (Keypad Control) : " << state << std::endl;
  if(state == Qt::Checked) { 
    if(!yarp::os::Network::isConnected(keypadOutPort.c_str(), actionDecRecPort.c_str()))
      {
	if(yarp::os::Network::connect(keypadOutPort.c_str(), actionDecRecPort.c_str())) {
	  std::cout << "OK " << std::endl;
	}
	else {
	  std::cout << "Could Not connect  " << std::endl;
	  keypadButton->setCheckState(Qt::Unchecked);
	}
      }
  }
  else if(state == Qt::Unchecked)   {
    if(yarp::os::Network::isConnected(keypadOutPort.c_str(), actionDecRecPort.c_str()))
      {
	if(!yarp::os::Network::disconnect(keypadOutPort.c_str(), actionDecRecPort.c_str()))
	  {
	    keypadButton->setCheckState(Qt::Checked);
	  }
      }
  }
}

void MainFrame::enableSkinReaction(int state)
{
  std::cout << "State Changed (Keypad Control) : " << state << std::endl;
  
  if(state == Qt::Checked) {
    bool t = true;
    t = t && yarp::os::Network::connect(skinLeftHandOut.c_str(), actionDecRecPort.c_str());
    t = t && yarp::os::Network::connect(skinRightHandOut.c_str(), actionDecRecPort.c_str());
    t = t && yarp::os::Network::connect(skinLeftArmOut.c_str(), actionDecRecPort.c_str());
    t = t && yarp::os::Network::connect(skinRightArmOut.c_str(), actionDecRecPort.c_str());
    t = t && yarp::os::Network::connect(skinLeftFootOut.c_str(), actionDecRecPort.c_str());
    t = t && yarp::os::Network::connect(skinRightFootOut.c_str(), actionDecRecPort.c_str());
    t = t && yarp::os::Network::connect(skinCheeksOut.c_str(), actionDecRecPort.c_str());
    t = t && yarp::os::Network::connect(skinTorsoOut.c_str(), actionDecRecPort.c_str());
    if(!t)
      {
	std::cout << "Could not connect skin sensors to actionDecion Module" << std::endl;
      }
  }
  else if(state == Qt::Unchecked) {
    bool t = true;
    t = t && yarp::os::Network::disconnect(skinLeftHandOut.c_str(), actionDecRecPort.c_str());
    t = t && yarp::os::Network::disconnect(skinRightHandOut.c_str(), actionDecRecPort.c_str());
    t = t &&  yarp::os::Network::disconnect(skinLeftArmOut.c_str(), actionDecRecPort.c_str());
    t = t &&  yarp::os::Network::disconnect(skinRightArmOut.c_str(), actionDecRecPort.c_str());
    t = t &&  yarp::os::Network::disconnect(skinLeftFootOut.c_str(), actionDecRecPort.c_str());
    t = t &&  yarp::os::Network::disconnect(skinRightFootOut.c_str(), actionDecRecPort.c_str());
    t = t &&  yarp::os::Network::disconnect(skinCheeksOut.c_str(), actionDecRecPort.c_str());
    t = t &&  yarp::os::Network::disconnect(skinTorsoOut.c_str(), actionDecRecPort.c_str());  
   if(!t)
      {
	std::cout << "Could not disconnect skin sensors to actionDecion Module" << std::endl;
      }
  }
 }
QGroupBox * MainFrame::createModeRadioButtons() {
    // groupbox contains radio buttons to select behaviour modes
    QGroupBox * gbox = new QGroupBox(tr("Behaviour Modes"));
    //userRButton = new QRadioButton(tr("&User Manual Mode"));
    keypadButton = new QCheckBox(tr("&Enable Keypad control"));
    tactileSenseButton = new QCheckBox(tr("&Enable Skin Sensing connection"));
    QVBoxLayout *vBox = new QVBoxLayout;
    vBox->addWidget(keypadButton);
    vBox->addWidget(tactileSenseButton);
    vBox->addStretch(1);
    gbox->setLayout(vBox);
    connect(keypadButton, SIGNAL(stateChanged(int )), this, SLOT(enableKeypadControl(int )));
    connect(tactileSenseButton, SIGNAL(stateChanged(int)), this, SLOT(enableSkinReaction(int)));
    /* 
   userRButton = new ModeRadioButton(BehaviourManager::USER_MODE, tr("&User Manual Mode"));
    autoRButton = new ModeRadioButton(BehaviourManager::AUTO_MODE, tr("&Autonomous Mode"));
    hybridRButton = new ModeRadioButton(BehaviourManager::HYBRID_MODE, tr("&Hybrid Mode"));

    QVBoxLayout *vBox = new QVBoxLayout;
    vBox->addWidget(userRButton);
    vBox->addWidget(autoRButton);
    vBox->addWidget(hybridRButton);
    vBox->addStretch(1);
    gbox->setLayout(vBox);

    connect(userRButton, SIGNAL(modeChanged(BehaviourManager::BehaviourModeType)), this, SLOT(setMode(BehaviourManager::BehaviourModeType)));
    connect(autoRButton, SIGNAL(modeChanged(BehaviourManager::BehaviourModeType)), this, SLOT(setMode(BehaviourManager::BehaviourModeType)));
    connect(hybridRButton, SIGNAL(modeChanged(BehaviourManager::BehaviourModeType)), this, SLOT(setMode(BehaviourManager::BehaviourModeType)));
    */
    return gbox;
}

void MainFrame::chooseConfFile()
{
  QString fileName = QFileDialog::getOpenFileName(this, tr("Choose configuration file"), tr(""));
  if(fileName != NULL)
    confFileText->setText(fileName);
}

void MainFrame::chooseBehFile()
{
  QString fileName = QFileDialog::getOpenFileName(this, tr("Choose behaviour file"), tr(""));
  if(fileName != NULL)
    behFileText->setText(fileName);
}

void MainFrame::setConfFile()
{
  //    bManager->initConfiguration((char *)((confFileText->text().toLatin1()).data()));
}

void MainFrame::setBehFile()
{
  behFileText->text();
}

void MainFrame::keyPressEvent(QKeyEvent * event)
{
	yarp::os::Bottle &b = cmdPort.prepare();        // Make a place to store things.
    b.clear();
    b.add("(msg_type keyboard)");
	b.add(event->text().toStdString().c_str());
  std::cout << "Key pressed:   " << event->text().toStdString() << std::endl;
  cmdPort.write();
  /*
  if(event->text().toStdString() == USER_MODE_KEY)
    {
      //	userRButton->setChecked(true);
      return;
    }
  else if(event->text().toStdString() == HYBRID_MODE_KEY)
    {
      //	hybridRButton->setChecked(true);
      return;
    }
  else if(event->text().toStdString() == AUTO_MODE_KEY)
    {
      //	autoRButton->setChecked(true);
      return;
    }
  */
}
