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


#include "streamingdialog.h"

#include "playerbutton.h"

#include <QTabWidget>
#include <QGridLayout>
#include <QResizeEvent>
#include <QInputDialog>

StreamingDialog::StreamingDialog (QWidget *parent, XClient *client) : QMainWindow (parent)
{
	QSettings s;

	m_client = client;

	setWindowFlags (Qt::Dialog);
	setAttribute (Qt::WA_DeleteOnClose);
	setWindowTitle (tr ("Esperanza - Streaming directory"));

	QWidget *base = new QWidget (this);
	setCentralWidget (base);
	
	QGridLayout *g = new QGridLayout (base);

	m_tab = new QTabWidget (base);
	m_bookmarks = new StreamingBookmarks (base, client);
	m_tab->addTab (m_bookmarks, tr ("My bookmarks"));
	m_tab->addTab (new QWidget, tr ("Last.Fm radio"));
	m_tab->addTab (new QWidget, tr ("Icecast directory"));

	g->addWidget (m_tab, 0, 0, 4, 4);
	g->setRowStretch (0, 1);
	
	QWidget *dummy = new QWidget (base);
	QHBoxLayout *hbox = new QHBoxLayout (dummy);

	g->addWidget (dummy, 4, 0, 1, 4);

	PlayerButton *add = new PlayerButton (dummy, ":images/plus.png");
	connect (add, SIGNAL (clicked (QMouseEvent *)), this, SLOT (add_pressed ()));
	PlayerButton *close = new PlayerButton (dummy, ":images/stop.png");

	hbox->addStretch (1);
	hbox->addWidget (add);
	hbox->addWidget (close);

	resize (s.value ("streamingdialog/size", QSize (500, 350)).toSize ());
}

Q_DECLARE_METATYPE(QList<quint32>)

void
StreamingDialog::add_pressed ()
{
	bool ok;
	QString url = QInputDialog::getText (this, tr ("Bookmark this URL"),
										 tr ("URL:"), QLineEdit::Normal,
										 "http://", &ok);
	if (ok) {
		m_client->medialib.addEntry (XClient::qToStd (url)) ();
		m_client->medialib.getID (XClient::qToStd (url)) (Xmms::bind (&StreamingDialog::handle_id, this));
	}
}

bool
StreamingDialog::handle_id (uint32_t id)
{
	m_bookmarks->add_bookmark (id);
	return true;
}

void
StreamingDialog::resizeEvent (QResizeEvent *ev)
{
	QSettings s;
	s.setValue ("streamingdialog/size", ev->size ());
}

StreamingBookmarks::StreamingBookmarks (QWidget *parent, XClient *client) : QTreeView (parent)
{
	setIndentation (0);
	setAlternatingRowColors (true);
	setItemsExpandable (false);
	setRootIsDecorated (false);
	setTabKeyNavigation (false);
	setTextElideMode (Qt::ElideNone);
	setVerticalScrollBarPolicy (Qt::ScrollBarAlwaysOn);

	m_client = client;
	connect (m_client->cache (), SIGNAL (entryChanged (uint32_t)), this, SLOT (entry_update (uint32_t)));
	m_model = new QStandardItemModel (this);
}

void
StreamingBookmarks::fill_bookmarks ()
{
	QSettings s;

	m_model->clear ();

	m_model->setColumnCount (3);
	QStringList headers;
	headers.append (QString ("Name"));
	headers.append (QString ("Play count"));
	headers.append (QString ("Last time played"));
	m_model->setHorizontalHeaderLabels (headers);

	setModel (m_model);

	QList<quint32> list = s.value ("streamingdialog/bookmarks").value<QList < quint32 > > ();
	for (int i = 0; i < list.size (); i++) {
		add_bookmark (list.value (i));
	}
}

void
StreamingBookmarks::add_bookmark (uint32_t id)
{
	QStandardItem *item;
	QHash<QString, QVariant> h = m_client->cache ()->get_info (id);
	if (h.contains ("channel")) {
		item = new QStandardItem (h["channel"].toString ());
	} else {
		item = new QStandardItem (QString::number (id));
	}
	m_items[id] = item;
}

void
StreamingBookmarks::entry_update (uint32_t id)
{
	if (m_items.contains (id)) {
		QHash<QString, QVariant> h = m_client->cache ()->get_info (id);
		QStandardItem *item = m_items[id];
		if (h.contains ("channel")) {
			item->setText (h["channel"].toString ());
		} else if (h.contains ("url")) {
			item->setText (h["url"].toString ());
		}
	}
}
