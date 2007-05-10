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


#include "xclient.h"
#include "preferences.h"

#include <QMainWindow>
#include <QSettings>
#include <QGridLayout>
#include <QWidget>
#include <QSpinBox>
#include <QCheckBox>
#include <QHeaderView>
#include <QPushButton>
#include <QColor>
#include <QLabel>
#include <QColorDialog>
#include <QMessageBox>
#include <QApplication>
#include <QLineEdit>
#include <QKeyEvent>
#include <QDialogButtonBox>
#include <QSystemTrayIcon>

QList < QMap < QString, QVariant > >
PreferenceDialog::build_prefvalues ()
{
	QList < QMap < QString, QVariant > > ret;

	PREF_VALUE("core/autostart", tr ("Autostart xmms2d if not running"), T_BOOL, true);
	PREF_VALUE("core/ignoredesktopsettings", tr ("Ignore the desktop settings, use this if everything is black"), T_BOOL, false);
	PREF_VALUE("core/numrandomsongs", tr ("Number of songs to randomly add"), T_NUM, 5);
	PREF_VALUE("core/skiplocales", tr ("Ignore locale settings and use default language"), T_BOOL, false);
	PREF_VALUE("serverdialog/show", tr ("Show serverbrowser on startup"), T_BOOL, true);
	PREF_VALUE("playlist/jumptocurrent", tr ("Jump to current entry on song change"), T_BOOL, true);
	PREF_VALUE("playlist/compactmode", tr ("Use compact (boring) playlist mode"), T_BOOL, false);
	PREF_VALUE("playlist/albumartplace", tr ("Show albumart under artist"), T_BOOL, true);
	PREF_VALUE("ui/showstop", tr ("Show a stop button as well"), T_BOOL, false);
	PREF_VALUE("ui/reverseplaytime", tr ("Use reversed elapsed time"), T_BOOL, true);
	PREF_VALUE("core/pixmapcache", tr ("Size of album art cache in kb"), T_NUM, 12040);
	PREF_VALUE("ui/contextvalues", tr ("Values to be shown in context row (comma-separated)"), T_STR, "album,timesplayed,duration");
	PREF_VALUE("ui/contextareabright", tr ("Draw the contextarea in a bright color"), T_BOOL, true);
	PREF_VALUE("ui/titlelighter", tr ("Paint the progress in a lighter color"), T_BOOL, false);
	PREF_VALUE("ui/volumepopup", tr ("Show volume in a popup"), T_BOOL, false);
	PREF_VALUE("ui/volumeinteractive", tr ("Change volume interactivly, could cause problems."), T_BOOL, false);
	PREF_VALUE("lastfm/showoink", tr ("Show oink search in last.fm context"), T_BOOL, false);
    PREF_VALUE("medialib/completion", tr ("Load artists and albums for completion in Medialib window"), T_BOOL, true);
	if (QSystemTrayIcon::isSystemTrayAvailable ()) {
		PREF_VALUE("core/systray", tr ("Show system tray icon"), T_BOOL, true);
		PREF_VALUE("core/donotification", tr ("Show notification on song change"), T_BOOL, true);
	}

	return ret;
}

void
PreferenceDialog::save_defaults ()
{
	QSettings s;
	QList < QMap < QString, QVariant > > prefs = PreferenceDialog::build_prefvalues ();

	for (int i = 0; i < prefs.size (); i ++) {
		QMap<QString, QVariant> m = prefs.at (i);
		if (!s.contains (m["value"].toString ())) {
			s.setValue (m["value"].toString (), m["default"]);
		}
	}
}

PreferenceDialog::PreferenceDialog (QWidget *parent, XClient *client) : QMainWindow (parent)
{
	setWindowFlags (Qt::Dialog);
	setAttribute (Qt::WA_DeleteOnClose);
	setWindowTitle ("Esperanza - Preferences");
	QSettings s;

	m_client = client;

	m_base = new QWidget (this);
	setCentralWidget (m_base);
	resize (600, 500);

	QGridLayout *g = new QGridLayout (m_base);

	m_table = new QTableWidget (m_base);
	m_table->setColumnCount (2);

	QStringList l (tr ("Name"));
	l.append (tr ("Value"));

	m_table->setHorizontalHeaderLabels (l);
	m_table->verticalHeader ()->hide ();
	m_table->horizontalHeader ()->setResizeMode (0, QHeaderView::Stretch);
	m_table->horizontalHeader ()->setClickable (false);
	m_table->horizontalHeader ()->resizeSection (1, 100);
	m_table->setSelectionMode (QAbstractItemView::NoSelection);

	g->addWidget (m_table, 0, 0, 1, 1);
	g->setColumnStretch (0, 1);
	g->setRowStretch (0, 1);

	QDialogButtonBox *bbox = new QDialogButtonBox (Qt::Horizontal, m_base);

	QPushButton *ok = new QPushButton (tr ("Save"), bbox);
	ok->setDefault (true);
	connect (ok, SIGNAL (clicked ()), this, SLOT (on_save ()));

	QPushButton *cancel = new QPushButton (tr ("Discard"), bbox);
	connect (cancel, SIGNAL (clicked ()), this, SLOT (on_cancel ()));

	QPushButton *reset = new QPushButton (tr ("Reset"), bbox);
	connect (reset, SIGNAL (clicked ()), this, SLOT (on_reset ()));

	bbox->addButton (ok, QDialogButtonBox::AcceptRole);
	bbox->addButton (reset, QDialogButtonBox::ResetRole);
	bbox->addButton (cancel, QDialogButtonBox::DestructiveRole);

	g->addWidget (bbox, 1, 0, 1, 1);

}

