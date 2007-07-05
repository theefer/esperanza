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


#ifndef __STREAMING_DIALOG_H__
#define __STREAMING_DIALOG_H__

#include "xclient.h"

#include "playlistmodel.h"
#include "icecasthandler.h"

#include <QDialog>
#include <QTabWidget>
#include <QSettings>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QTreeView>
#include <QKeyEvent>
#include <QTimer>
#include <QHttp>
#include <QTemporaryFile>
#include <QProgressBar>
#include <QPushButton>
#include <QSortFilterProxyModel>

class StreamingIcecast : public QWidget
{
    Q_OBJECT
    public:
        StreamingIcecast (QWidget *parent, XClient *client);

	public slots:
		void context_menu (const QPoint &);
        
    private slots:
        void req_start ();
        void req_finished (int, bool);
        void req_progress (int, int);
        void refresh ();
        void parse_xml ();
        void add_channels (const QList<IcecastChannel> &);
        void sort (int);
        void do_filter (const QString &);
        void dbclicked (const QModelIndex &);
		void add_to_pls ();
		void add_to_bookmarks ();
		void custom_context (const QPoint &);
        
    private:

        QHttp m_http;
        QFile m_file;
        QProgressBar *m_progress;
        QTreeView *m_tree;
        QPushButton *m_refresh;
        QStandardItemModel *m_model;
        QSortFilterProxyModel *m_proxy;
        QLineEdit *m_le;
        XClient *m_client;
		Qt::SortOrder m_order;
};

class StreamingBookmarks : public QTreeView
{
	Q_OBJECT
	public:
		StreamingBookmarks (QWidget *parent, XClient *client);

        void keyPressEvent (QKeyEvent *);
    private slots:
        void dbclicked (const QModelIndex &);
        void update_list ();
    private:
        bool handle_list (const Xmms::List<std::string> &);
		XClient *m_client;
        PlaylistModel *m_model;
        QTimer *m_timer;

};

class StreamingDialog : public QDialog
{
	Q_OBJECT
	public:
		StreamingDialog (QWidget *, XClient *);

		void resizeEvent (QResizeEvent *);


	private slots:
		void add_pressed (QMouseEvent *);
        void set_current (int);

	private:
		bool added_cb (const QString &);
		bool handle_id (uint32_t);

		QTabWidget *m_tab;
		XClient *m_client;
		StreamingBookmarks *m_bookmarks;
};

#endif

