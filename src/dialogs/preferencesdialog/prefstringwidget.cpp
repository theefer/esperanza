#include "prefstringwidget.h"
#include <QSettings>
#include <QDebug>

PrefStringWidget::PrefStringWidget (QWidget *parent, PreferenceValue *pref)
: QWidget (parent)
{
	QSettings s;
	setupUi (this);
	label->setText (pref->help ());	
	lineEdit->setText (s.value (pref->key (), pref->defval ()).toString ());
}
