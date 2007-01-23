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

#ifndef __COLLECTIONLIST_H__
#define __COLLECTIONLIST_H__

#include "xclient.h"
#include <QTreeWidget>

class CollectionListItem : public QTreeWidgetItem
{
	public:
		CollectionListItem (QTreeWidgetItem *parent,
							const Xmms::Collection::Namespace &ns,
							const QString &name) :
			QTreeWidgetItem (parent, QStringList (name))
		{
			m_ns = ns;
		};
		
		CollectionListItem (QTreeWidget *parent,
		                    const Xmms::Collection::Namespace &ns,
		                    const QString &name) :
	        QTreeWidgetItem (parent, QStringList (name))
	    {
	        m_ns = ns;
	    };
	    
	    CollectionListItem *parent () const {
	        return dynamic_cast<CollectionListItem *> (QTreeWidgetItem::parent ());
	    };

		Xmms::Collection::Namespace ns () const {
			return m_ns;
		};

	private:
		Xmms::Collection::Namespace m_ns;
};

class CollectionList : public QTreeWidget
{
	Q_OBJECT
	public:
		CollectionList (QWidget *, XClient *);

	signals:
		void switch_view (const Xmms::Collection::Namespace &, const QString &);

	private slots:
		void active_row (QTreeWidgetItem *, int);
		void item_changed (QTreeWidgetItem *, int);

	private:
		bool list_cb (const Xmms::Collection::Namespace &,
					  const Xmms::List<std::string> &);
					  
		bool coll_changed (const Xmms::Dict &);

		CollectionListItem *m_collections;
		CollectionListItem *m_playlists;
		XClient *m_client;

};

#endif


