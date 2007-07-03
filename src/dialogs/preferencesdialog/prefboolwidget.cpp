#include "prefboolwidget.h"
#include <QSettings>
#include <QDebug>

PrefBoolWidget::PrefBoolWidget (QWidget *parent, PreferenceValue *pref)
: QWidget (parent)
{
	QSettings s;
	setupUi (this);
	checkBox->setText (pref->help ());	
	checkBox->setCheckState (s.value (pref->key (), pref->defval ()).toBool () ? Qt::Checked : Qt::Unchecked);
}
