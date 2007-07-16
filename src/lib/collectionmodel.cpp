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

#include "collectionmodel.h"

#include <QAbstractItemModel>

CollectionModel::CollectionModel (QObject *parent, XClient *client) : QAbstractItemModel (parent)
{
	connect (client, SIGNAL (gotConnection (XClient *)), this, SLOT (got_connection (XClient *)));
	connect (client->cache (), SIGNAL(entryChanged (uint32_t)), this, SLOT (entry_changed (uint32_t)));

	m_client = client;

	m_columns.append ("Artist");
	m_columns.append ("Album");
	m_columns.append ("Title");

	m_cached_size.append (QSize ());
	m_cached_size.append (QSize ());
	m_cached_size.append (QSize ());
}

void
CollectionModel::entry_changed (uint32_t id)
{
	QList<uint32_t> pos = getPosById (id);
	for (int i = 0; i < pos.size (); i ++) {
		QModelIndex idx1 = index (pos.at (i), 0);
		QModelIndex idx2 = index (pos.at (i), m_columns.size ());
		emit dataChanged(idx1, idx2);
	}
}

QList<uint32_t>
CollectionModel::getPosById (uint32_t id)
{
	QList<uint32_t> ret;
	int32_t pos = m_plist.indexOf (id);

	while (pos != -1) {
		ret.append (pos);
		pos = m_plist.indexOf (id, pos + 1);
	}

	return ret;
}

void
CollectionModel::got_connection (XClient *client)
{
	m_client = client;
}

void
CollectionModel::set_collection (const Xmms::Coll::Coll &ref)
{
    static const char *o[] = {"artist", "album", "tracknr", "id"};
    std::list<std::string> order (o, o+4);
	m_client->collection ()->queryIds (ref, order) (Xmms::bind (&CollectionModel::id_list_get, this));
}

bool
CollectionModel::id_list_get (Xmms::List<unsigned int> const &list)
{
	beginRemoveRows (QModelIndex (), 0, m_plist.size ());
	m_plist.clear ();
	endRemoveRows ();

	int i = 0;
	for (list.first (); list.isValid (); ++list) {
		i ++;
	}

	beginInsertRows (QModelIndex (), 0, i);
	for (list.first (); list.isValid (); ++list) {
		m_plist.append (*list);
	}
	endInsertRows ();
	
    emit searchDone ();

	return true;
}

int
CollectionModel::rowCount (const QModelIndex &parent) const
{
	if (!parent.isValid ()) {
		return m_plist.size ();
	}
	return 0;
}

int
CollectionModel::columnCount (const QModelIndex &parent) const
{
	return 3;
}

QVariant
CollectionModel::data (const QModelIndex &index, int role) const
{
	if (index.row () >= m_plist.size ()) {
		return QVariant ();
	} else if (index.row () < 0) {
		return QVariant ();
	}
	
	if (role == MedialibIdRole) {
        return QVariant (m_plist[index.row ()]);
	}

	if (role == Qt::SizeHintRole) {
		if (m_cached_size[index.column ()].isValid ()) {
			return QVariant (m_cached_size[index.column ()]);
		}
		return QVariant ();
	}

	if (role == Qt::DisplayRole || role == Qt::ToolTipRole) {
		unsigned int id = m_plist[index.row ()];

		QString key = m_columns[index.column ()].toLower ();

		CollectionModel *fake = const_cast<CollectionModel*> (this);
		QHash<QString, QVariant> d = fake->m_client->cache ()->get_info (id);

		if (d.contains (key)) {
			return QVariant (d[key]);
		}

		return QVariant ();
	}

	return QVariant ();
}

QVariant
CollectionModel::headerData (int section, Qt::Orientation orientation, int role) const
{
	if (role != Qt::DisplayRole) {
		return QVariant ();
	}

	if (orientation == Qt::Horizontal) {
		if (section <= m_columns.size ())
			return QVariant (m_columns[section]);
	}

	return QVariant ();
}

Qt::ItemFlags
CollectionModel::flags (const QModelIndex &) const
{
	return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;
}

QModelIndex
CollectionModel::parent (const QModelIndex &idx) const
{
	return QModelIndex ();
}

QModelIndex
CollectionModel::index (int row, int column, const QModelIndex &parent) const 
{
	if (!parent.isValid ()) {
		if (row > (m_plist.size () - 1))
			return QModelIndex ();
		if (row < 0)
			return QModelIndex ();
		return createIndex (row, column, -1);
	}

	return QModelIndex ();
}

