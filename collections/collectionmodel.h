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

#ifndef __COLLECTIONMODEL_H__
#define __COLLECTIONMODEL_H__

#include "xclient.h"

#include <QAbstractItemModel>

class CollectionModel : public QAbstractItemModel
{
	Q_OBJECT
	public:
		CollectionModel (QObject *parent, XClient *client);

		int rowCount (const QModelIndex &parent) const;
		int columnCount (const QModelIndex &parent) const;
		QVariant data (const QModelIndex &index, int role = Qt::DisplayRole) const;
		QVariant headerData (int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
		Qt::ItemFlags flags (const QModelIndex &) const;
		QModelIndex parent (const QModelIndex &) const;
		QModelIndex index (int row, int column, const QModelIndex &idx = QModelIndex ()) const;
};

#endif

