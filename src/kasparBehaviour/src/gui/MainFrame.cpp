#include "gui/MainFrame.h"
#include "BehaviourManager.h"

MainFrame::MainFrame(QWidget *parent) : QWidget(parent) {

	initLayout(QDir::currentPath() + "/" + "config.ini");
    initConf();
}

MainFrame::MainFrame(QString configfile, QWidget *parent) : QWidget(parent) {

    initLayout(configfile);
    initConf();
}

MainFrame::~MainFrame() {

    if(bManager){
        delete bManager;
    }

    // no need to delete, because it is responsible by BehaviourManager
    //    if(bMode) {
    //        delete bMode;
    //    }

    if(userRButton){ delete userRButton;}
    if(autoRButton){ delete autoRButton;}
    if(hybridRButton){ delete hybridRButton;}
    if(confFileText){ delete confFileText;}
    if(behFileText){ delete behFileText;}
}

void MainFrame::initConf() {
    bManager = new BehaviourManager();
    
    userRButton->setChecked(true);
    if(!bManager->setBehaviourMode(userRButton->getModeNum()))
	//if(!bManager->setBehaviourMode(BehaviourManager::USER_MODE))
	{
        ACE_OS::fprintf(stderr, "Something wrong with setBehaviourMode\n");
    }
    bMode = NULL;
        
    //bMode = bManager->getCurrentMode();
	bManager->getCurrentMode(bMode);
    if(!bMode)
    {
        ACE_OS::fprintf(stderr, "BehaviourMode object is NULL\n");
    }
    else
    {
        bMode->prepare("1");
    }
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

    QHBoxLayout *batteryBox = new QHBoxLayout;
    QLabel *label = new QLabel(tr("Battery Voltage"));
    QLCDNumber *batteryLcd = new QLCDNumber(2);
    batteryLcd->setSegmentStyle(QLCDNumber::Filled);
    batteryBox->addWidget(label);
    batteryBox->addWidget(batteryLcd);

    //QSlider *slider = new QSlider(Qt::Horizontal);
    //slider->setRange(0, 99);
    //slider->setValue(0);

    QHBoxLayout *confLayout = new QHBoxLayout;
    QGroupBox *cbox = new QGroupBox(tr("System Setting File"));
    //directoryComboBox = createComboBox(QDir::currentPath());
    confFileText = new QLineEdit(configfile);
    confFileText->setMinimumWidth(texteditwidth);
    QPushButton *cButton = new QPushButton(tr("Choose File"));

    connect(cButton, SIGNAL(clicked()), this, SLOT(chooseConfFile()));

    QPushButton *setButton = new QPushButton(tr("Set"));
    connect(setButton, SIGNAL(clicked()), this, SLOT(setConfFile()));
    //confLayout->addWidget(directoryComboBox);
    confLayout->addWidget(confFileText);
    confLayout->addWidget(cButton);
    confLayout->addWidget(setButton);
    cbox->setLayout(confLayout);

    QHBoxLayout *behLayout = new QHBoxLayout;
    QGroupBox *behbox = new QGroupBox(tr("Behaviour Conf File"));

    //QComboBox *directoryComboBox1 = createComboBox(QDir::currentPath());
    behFileText = new QLineEdit(QDir::currentPath());
    behFileText->setMinimumWidth(texteditwidth);
    QPushButton *cButton1 = new QPushButton(tr("Choose File"));
    
    connect(cButton1, SIGNAL(clicked()), this, SLOT(chooseBehFile()));

    QPushButton *setButton1 = new QPushButton(tr("Set"));
    //behLayout->addWidget(directoryComboBox1);
    behLayout->addWidget(behFileText);
    behLayout->addWidget(cButton1);
    behLayout->addWidget(setButton1);
    behbox->setLayout(behLayout);


    connect(quit, SIGNAL(clicked()), qApp, SLOT(quit()));
    //connect(slider, SIGNAL(valueChanged(int)), batteryLcd, SLOT(display(int)));

    /*
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(gbox);
    layout->addLayout(batteryBox);

    layout->addWidget(slider);
    layout->addWidget(cbox);
    layout->addWidget(quit);
    setLayout(layout);
     */
    QGridLayout *layout = new QGridLayout;
    layout->addWidget(gbox, 0, 0, 2, 1);
    layout->addWidget(cbox, 2, 0, 1, 2);
    //layout->addWidget(behbox, 3, 0, 1, 2);
    //layout->addLayout(batteryBox, 4, 1);
    layout->addWidget(quit, 5, 1);
    setLayout(layout);
}

QGroupBox * MainFrame::createModeRadioButtons() {
    // groupbox contains radio buttons to select behaviour modes
    QGroupBox * gbox = new QGroupBox(tr("Behaviour Modes"));
    //QRadioButton *userRButton = new QRadioButton(tr("&User Manual Mode"));
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

    return gbox;
}

void MainFrame::setMode(BehaviourManager::BehaviourModeType m) {
    //ACE_OS::printf("Behaviour Mode %d is selected\n", m);
    bManager->setBehaviourMode(m);
    
    //bManager->getCurrentMode(*bMode);
    //bManager->currentMode->prepare();
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
    bManager->initConfiguration((char *)((confFileText->text().toLatin1()).data()));
}

void MainFrame::setBehFile()
{
    behFileText->text();
}

void MainFrame::keyPressEvent(QKeyEvent * event)
{
	if(event->text().toStdString() == USER_MODE_KEY)
	{
		bManager->setBehaviourMode(BehaviourManager::USER_MODE);
		userRButton->setChecked(true);
		return;
	}
	else if(event->text().toStdString() == HYBRID_MODE_KEY)
	{
		bManager->setBehaviourMode(BehaviourManager::HYBRID_MODE);
		hybridRButton->setChecked(true);
		return;
	}
	else if(event->text().toStdString() == AUTO_MODE_KEY)
	{
		bManager->setBehaviourMode(BehaviourManager::AUTO_MODE);
		autoRButton->setChecked(true);
		return;
	}

    if(this->userRButton->isChecked() || this->hybridRButton->isChecked())
    {
        std::cout << "Key pressed:   " << event->text().toStdString() << std::endl;
		bManager->sendCommand(event->text().toStdString());
    }
}
