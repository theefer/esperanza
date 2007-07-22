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


#ifndef __FANCY_PLAYLIST_VIEW_H__
#define __FANCY_PLAYLIST_VIEW_H__

#include <xmmsclient/xmmsclient++.h>

#include <QTreeView>
#include <QItemDelegate>
#include <QDropEvent>

#include "playlistview.h"
#include "fancyplaylistmodel.h"
#include "xclient.h"

class PlaylistDelegate : public QItemDelegate
{
	public:
		PlaylistDelegate (QObject *parent, PlaylistModel *model);
		void paint (QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

	private:
		PlaylistModel *m_model;
};

class FancyPlaylistView : public PlaylistView
{
	Q_OBJECT
	public:
		FancyPlaylistView (QWidget *parent, XClient *client);
		void mousePressEvent (QMouseEvent *ev);

	public slots:
		void changed_settings ();

	private slots:
		void item_selected (const QItemSelection & selected, const QItemSelection & deselected);

	signals:
		void selectedID (uint32_t);

};

#endif
