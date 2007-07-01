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

	QStringList sl;
	sl << tr("Description");
	sl << tr("Keycombination");
	shortcutsTable->setColumnCount(2);
	shortcutsTable->setHorizontalHeaderLabels (sl);
	shortcutsTable->verticalHeader ()->hide ();
	shortcutsTable->horizontalHeader ()->setClickable (false);
	shortcutsTable->horizontalHeader ()->resizeSection (0, 400);
	shortcutsTable->horizontalHeader ()->resizeSection (1, 150);

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
			// just save the int as a widget pointer.
			prefWidgets[pref] = (QWidget*)createShortcutPref (tab, pref);
			w = NULL;
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
		if (s.value (pref->key (), pref->defval ()).toInt () == iterSelect->value ())
			co->setCurrentIndex (co->count () - 1);
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
	QString sValue;

	gb = new QGroupBox (tab);
	vbox = new QVBoxLayout ();
	iter = new QMapIterator<QString, QVariant> (pref->data ().toMap ());

	sValue = s.value (pref->key (), pref->defval ()).toString ();
	qDebug () << "Key: " << pref->key () << ", value: " << sValue;
	gb->setTitle (pref->help ());

	while (iter->hasNext ()) {
		iter->next ();
		hbox = new QHBoxLayout ();
		cb = new QCheckBox (gb);

		cb->setText (iter->key ());
		if (sValue.contains (iter->value ().toString ()))
			cb->setCheckState (Qt::Checked);
		else
			cb->setCheckState (Qt::Unchecked);
		hbox->addWidget (cb);

		if (iter->hasNext ())
		{
			iter->next ();
			cb = new QCheckBox (gb);

			cb->setText (iter->key ());
			if (sValue.contains (iter->value ().toString ()))
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

int PreferencesDialog::createShortcutPref (QWidget* tab, PreferenceValue *pref)
{
	QSettings s;
	QString sKey = s.value (pref->key (), pref->defval ()).toString ();
	QTableWidgetItem *desc = new QTableWidgetItem (pref->help ());
	QTableWidgetItem *key = new QTableWidgetItem (sKey);
	int row = shortcutsTable->rowCount ();

	// add a row
	shortcutsTable->setRowCount (row + 1);

	// set the content for the row
	qDebug () << "adding Shortcut, row: " << row << ", text: " << pref->help () << ", Key: " << sKey;
	shortcutsTable->setItem (row, 0, desc);
	shortcutsTable->setItem (row, 1, key);

	// we don't need it to return a Widget 
	// for the shortcuts tab
	return row;
}

bool PreferencesDialog::getBoolValue(QWidget *w)
{
	QCheckBox *cb;
	cb = dynamic_cast<QCheckBox *> (w);
	return cb->checkState () == Qt::Checked ? true : false;
}

int PreferencesDialog::getNumberValue(QWidget *w)
{
	QSpinBox *sb;
	sb = dynamic_cast<QSpinBox *> (w->layout ()->itemAt (2)->widget ());
	return sb->value ();
}

QString PreferencesDialog::getStringValue(QWidget *w)
{
	QLineEdit *le;
	le = dynamic_cast<QLineEdit *> (w->layout ()->itemAt (2)->widget ());
	return le->text ();
}

QVariant PreferencesDialog::getSelectionValue(QWidget *w)
{
	QComboBox *cb;
	cb = dynamic_cast<QComboBox *> (w->layout ()->itemAt (2)->widget ());
	return cb->itemData (cb->currentIndex ());
}

QString PreferencesDialog::getMultiSelectValue(QWidget *w, QMap<QString, QVariant> range)
{
	bool first = true;
	QString ret, tmp;
	QCheckBox *cb;
	QLayoutItem *li;
	for (int i = 0; i < w->layout ()->count (); i++)
	{
		li = w->layout ()->itemAt (i);
		for (int y = 0; y < li->layout ()->count (); y++)
		{
			cb = dynamic_cast<QCheckBox *> (li->layout ()->itemAt (y)->widget ());
			
			if (cb->checkState () == Qt::Checked)
			{
				if(!first)
					ret += ",";
				else
					first = false;
				ret += range[cb->text ()].toString ();
			}
		}
	}

	return ret;
}

QString PreferencesDialog::getShortcutValue(QWidget *w)
{

	return "";
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
				v = QVariant(getBoolValue (w));
				break;
			case PreferenceValue::Num:
				v = QVariant(getNumberValue (w));
				break;
			case PreferenceValue::Str:
				v = QVariant(getStringValue (w));
				break;
			case PreferenceValue::Selection:
				v = getSelectionValue (w);
				break;
			case PreferenceValue::MultiSelection:
				v = QVariant(getMultiSelectValue (w, pref->data ().toMap ()));
				break;
			case PreferenceValue::Key:
				break;
			default:
				qDebug () << "ERROR! unknown preference type of key '" <<
							pref->key () << "' (type: '" << pref->type () << "')";
				break;
		}

		if (v.isValid ())
		{
			qDebug () << "setting Key: " << pref->key () << " to value: " << v.toString ();
			s.setValue (pref->key (), v);
		}
		else
			qDebug () << "ERROR!! got no valid value for key: " << pref->key ();
	}
}

void PreferencesDialog::pressedCancel ()
{
	close ();
}
