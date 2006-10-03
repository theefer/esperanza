#ifndef __SHORTCUTEDITOR_H__
#define __SHORTCUTEDITOR_H__

#include "preferences.h"

class ShortCutEditor : public PreferenceDialog
{
	Q_OBJECT
	public:
		ShortCutEditor (QWidget *, XClient *);
		QList < QMap < QString, QVariant > > build_prefvalues ();
};

#endif
