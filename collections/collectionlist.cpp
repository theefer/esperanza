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

	m_collections = new CollectionListItem (this, Xmms::Collection::COLLECTIONS, tr ("Collections"));
	m_playlists = new CollectionListItem (this, Xmms::Collection::PLAYLISTS, tr ("Playlists"));

	client->collection.broadcastCollectionChanged () (Xmms::bind (&CollectionList::coll_changed, this));

	client->collection.list (Xmms::Collection::COLLECTIONS)(
							 boost::bind (&CollectionList::list_cb, this,
										  Xmms::Collection::COLLECTIONS, _1));

	client->collection.list (Xmms::Collection::PLAYLISTS)(
							 boost::bind (&CollectionList::list_cb, this,
										  Xmms::Collection::PLAYLISTS, _1));
	m_client = client;

    connect (this, SIGNAL (itemChanged (QTreeWidgetItem *, int)), this, SLOT (item_changed (QTreeWidgetItem *, int)));
}

bool
CollectionList::coll_changed (const Xmms::Dict &d)
{
    QString name;
    int type;
    Xmms::Collection::Namespace ns;
    
    if (d.contains ("name")) {
        name = XClient::stdToQ (d.get<std::string> ("name"));
    } else {
        qWarning ("No name in dict!");
        return true;
    }
    
    if (d.contains ("type")) {
        type = d.get<int32_t> ("type");
    } else {
        qWarning ("no type in dict!");
        return true;
    }
    
    if (d.contains ("namespace")) {
        std::string s = d.get<std::string> ("namespace");
        if (s == "Collections")
            ns = Xmms::Collection::COLLECTIONS;
        else
            ns = Xmms::Collection::PLAYLISTS;
    } else {
        qWarning ("no namespace in dict!");
        return true;
    }
    
    if (type == XMMS_COLLECTION_CHANGED_ADD ||
        type == XMMS_COLLECTION_CHANGED_RENAME ||
        type == XMMS_COLLECTION_CHANGED_REMOVE) {
            /*
             * It's ok for now to just reload the whole list.
             * THe numbers of collections and playlists shouldn't 
             * be that many and it won't affect performance that much.
             */
            m_client->collection.list (ns) (
                                       boost::bind (&CollectionList::list_cb, this,
                                       ns, _1));
    }
        
    return true;
}

void
CollectionList::item_add ()
{
    QTreeWidgetItem *parent = currentItem ();
    if (!parent) return; /* not selected */
    /* get parent */
    while (parent->parent ()) parent = parent->parent ();
    
    CollectionListItem *item = new CollectionListItem (dynamic_cast<CollectionListItem *> (parent), "New");
    item->setFlags (Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    setCurrentItem (item);
    editItem (item, 0);
}

void
CollectionList::item_changed (QTreeWidgetItem *item, int c)
{
    CollectionListItem *p = dynamic_cast<CollectionListItem *> (item);
    
    if (item->text (0) == "New") {
        return;
    }
    
    if (!p) {
        qWarning ("p is not defined!");
        return;
    }
        
    Xmms::Coll::Idlist coll;
    m_client->collection.save (coll, XClient::qToStd (p->text (0)), p->ns ()) ();
}


void
CollectionList::active_row (QTreeWidgetItem *item, int c)
{
	CollectionListItem *i = dynamic_cast<CollectionListItem *> (item);
	if (i) {
	    emit switch_view (i->ns (), i->text (0));
    }
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

