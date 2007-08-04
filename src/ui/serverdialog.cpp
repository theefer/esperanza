/**
 *  This file is a part of Esperanza, an XMMS2 Client.
 *
 *  Copyright (C) 2005-2007 XMMS2 Team
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


#include "serverdialog.h"

#include <QDialog>
#include <QGridLayout>
#include <QMap>
#include <QSettings>
#include <QLabel>
#include <QListWidgetItem>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QErrorMessage>

#include "playerbutton.h"
#include "mdns.h"
#include "debug.h"

ServerDialog::ServerDialog (QWidget *parent) : QDialog (parent), m_mdns (NULL)
{
	m_mdns.browse_service ("_xmms2._tcp");

	QGridLayout *g = new QGridLayout (this);

	QLabel *l = new QLabel (tr ("Welcome to Esperanza\nPlease select a server to connect to.\nYou may disable this dialog in the preferences."), this);
	l->setAlignment (Qt::AlignCenter);
	g->addWidget (l, 0, 0, 1, 2);
	g->setRowStretch (0, 0);

	m_list = new QListWidget (this);
	m_list->setSelectionMode (QAbstractItemView::SingleSelection);

	QMap <QString, QVariant> def;
#ifndef Q_WS_WIN32
	def["local"] = QString ("local");
#endif

	QSettings s;
	QMap<QString, QVariant> m = s.value ("serverbrowser/list", def).toMap ();

	QString defconn = s.value ("serverbrowser/default", "local").toString ();

	QList<QString> lst = m.keys ();
	for (int i = 0; i < lst.size (); i ++) {
		QListWidgetItem *q = new QListWidgetItem (lst.at (i), m_list);
		q->setData (Qt::ToolTipRole, m[lst.at (i)]);
		if (lst.at (i) == defconn)
			m_list->setCurrentItem (q);
	}

	g->addWidget (m_list, 1, 0, 2, 2);
	g->setRowStretch (1, 1);
	g->setMargin (1);

	QWidget *dummy = new QWidget (this);
	QHBoxLayout *hbox = new QHBoxLayout (dummy);

	PlayerButton *c = new PlayerButton (dummy, tr ("Connect"));
	connect (c, SIGNAL(clicked (QMouseEvent *)), this, SLOT (accept ()));
	hbox->addWidget (c);

	c = new PlayerButton (dummy, tr ("Quit"));
	connect (c, SIGNAL(clicked (QMouseEvent *)), this, SLOT (reject ()));
	hbox->addWidget (c);

	hbox->addStretch (1);

	PlayerButton *b = new PlayerButton (dummy, ":images/plus.png");
	connect (b, SIGNAL (clicked (QMouseEvent *)), this, SLOT (add_server ()));
	hbox->addWidget (b);

	PlayerButton *b2 = new PlayerButton (dummy, ":images/minus.png");
	connect (b2, SIGNAL (clicked (QMouseEvent *)), this, SLOT (remove_server ()));
	hbox->addWidget (b2);

	g->addWidget (dummy, 3, 0, 1, 2);

	connect (m_list, SIGNAL (itemActivated (QListWidgetItem *)), this, SLOT (accept ()));
	connect (&m_mdns, SIGNAL (serverlistChanged ()), this, SLOT (mdns_server_update ()));

	mdns_server_update ();

	resize (250, 300);
}

void
ServerDialog::mdns_server_update ()
{
	QList<MDNSServer *>servers = m_mdns.serverlist ();
	QSettings s;

	while (m_mdns_servers.count ()) {
		QListWidgetItem *item = m_mdns_servers.at (0);
		m_mdns_servers.removeAll (item);
		delete item;
	}

	for (int i = 0; i < servers.count (); i++) {
		MDNSServer *serv = servers.at (i);
		if (!serv->complete ())
			continue;
		QListWidgetItem *item = new QListWidgetItem (serv->name () + " (discovered)", m_list);
		QString path = QString ("tcp://%1:%2").arg (serv->ip()).arg (serv->port ());
		item->setData (Qt::ToolTipRole, path);
		item->setData (Qt::UserRole, true);
		if (item->text () == s.value ("serverbrowser/default").toString ())
			m_list->setCurrentItem (item);
		m_mdns_servers.append (item);
	}
}

void
ServerDialog::keyPressEvent (QKeyEvent *ev)
{
	if (ev->key () == Qt::Key_Enter || ev->key () == Qt::Key_Return)
		accept ();
}

AddServerDialog::AddServerDialog (QWidget *parent)
: QDialog(parent)
{
	QGridLayout *g = new QGridLayout (this);
	QLabel *l = new QLabel (tr ("Add a new server"), this);
	g->addWidget (l, 0, 0, 1, 2);

	l = new QLabel (tr ("Name"), this);
	g->addWidget (l, 1, 0, 1, 1);
	m_name = new QLineEdit (this);
	g->addWidget (m_name, 1, 1, 1, 1);

	l = new QLabel (tr ("Path"), this);
	g->addWidget (l, 2, 0, 1, 1);
	m_path = new QLineEdit (this);
	g->addWidget (m_path, 2, 1, 1, 1);

	QPushButton *ok = new QPushButton (tr ("Add"), this);
	connect (ok, SIGNAL (clicked ()), this, SLOT (accept ()));
	QPushButton *cancel = new QPushButton (tr ("Cancel"), this);
	connect (cancel, SIGNAL (clicked ()), this, SLOT (reject ()));
	g->addWidget (ok, 3, 0, 1, 1);
	g->addWidget (cancel, 3, 1, 1, 1);
}

void
ServerDialog::add_server ()
{
	AddServerDialog add (this);
	QListWidgetItem *item;
	if (add.exec () == QDialog::Accepted) {
		for (int i = 0; i < m_list->count (); i++) {
			item = m_list->item (i);
			if (item->text () == add.m_name->text ()) {
				QErrorMessage *err = new QErrorMessage (this);
				err->showMessage (tr ("Sorry, that name is already taken."));
				err->exec ();
				return;
			}
		}
		item = new QListWidgetItem (add.m_name->text (), m_list);
		item->setData (Qt::ToolTipRole, add.m_path->text ());
		m_list->setCurrentItem (item);
	}
}

void
ServerDialog::remove_server ()
{
	QListWidgetItem *item = m_list->currentItem ();

	if (item->data (Qt::UserRole).toBool ()) {
		QErrorMessage *err = new QErrorMessage (this);
		err->showMessage (tr ("Sorry, you can't remove autodetected servers!"));
		err->exec ();
		return;
	}
	if (item->text () == "local") {
		QErrorMessage *err = new QErrorMessage (this);
		err->showMessage (tr ("Sorry, you can't remove the local server!"));
		err->exec ();
		return;
	}

	delete item;
}

QString
ServerDialog::get_default ()
{
	QSettings s;
	QMap<QString, QVariant> m = s.value ("serverbrowser/list").toMap ();
	QString defconn = s.value ("serverbrowser/default", "local").toString ();

	QList<QString> lst = m.keys ();
	for (int i = 0; i < lst.size (); i ++) {
		if (lst.at (i) == defconn)
			return m[lst.at (i)].toString ();
	}

	return QString ();
}

QString
ServerDialog::get_path ()
{
	QSettings s;
	int ret = exec ();
	QString tmp;

	QMap<QString, QVariant> m;
	for (int i = 0; i < m_list->count (); i ++) {
		QListWidgetItem *item = m_list->item (i);
		if (!item->data (Qt::UserRole).toBool ())
			m[item->text ()] = QVariant (item->toolTip ());
	}

	s.setValue ("serverbrowser/list", m);
	QListWidgetItem *d = m_list->currentItem ();
	if (d) {
		s.setValue ("serverbrowser/default", d->text ());
		if (ret == QDialog::Accepted) {
			tmp = d->toolTip ();
			DBGOUT ("returning:" << tmp);
			return tmp;
		}
	}
	return QString ();
}
