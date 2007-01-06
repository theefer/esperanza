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

#include "collections/collectionmodel.h"

#include <QAbstractItemModel>

CollectionModel::CollectionModel (QObject *parent, XClient *client) : QAbstractItemModel (parent)
{
}

int
CollectionModel::rowCount (const QModelIndex &parent) const
{
	return 1;
}

int
CollectionModel::columnCount (const QModelIndex &parent) const
{
	return 1;
}

QVariant
CollectionModel::data (const QModelIndex &index, int role) const
{
	if (role == Qt::DisplayRole && index.column () == 0)
		return QVariant ("hej");
	return QVariant ();
}

QVariant
CollectionModel::headerData (int section, Qt::Orientation orientation, int role) const
{
	if (orientation == Qt::Horizontal)
		return QVariant ("Track");
	return QVariant ();
}

Qt::ItemFlags
CollectionModel::flags (const QModelIndex &) const
{
}

QModelIndex
CollectionModel::parent (const QModelIndex &) const
{
	return QModelIndex ();
}

QModelIndex
CollectionModel::index (int row, int column, const QModelIndex &idx) const 
{
	return QModelIndex ();
}

