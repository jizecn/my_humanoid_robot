#include "gui/ModeRadioButton.h"
#include "BehaviourManager.h"
ModeRadioButton::ModeRadioButton(BehaviourManager::BehaviourModeType mode, const QString & text, QWidget * parent, const char * name) : QRadioButton(text, parent)
{
	modeNum = mode;
	connect(this, SIGNAL(clicked()), this, SLOT(emitModeSignal()));
}

ModeRadioButton::~ModeRadioButton()
{
}

void ModeRadioButton::emitModeSignal()
{
	emit modeChanged(modeNum);
}

BehaviourManager::BehaviourModeType ModeRadioButton::getModeNum()
{
    return modeNum;
}

