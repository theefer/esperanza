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
#include <QDebug>

PreferencesDialog::PreferencesDialog (QWidget *parent, XClient *client_)
: QDialog (parent)
{
	setupUi (this);
	client = client_;
	QList<PreferenceValue *> prefs = PreferenceManager::instance ()->values ();
	initTabs (prefs);
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
	
	tabLook->setLayout (new QVBoxLayout ());
	tabFeel->setLayout (new QVBoxLayout ());
	tabCore->setLayout (new QVBoxLayout ());
	tabShortcuts->setLayout (new QVBoxLayout ());

	while (prefsIter.hasNext ())
	{
		tab = NULL;
		pref = prefsIter.next ();
		switch (pref->section ())
		{
			case PreferenceValue::Core: tab = tabCore; break;
			case PreferenceValue::Look: tab = tabLook; break;
			case PreferenceValue::Feel: tab = tabFeel; break;
			case PreferenceValue::Shortcuts: tab = tabShortcuts; break;
			default: qDebug () << "ERROR! unclassified Preference" << pref->key (); break;
		}
		if (tab)
			addPref (tab, pref);

	}

	tabLook->layout ()->addItem (new QSpacerItem (1, 1, QSizePolicy::Expanding, QSizePolicy::Expanding));
	tabFeel->layout ()->addItem (new QSpacerItem (1, 1, QSizePolicy::Expanding, QSizePolicy::Expanding));
	tabCore->layout ()->addItem (new QSpacerItem (1, 1, QSizePolicy::Expanding, QSizePolicy::Expanding));
	tabShortcuts->layout ()->addItem (new QSpacerItem (1, 1, QSizePolicy::Expanding, QSizePolicy::Expanding));
}

void PreferencesDialog::addPref (QWidget* tab, PreferenceValue *pref)
{
	QWidget *w = NULL;

	switch (pref->type ())
	{
		case PreferenceValue::Bool:
			w = createBoolPref (tab, pref);
			break;
		case PreferenceValue::Num:
			w = createNumberPref (tab, pref);
			break;
		case PreferenceValue::Str:
			w = createStringPref (tab, pref);
			break;
		case PreferenceValue::Selection:
			w = createSelectionPref (tab, pref);
			break;
		case PreferenceValue::MultiSelection:
			w = createMultiSelectPref (tab, pref);
			break;
		case PreferenceValue::Key:
			w = createShortcutPref (tab, pref);
			break;
		default:
			qDebug () << "ERROR! unknown preference type of key '" <<
						pref->key () << "' (type: '" << pref->type () << "')";
			break;
	}

	if(w)
		tab->layout ()->addWidget (w);
}

QWidget* PreferencesDialog::createBoolPref (QWidget* tab, PreferenceValue *pref)
{
	QSettings s;
	QCheckBox *cb;

	cb = new QCheckBox (tab);
	cb->setText (pref->help ());
	cb->setCheckState (s.value (pref->key (), pref->defval ()).toBool () ? Qt::Checked : Qt::Unchecked);

	return cb;
}

QWidget* PreferencesDialog::createNumberPref (QWidget* tab, PreferenceValue *pref)
{
	QSettings s;
	QWidget *w;
	QHBoxLayout *hlay;
	QLabel *lab;
	QSpinBox *sb;

	w = new QWidget (tab);
	hlay = new QHBoxLayout (w);
	lab = new QLabel (w);
	sb = new QSpinBox (w);

	sb->setRange (0, 999999); // what should be min and max!??! No Idea what are good values.
	sb->setValue (s.value (pref->key (), pref->defval ()).toInt ());

	lab->setText (pref->help ());
	lab->setSizePolicy (QSizePolicy::Expanding, QSizePolicy::Preferred);

	hlay->addWidget (lab);
	hlay->addItem (new QSpacerItem (10, 0, QSizePolicy::Preferred, QSizePolicy::Preferred));
	hlay->addWidget (sb);

	w->setLayout (hlay);

	return w;
}

QWidget* PreferencesDialog::createStringPref (QWidget* tab, PreferenceValue *pref)
{
	QSettings s;
	QWidget *w;
	QHBoxLayout *hlay;
	QLabel *lab;
	QLineEdit *le;

	w = new QWidget (tab);
	hlay = new QHBoxLayout (w);
	lab = new QLabel (w);
	le = new QLineEdit (w);

	lab->setText (pref->help ());

	le->setSizePolicy (QSizePolicy::Expanding, QSizePolicy::Preferred);
	le->setText (s.value (pref->key (), pref->defval ()).toString ());

	hlay->addWidget (lab);
	hlay->addItem (new QSpacerItem (10, 0, QSizePolicy::Preferred, QSizePolicy::Preferred));
	hlay->addWidget (le);

	w->setLayout (hlay);

	return w;
}

QWidget* PreferencesDialog::createSelectionPref (QWidget* tab, PreferenceValue *pref)
{
	QSettings s;
	QWidget *w;
	QHBoxLayout *hlay;
	QLabel *lab;
	QComboBox *co;
	QMapIterator<QString, QVariant> *iterSelect;

	w = new QWidget (tab);
	hlay = new QHBoxLayout (w);
	lab = new QLabel (w);
	co = new QComboBox (w);
	iterSelect = new QMapIterator<QString, QVariant> (pref->data ().toMap ());

	while (iterSelect->hasNext ()) {
		iterSelect->next ();
		co->addItem (iterSelect->key (), QVariant (iterSelect->value ()));
	}

	lab->setText (pref->help ());

	hlay->addWidget (lab);
	hlay->addItem (new QSpacerItem (10, 0, QSizePolicy::Preferred, QSizePolicy::Preferred));
	hlay->addWidget (co);

	w->setLayout (hlay);

	return w;
}

QWidget* PreferencesDialog::createMultiSelectPref (QWidget* tab, PreferenceValue *pref)
{
	QSettings s;
	QVBoxLayout *vbox;
	QHBoxLayout *hbox;
	QGroupBox *gb;
	QMapIterator<QString, QVariant> *iter;
	QCheckBox *cb;

	gb = new QGroupBox (tab);
	vbox = new QVBoxLayout ();
	iter = new QMapIterator<QString, QVariant> (pref->data ().toMap ());

	gb->setTitle (pref->help ());

	while (iter->hasNext ()) {
		iter->next ();
		hbox = new QHBoxLayout ();
		cb = new QCheckBox (gb);

		cb->setText (iter->key ());
		if (s.value (pref->key (), pref->defval ()).toString ().contains (iter->value ().toString ()))
			cb->setCheckState (Qt::Checked);
		else
			cb->setCheckState (Qt::Unchecked);
		hbox->addWidget (cb);

		if (iter->hasNext ())
		{
			iter->next ();
			cb = new QCheckBox (gb);

			cb->setText (iter->key ());
			if (s.value (pref->key (), pref->defval ()).toString ().contains (iter->value ().toString ()))
				cb->setCheckState (Qt::Checked);
			else
				cb->setCheckState (Qt::Unchecked);
			hbox->addWidget (cb);
		}
		vbox->addLayout (hbox);
	}

	gb->setLayout (vbox);

	return gb;
}

QWidget* PreferencesDialog::createShortcutPref (QWidget* tab, PreferenceValue *pref)
{
	QSettings s;
	QWidget *w;

	w = NULL;

	return w;
}

void PreferencesDialog::pressedOk ()
{
	pressedApply ();
	pressedCancel ();
}

void PreferencesDialog::pressedApply ()
{
	// TODO: save!!
}

void PreferencesDialog::pressedCancel ()
{
	close ();
}
