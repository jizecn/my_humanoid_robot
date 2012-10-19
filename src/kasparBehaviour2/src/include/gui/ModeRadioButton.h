#ifndef __MODE_RADIO_BUTTON_H__
#define __MODE_RADIO_BUTTON_H__

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

#include "BehaviourManager.h"
#include "BehaviourMode.h"
#include "UserMode.h"
#include "AutonomousMode.h"
#include "APFMode.h"

/**
 * RadioButton to select behaviour mode
 */
class ModeRadioButton : public QRadioButton {
    Q_OBJECT

public:
    ModeRadioButton(BehaviourManager::BehaviourModeType mode, const QString & text, QWidget * parent = 0, const char * name = 0);
    ~ModeRadioButton();
    BehaviourManager::BehaviourModeType  getModeNum();
public slots:
    //    void setMode(int);
    void emitModeSignal();
signals:
	void modeChanged(BehaviourManager::BehaviourModeType);

private:

    BehaviourManager::BehaviourModeType modeNum;
};

#endif