void
PreferenceDialog::showEvent (QShowEvent *ev)
{
	if (m_table->rowCount () < 1)
		fill_list ();
	QMainWindow::showEvent (ev);
}

void
PreferenceDialog::fill_list ()
{
	QSettings s;

	QList < QMap < QString, QVariant > > pref = build_prefvalues ();

	m_table->setRowCount (pref.size ());

	for (int i = 0; i < pref.size (); i ++) {
		QMap<QString, QVariant> m = pref.at (i);
		QString val = m["value"].toString ();
		QString help = m["help"].toString ();
		QVariant def = m["default"];
		int type = m["type"].toInt ();

		QTableWidgetItem *item = new QTableWidgetItem (help);
		item->setData (Qt::UserRole, m);
		item->setToolTip (help);
		
		m_table->setItem (i, 0, item);

		switch (type) {
			case T_BOOL:
				{
					QCheckBox *cb = new QCheckBox (this);
					if (s.value (val, def).toBool ())
						cb->setCheckState (Qt::Checked);
					else
						cb->setCheckState (Qt::Unchecked);
					m_table->setCellWidget (i, 1, cb);
					break;
				}
			case T_NUM:
				{
					QSpinBox *sp = new QSpinBox (m_base);
					sp->setRange (0, INT_MAX);
					sp->setValue (s.value (val, def).toInt ());
					m_table->setCellWidget (i, 1, sp);
					break;
				}
			case T_COLOR:
				{
					ColorButton *cb = new ColorButton (m_base, s.value (val, def).value<QColor> ());
					m_table->setCellWidget (i, 1, cb);
					break;
				}
			case T_STR:
				{
					QLineEdit *le = new QLineEdit (s.value (val, def).toString (), m_base);
					m_table->setCellWidget (i, 1, le);
					break;
				}
			default:
				qDebug ("error!");
				break;
		}
	}
}

void
PreferenceDialog::on_save ()
{
	QSettings s;
	for (int i = 0; i < m_table->rowCount (); i++) {
		QTableWidgetItem *item = m_table->item (i, 0);
		QMap<QString, QVariant> m = item->data (Qt::UserRole).toMap ();
		QVariant ret;
		switch (m["type"].toInt ()) {
			case T_BOOL:
				{
					QCheckBox *cb = dynamic_cast<QCheckBox *>(m_table->cellWidget (i, 1));
					ret = QVariant (cb->checkState () == Qt::Checked ? true : false);
					break;
				}
			case T_NUM:
				{
					QSpinBox *sp = dynamic_cast<QSpinBox *>(m_table->cellWidget (i, 1));
					ret = QVariant (sp->value ());
					break;
				}
			case T_COLOR:
				{
					ColorButton *cb = dynamic_cast<ColorButton *>(m_table->cellWidget (i, 1));
					ret = QVariant (cb->current_color ());
					break;
				}
			case T_STR:
				{
					QLineEdit *le = dynamic_cast<QLineEdit *>(m_table->cellWidget (i, 1));
					ret = QVariant (le->text ());
					break;
				}
		}
		s.setValue (m["value"].toString (), ret);
	}

	m_client->settings ()->change_settings ();

	close ();
	s.sync ();
}

void
PreferenceDialog::on_cancel ()
{
	close ();
}

void
PreferenceDialog::on_reset ()
{
	QSettings s;

	if (QMessageBox::question (this,
							   tr ("Reset settings?"),
							   tr ("Are your sure you want to reset program to standard values?"),
							   tr ("Yes"), tr ("No")) == 0) {
		QStringList l = s.allKeys ();
		for (int i = 0; i < l.count (); i++) {
			s.remove (l.at (i));
		}
	}

	close ();
}
