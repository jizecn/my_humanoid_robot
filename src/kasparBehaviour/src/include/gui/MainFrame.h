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
#include <QComboBox>
#include <QDir>
#include <QGridLayout>
#include <QFileDialog>
#include <QLineEdit>
#include <QKeyEvent>

#include "BehaviourManager.h"
#include "BehaviourMode.h"
//#include "UserMode.h"
#include "AutonomousMode.h"
//#include "APFMode.h"

#include "KasparSequenceMap.h"

#include "ModeRadioButton.h"


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

public slots:
    void setMode(BehaviourManager::BehaviourModeType);

private slots:
    void chooseConfFile();
    void chooseBehFile();
    void setConfFile();
    void setBehFile();
    void keyPressEvent(QKeyEvent * event);

signals:

private:
    QComboBox *createComboBox(const QString &text);
    void initLayout(QString configfile);
    QGroupBox * createModeRadioButtons();

    BehaviourManager *bManager;
    BehaviourMode *bMode;
    //UserMode *uMode;
    //APFMode *aMode;
    //AutonomousMode *auMode;
    ModeRadioButton *userRButton;
    ModeRadioButton *autoRButton;
    ModeRadioButton *hybridRButton;

    QComboBox *directoryComboBox;
    QLineEdit *confFileText;
    QLineEdit *behFileText;

    static const int texteditwidth = 300;
};

#endif
