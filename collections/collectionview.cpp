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

#include "collections/collectionview.h"

#include <QTreeView>

#include "collections/collectionmodel.h"

CollectionView::CollectionView (QWidget *parent, XClient *client) : QTreeView (parent)
{
	m_model = new CollectionModel (this, client);
	setModel (m_model);

	connect (parent, SIGNAL (switch_view (const Xmms::Collection::Namespace &, const QString &)),
			 m_model, SLOT (view_switch (const Xmms::Collection::Namespace &, const QString &)));
}

