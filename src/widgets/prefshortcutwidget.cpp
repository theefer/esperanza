#include "prefshortcutwidget.h"
#include <QSettings>
#include <QDebug>

#include <grepshortcutkeydlg/grepshortcutkeydlg.h>

PrefShortcutWidget::PrefShortcutWidget (QWidget *parent, PreferenceValue *pref)
: QWidget (parent)
{
	QSettings s;
	setupUi (this);
	label->setText (pref->help ());	
	leShortcut->setText (s.value (pref->key (), pref->defval ()).toString ());
}

void PrefShortcutWidget::grepShortcut ()
{
	GrepShortcutKeyDlg* grep = new GrepShortcutKeyDlg((QWidget*)parent ());
	connect(grep, SIGNAL(newShortcutKey(QKeySequence)), this, SLOT(onNewShortcutKey(QKeySequence)));
	grep->show();
}

void PrefShortcutWidget::onNewShortcutKey(QKeySequence key) {
	leShortcut->setText (key.toString(QKeySequence::NativeText));
}

