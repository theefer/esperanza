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


#ifndef __STREAMING_DIALOG_H__
#define __STREAMING_DIALOG_H__

#include "xclient.h"

#include <QMainWindow>
#include <QTabWidget>
#include <QSettings>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QTreeView>

class StreamingBookmarks : public QTreeView
{
	Q_OBJECT
	public:
		StreamingBookmarks (QWidget *parent, XClient *client);
		void add_bookmark (uint32_t);

	private slots:
		void entry_update (uint32_t);

	private:
		void fill_bookmarks ();
		void set_attributes (const QHash<QString, QVariant> &, const QList<QStandardItem *> &);

		XClient *m_client;
		QStandardItemModel *m_model;

		QHash<uint32_t, QList <QStandardItem*> > m_items;
};

class StreamingDialog : public QMainWindow
{
	Q_OBJECT
	public:
		StreamingDialog (QWidget *, XClient *);

		void resizeEvent (QResizeEvent *);


	private slots:
		void add_pressed ();

	private:
		bool added_cb (const QString &);
		bool handle_id (uint32_t);

		QTabWidget *m_tab;
		XClient *m_client;
		StreamingBookmarks *m_bookmarks;
};

#endif

