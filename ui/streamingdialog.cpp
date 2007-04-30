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
#include <QDateTime>
#include <QHeaderView>

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

void
StreamingDialog::add_pressed ()
{
	bool ok;
	QString url = QInputDialog::getText (this, tr ("Bookmark this URL"),
										 tr ("URL:"), QLineEdit::Normal,
										 "http://", &ok);
	if (ok) {
		m_client->medialib.addEntry (XClient::qToStd (url)) (boost::bind (&StreamingDialog::added_cb, this, url));
	}
}

bool
StreamingDialog::added_cb (const QString &url)
{
	qDebug ("url added!");
	m_client->medialib.getID (XClient::qToStd (url)) (Xmms::bind (&StreamingDialog::handle_id, this));
	return true;
}

bool
StreamingDialog::handle_id (uint32_t id)
{
	qDebug ("adding bookmark!");
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
	setHorizontalScrollBarPolicy (Qt::ScrollBarAlwaysOff);
	setTextElideMode (Qt::ElideRight);

	m_client = client;
	connect (m_client->cache (), SIGNAL (entryChanged (uint32_t)), this, SLOT (entry_update (uint32_t)));
	m_model = new QStandardItemModel (this);
	setModel (m_model);
	fill_bookmarks ();
}

void
StreamingBookmarks::fill_bookmarks ()
{
	QSettings s;

	m_model->clear ();

	m_model->setColumnCount (4);
	QStringList headers;
	headers.append (QString ("Name"));
	headers.append (QString ("Track"));
	headers.append (QString ("Play count"));
	headers.append (QString ("Last played"));
	m_model->setHorizontalHeaderLabels (headers);

	header ()->setResizeMode (0, QHeaderView::Stretch);
	header ()->setResizeMode (1, QHeaderView::Stretch);
	header ()->setStretchLastSection (false);

	QStringList list = s.value ("streamingdialog/bookmarks").toStringList ();
	for (int i = 0; i < list.size (); i++) {
		add_bookmark (list.value (i).toUInt ());
	}
}

void
StreamingBookmarks::add_bookmark (uint32_t id)
{
	QSettings s;

	QStringList l = s.value("streamingdialog/bookmarks").toStringList ();
	if (!l.contains (QString::number (id))) {
		l.append (QString::number (id));
		s.setValue ("streamingdialog/bookmarks", l);
	}

	QList<QStandardItem *> list;
	QStandardItem *text, *track, *playcount, *ltime;
	QHash<QString, QVariant> h = m_client->cache ()->get_info (id);

	text = new QStandardItem;
	track = new QStandardItem;
	playcount = new QStandardItem;
	ltime = new QStandardItem;

	list.append (text);
	list.append (track);
	list.append (playcount);
	list.append (ltime);

	set_attributes (h, list);

	m_items[id] = list;

	m_model->appendRow (list);
}

void
StreamingBookmarks::set_attributes (const QHash<QString, QVariant> &h, const QList<QStandardItem *> &list)
{
	QStandardItem *text = list[0];
	QStandardItem *track = list[1];
	QStandardItem *playcount = list[2];
	QStandardItem *ltime = list[3];

	if (h.contains ("channel")) {
		text->setText (h["channel"].toString ());
	} else {
		if (h.contains ("url")) {
			text->setText (h["url"].toString ());
		} else {
			text->setText (tr ("Getting info..."));
		}
	}
	if (h.contains ("title")) {
		track->setText (h["title"].toString ());
	} else {
		track->setText (tr ("N/A"));
	}
	if (h.contains ("timesplayed") && h["timesplayed"].toUInt () > 0) {
		playcount->setText (h["timesplayed"].toString ());
	} else {
		playcount->setText (tr ("Never"));
	}
	if (h.contains ("laststarted")) {
		QDateTime dt = QDateTime::fromTime_t (h["laststarted"].toUInt ());
		ltime->setText (dt.toString ("yy.MM.dd hh:mm"));
	} else {
		ltime->setText (tr ("Never"));
	}

	playcount->setTextAlignment (Qt::AlignRight);
	ltime->setTextAlignment (Qt::AlignRight);
}

void
StreamingBookmarks::entry_update (uint32_t id)
{
	if (m_items.contains (id)) {
		QHash<QString, QVariant> h = m_client->cache ()->get_info (id);
		QList<QStandardItem *> list = m_items[id];
		set_attributes (h, list);
	}
}
