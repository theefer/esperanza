/** 
 *  This file is a part of Esperanza, an XMMS2 Client.
 *
 *  Copyright (C) 2005-2006 XMMS2 Team
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version 2
 *  of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 */


#ifndef __PREFERENCESDIALOG_H__
#define __PREFERENCESDIALOG_H__

#include "xclient.h"
#include "preferencemanager.h"

#include "ui_preferencesdialog.h"

class PreferencesDialog : public QDialog, public Ui::prefDialog
{
	Q_OBJECT
	public:
		PreferencesDialog (QWidget *parent, XClient *client_);

	public slots:
		void clickedBtnBox(QAbstractButton*);

	protected:
		void pressedOk ();
		void pressedApply ();
		void pressedCancel ();
		void initTabs (QList<PreferenceValue *> prefs);
		void addPref (QWidget* tab, PreferenceValue *pref);

		// show the current PrefValues
		QWidget* createBoolPref(QWidget* tab, PreferenceValue *pref);
		QWidget* createNumberPref(QWidget* tab, PreferenceValue *pref);
		QWidget* createStringPref(QWidget* tab, PreferenceValue *pref);
		QWidget* createSelectionPref(QWidget* tab, PreferenceValue *pref);
		QWidget* createMultiSelectPref(QWidget* tab, PreferenceValue *pref);
		int createShortcutPref(QWidget* tab, PreferenceValue *pref);

		// get the new PrefValues
		bool getBoolValue(QWidget *w);
		int getNumberValue(QWidget *w);
		QString getStringValue(QWidget *w);
		QVariant getSelectionValue(QWidget *w);
		QString getMultiSelectValue(QWidget *w, QMap<QString, QVariant> range);
		QString getShortcutValue(QWidget *w);


	private:
		QMap<PreferenceValue *, QWidget *> prefWidgets;
		XClient *client;
};
#endif
