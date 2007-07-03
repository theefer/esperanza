#include "prefnumberwidget.h"
#include <QSettings>
#include <QDebug>

PrefNumberWidget::PrefNumberWidget (QWidget *parent, PreferenceValue *pref)
: QWidget (parent)
{
	QSettings s;
	setupUi (this);
	label->setText (pref->help ());	
	spinBox->setRange (0, 999999); // what should be min and max!??! No Idea what are good values.
	spinBox->setValue (s.value (pref->key (), pref->defval ()).toInt ());
}
