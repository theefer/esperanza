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

CollectionList::CollectionList (QWidget *parent, Xmms::Collection::Namespace ns, XClient *client) : QTreeWidget (parent)
{
	setColumnCount (1);
	setHeaderLabels (QStringList ("Collections"));
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

	connect (this, SIGNAL (itemClicked (QTreeWidgetItem *, int)),
			 this, SLOT (active_row (QTreeWidgetItem *, int)));

	client->collection.list (ns, Xmms::bind (&CollectionList::list_cb, this));
	m_ns = ns;
}

void
CollectionList::active_row (QTreeWidgetItem *item, int c)
{
	emit switch_view (m_ns, item->text (0));
}

bool
CollectionList::list_cb (const Xmms::List<std::string> &list)
{
	QTreeWidgetItem *i;
	clear ();

	if (m_ns == Xmms::Collection::COLLECTIONS) {
		i = new QTreeWidgetItem (QStringList ("Universe"));
		addTopLevelItem (i);
	}

	for (list.first (); list.isValid (); ++ list) {
		QString s = QString::fromUtf8 ((*list).c_str ());
		if (s.startsWith ("_"))
			continue;
		i = new QTreeWidgetItem (QStringList (s));
		addTopLevelItem (i);
	}

	return true;
}

