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


#ifndef __PLAYLIST_VIEW_H__
#define __PLAYLIST_VIEW_H__

#include <xmmsclient/xmmsclient++.h>

#include <QTreeView>
#include <QItemDelegate>
#include <QDropEvent>

#include "playlistmodel.h"
#include "xclient.h"

class PlaylistView : public QTreeView
{
	Q_OBJECT
	public:
		PlaylistView (QWidget *parent, XClient *client);
		QModelIndexList getSelection ();

		void keyPressEvent (QKeyEvent *);
		void setModel (QAbstractItemModel *);

		/*
		PlaylistModel *model () const {
			return m_model;
		};
		*/

	public slots:
		void got_connection (XClient *client);
		void rows_inserted ();
		void head_size (int, int, int);
		void moved (const QModelIndex &, const QModelIndex &);
		void jump_pos (const QModelIndex &);
		bool handle_status (const Xmms::Playback::Status &);

	signals:
		void selectedID (uint32_t);

	protected:
		XClient *m_client;

	private:
		QWidget *m_parent;
		PlaylistModel *m_model;
		QItemSelectionModel *m_selections;
		Xmms::Playback::Status m_status;

		bool handle_update_pos (const uint32_t &);
};


#endif
