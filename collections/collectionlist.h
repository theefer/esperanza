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

class CollectionList : public QTreeWidget
{
	Q_OBJECT
	public:
		CollectionList (QWidget *, Xmms::Collection::Namespace, XClient *);

	signals:
		void switch_view (const Xmms::Collection::Namespace &, const QString &);

	private slots:
		void active_row (QTreeWidgetItem *, int);

	private:
		bool list_cb (const Xmms::List<std::string> &);
		Xmms::Collection::Namespace m_ns;

};

#endif


