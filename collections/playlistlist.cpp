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

#include "collections/playlistlist.h"

#include <QTreeWidget>
#include <QHeaderView>

PlaylistList::PlaylistList (QWidget *parent, XClient *client) : QTreeWidget (parent)
{
	setColumnCount (1);
	setHeaderLabels (QStringList ("Playlists"));
	setIndentation (0);
	setAlternatingRowColors (true);
	setItemsExpandable (false);
	setRootIsDecorated (false);
	setTabKeyNavigation (false);
	setTextElideMode (Qt::ElideNone);

	header ()->setResizeMode (0, QHeaderView::Stretch);
	header ()->setClickable (false);
	setSelectionMode (QAbstractItemView::SingleSelection);
	setFrameShape (QFrame::NoFrame);

	client->collection.list (Xmms::Collection::PLAYLISTS, Xmms::bind (&PlaylistList::list_cb, this));
}

bool
PlaylistList::list_cb (const Xmms::List<std::string> &list)
{
	QTreeWidgetItem *i;
	clear ();

	for (list.first (); list.isValid (); ++ list) {
		QString s = QString::fromUtf8 ((*list).c_str ());
		if (s.startsWith ("_"))
			continue;
		i = new QTreeWidgetItem (QStringList (s));
		addTopLevelItem (i);
	}

	return true;
}

