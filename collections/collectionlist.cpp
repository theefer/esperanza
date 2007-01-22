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

#include "collections/collectionlist.h"

#include <QTreeWidget>
#include <QTableWidgetItem>
#include <QHeaderView>

CollectionList::CollectionList (QWidget *parent, XClient *client) : QTreeWidget (parent)
{
	setColumnCount (1);
	setHeaderLabels (QStringList ("Collections"));
	setAlternatingRowColors (true);
	setRootIsDecorated (true);
	setTabKeyNavigation (false);

	header ()->setResizeMode (0, QHeaderView::Stretch);
	header ()->setClickable (false);
	setSelectionMode (QAbstractItemView::SingleSelection);
	setFrameShape (QFrame::NoFrame);

	connect (this, SIGNAL (itemClicked (QTreeWidgetItem *, int)),
			 this, SLOT (active_row (QTreeWidgetItem *, int)));

	m_collections = new QTreeWidgetItem (this, QStringList (tr ("Collections")));

	m_playlists = new QTreeWidgetItem (this, QStringList (tr ("Playlists")));

	client->collection.list (Xmms::Collection::COLLECTIONS)(
							 boost::bind (&CollectionList::list_cb, this,
										  Xmms::Collection::COLLECTIONS, _1));

	client->collection.list (Xmms::Collection::PLAYLISTS)(
							 boost::bind (&CollectionList::list_cb, this,
										  Xmms::Collection::PLAYLISTS, _1));
}

void
CollectionList::active_row (QTreeWidgetItem *item, int c)
{
	CollectionListItem *i = dynamic_cast<CollectionListItem *> (item);
	emit switch_view (i->ns (), i->text (0));
}

bool
CollectionList::list_cb (const Xmms::Collection::Namespace &ns,
						 const Xmms::List<std::string> &list)
{
	QTreeWidgetItem *i;

	if (ns == Xmms::Collection::COLLECTIONS) {
		while ((i = m_collections->takeChild (0))) {
			delete i;
		}
	} else {
		while ((i = m_playlists->takeChild (0))) {
			delete i;
		}
	}

	i = NULL;

	for (list.first (); list.isValid (); ++ list) {
		QString s = QString::fromUtf8 ((*list).c_str ());
		if (s.startsWith ("_"))
			continue;

		if (ns == Xmms::Collection::COLLECTIONS) {
			i = new CollectionListItem (m_collections, ns, s);
		} else {
			i = new CollectionListItem (m_playlists, ns, s);
		}
	}

	return true;
}

