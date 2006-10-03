#include "preferences.h"
#include "shortcuteditor.h"

ShortCutEditor::ShortCutEditor (QWidget *parent, XClient *client) :
	PreferenceDialog (parent, client)
{
}

QList < QMap < QString, QVariant > >
ShortCutEditor::build_prefvalues ()
{
	qDebug ("apa...");
	QList < QMap < QString, QVariant > > ret;

	PREF_VALUE("shortcut/openmlibwindow", "Open medialib window", T_KEY, "m");

	return ret;
}

