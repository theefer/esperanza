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

#include "preferencesdialog.h"

#include <QList>
#include <QListIterator>
#include <QWidget>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QSpinBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QSettings>
#include <QComboBox>
#include <QGroupBox>
#include <QHeaderView>
#include <QTableWidgetItem>
#include <QScrollArea>
#include <QDebug>

#include "prefboolwidget.h"
#include "prefnumberwidget.h"
#include "prefstringwidget.h"
#include "prefselectionwidget.h"
#include "prefmultiselectionwidget.h"
#include "prefshortcutwidget.h"

PreferencesDialog::PreferencesDialog (QWidget *parent, XClient *client_)
: QDialog (parent)
{
	QSettings s;
	int tab = s.value ("preferencesdialog/lastusedtab", 0).toInt ();

	setupUi (this);

	if (tab < 0 || tab > tabWidget->count () - 1)
	{
		tab = 0;
		s.setValue ("preferencesdialog/lastusedtab", 0);
	}

	client = client_;
	QList<PreferenceValue *> prefs = PreferenceManager::instance ()->values ();
	initTabs (prefs);
	tabWidget->setCurrentIndex (tab);
	connect(tabWidget, SIGNAL (currentChanged (int)), this, SLOT (currentTabChanged (int)));
}

void PreferencesDialog::clickedBtnBox (QAbstractButton* btn)
{
	switch (buttonBox->buttonRole (btn))
	{
		case QDialogButtonBox::AcceptRole:
			pressedOk ();
			break;
		case QDialogButtonBox::ApplyRole:
			pressedApply ();
			break;
		case QDialogButtonBox::RejectRole:
			pressedCancel ();
			break;
		default:
			break;
	}
}

void PreferencesDialog::initTabs (QList<PreferenceValue *> prefs)
{
	PreferenceValue *pref;
	QListIterator<PreferenceValue *> prefsIter (prefs);
	QWidget *tab;
	int margin = 2;
	
	tabLook->setLayout (new QVBoxLayout ());
	tabFeel->setLayout (new QVBoxLayout ());
	tabCore->setLayout (new QVBoxLayout ());
	tabShortcuts->setLayout (new QVBoxLayout ());

	tabLook->layout ()->setMargin (margin);
	tabFeel->layout ()->setMargin (margin);
	tabCore->layout ()->setMargin (margin);
	tabShortcuts->layout ()->setMargin (margin);

	QWidget *tabsaLook = createScrollableWidget (tabLook);
	QWidget *tabsaFeel = createScrollableWidget (tabFeel);
	QWidget *tabsaCore = createScrollableWidget (tabCore);
	QWidget *tabsaShortcuts = createScrollableWidget (tabShortcuts);
	
	while (prefsIter.hasNext ())
	{
		tab = NULL;
		pref = prefsIter.next ();
		switch (pref->section ())
		{
			case PreferenceValue::Core: tab = tabsaCore; break;
			case PreferenceValue::Look: tab = tabsaLook; break;
			case PreferenceValue::Feel: tab = tabsaFeel; break;
			case PreferenceValue::Shortcuts: tab = tabsaShortcuts; break;
			default: qDebug () << "ERROR! unclassified Preference" << pref->key (); break;
		}
		if (tab)
		{
			addPref (tab, pref);
		}

	}

	tabsaLook->layout ()->addItem (new QSpacerItem (1, 1, QSizePolicy::Expanding, QSizePolicy::Expanding));
	tabsaFeel->layout ()->addItem (new QSpacerItem (1, 1, QSizePolicy::Expanding, QSizePolicy::Expanding));
	tabsaCore->layout ()->addItem (new QSpacerItem (1, 1, QSizePolicy::Expanding, QSizePolicy::Expanding));
	tabsaShortcuts->layout ()->addItem (new QSpacerItem (1, 1, QSizePolicy::Expanding, QSizePolicy::Expanding));
}

QWidget* PreferencesDialog::createScrollableWidget (QWidget *parent)
{
	QWidget *w;
	QScrollArea *sa;

	sa = new QScrollArea (parent);
	w = new QWidget (sa);
	w->setLayout (new QVBoxLayout ());
	w->setSizePolicy (QSizePolicy::Expanding, QSizePolicy::Expanding);
	sa->setWidgetResizable (true);
	sa->setWidget (w);
	parent->layout ()->addWidget (sa);
	return w;
}

void PreferencesDialog::addPref (QWidget* tab, PreferenceValue *pref)
{
	QWidget *w = NULL;

	switch (pref->type ())
	{
		case PreferenceValue::Bool:
			w = new PrefBoolWidget (tab, pref);
			break;
		case PreferenceValue::Num:
			w = new PrefNumberWidget (tab, pref);
			break;
		case PreferenceValue::Str:
			w = new PrefStringWidget (tab, pref);
			break;
		case PreferenceValue::Selection:
			w = new PrefSelectionWidget (tab, pref);
			break;
		case PreferenceValue::MultiSelection:
			w = new PrefMultiSelectionWidget (tab, pref);
			break;
		case PreferenceValue::Key:
			w = new PrefShortcutWidget (tab, pref);
			break;
		default:
			qDebug () << "ERROR! unknown preference type of key '" <<
						pref->key () << "' (type: '" << pref->type () << "')";
			break;
	}

	if(w)
	{
		tab->layout ()->addWidget (w);
		prefWidgets[pref] = w;
	}
}

void PreferencesDialog::pressedOk ()
{
	pressedApply ();
	pressedCancel ();
}

void PreferencesDialog::pressedApply ()
{
	QSettings s;
	QMapIterator<PreferenceValue *, QWidget *> iter (prefWidgets);
	PreferenceValue *pref;
	QWidget *w;
	QVariant v;

	while(iter.hasNext ())
	{
		v.clear ();
		iter.next ();
		pref = iter.key ();
		w = iter.value ();

		switch (pref->type ())
		{
			case PreferenceValue::Bool:
				v = (dynamic_cast<PrefBoolWidget *> (w))->getPrefValue ();
				break;
			case PreferenceValue::Num:
				v = (dynamic_cast<PrefNumberWidget *> (w))->getPrefValue ();
				break;
			case PreferenceValue::Str:
				v = (dynamic_cast<PrefStringWidget *> (w))->getPrefValue ();
				break;
			case PreferenceValue::Selection:
				v = (dynamic_cast<PrefSelectionWidget *> (w))->getPrefValue ();
				break;
			case PreferenceValue::MultiSelection:
				v = (dynamic_cast<PrefMultiSelectionWidget *> (w))->getPrefValue ();
				break;
			case PreferenceValue::Key:
				v = (dynamic_cast<PrefShortcutWidget *> (w))->getPrefValue ();
				break;
			default:
				qDebug () << "ERROR! unknown preference type of key '" <<
							pref->key () << "' (type: '" << pref->type () << "')";
				break;
		}

		if (v.isValid ())
			s.setValue (pref->key (), v);
		else
			qDebug () << "ERROR!! got no valid value for key: " << pref->key ();
	}
}

void PreferencesDialog::pressedCancel ()
{
	close ();
}

void PreferencesDialog::currentTabChanged (int index)
{
	QSettings ().setValue ("preferencesdialog/lastusedtab", index);
}
