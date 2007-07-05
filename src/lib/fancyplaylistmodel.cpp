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


#include <xmmsclient/xmmsclient++.h>
#include <QAbstractTableModel>
#include <QHash>
#include <QVariant>
#include <QIcon>
#include <QMimeData>
#include <QSettings>
#include <QUrl>
#include <QFileInfo>
#include <QMessageBox>

#include "fancyplaylistmodel.h"

FancyPlaylistModel::FancyPlaylistModel (QObject *parent, XClient *client) : PlaylistModel (parent, client)
{
	QStringList c;
	c.append ("Artist");
	c.append ("Title");

	setColumns (c);

	c.clear ();

	c.append ("");
	c.append ("url");

	setColumnFallback (c);
}

int
FancyPlaylistModel::rowCount (const QModelIndex &parent) const
{
	int i = PlaylistModel::rowCount (parent);

	if (i == 0)
		i = 1;

	return i;
}

QModelIndex
FancyPlaylistModel::parent (const QModelIndex &idx) const
{
	if (!idx.isValid ())
		return QModelIndex ();

	if (idx.internalId () == -1) {
		return QModelIndex ();
	}

	return createIndex (idx.internalId (), idx.column (), -1);
}

QModelIndex
FancyPlaylistModel::index (int row, int column,
					  const QModelIndex &parent) const
{
	if (!parent.isValid ()) {
		if (row > (m_plist.size () - 1))
			return QModelIndex ();
		if (row < 0)
			return QModelIndex ();
		return createIndex (row, column, -1);
	}

	return createIndex (row, column, parent.row ());
}

QVariant
FancyPlaylistModel::data (const QModelIndex &index, int role) const
{
	if (!index.isValid ()) {
		return QVariant ();
	}

	if (index.internalId () == -1) {
		return PlaylistModel::data (index, role);
	} else {
		return decoration_data (index, role);
	}

	return QVariant ();
}

QVariant
FancyPlaylistModel::decoration_data (const QModelIndex &index, int role) const
{
	unsigned int r = index.parent ().row ();
	unsigned int id = m_plist[r];
	QHash<QString, QVariant> h = m_client->cache ()->get_info (id);

	if (role == Qt::TextAlignmentRole) {
		return QVariant (Qt::AlignVCenter);
	}

	if (role == Qt::DisplayRole) {
		if (index.column () == m_columns.size () - 1) {
			QString str;
			QSettings s;

			QStringList val = s.value ("ui/contextvalues").toString ().split (",");

			str.append ("\n");
			for (int i = 0; i < val.count (); i ++) {
				if (val.at (i) == "bitrate") {
					str.append (QString (tr("Bitrate: %0kbps")).arg (h["bitrate"].toUInt () / 1000));
				} else if (val.at (i) == "duration") {
					unsigned int dur = h["duration"].toUInt ();
					QString m;
					m.sprintf ("%02d:%02d", (dur / 60000), (dur/1000)%60);
					str.append (QString (tr("Duration: %0")).arg (m));
				} else if (val.at (i) == "timesplayed") {
					str.append (QString (tr("Times played: %0")).arg (h["timesplayed"].toUInt ()));
				} else if (val.at (i) == "album") {
					str.append (QString (tr("Album: %0")).arg (h["album"].toString ()));
				} else if (val.at (i) == "artist") {
					str.append (QString (tr("Artist: %0")).arg (h["artist"].toString ()));
				} else if (val.at (i) == "title") {
					str.append (QString (tr("Title: %0")).arg (h["title"].toString ()));
				} else if (val.at (i) == "genre") {
					str.append (QString (tr("Genre: %0")).arg (h["genre"].toString ()));
				} else if (val.at (i) == "tracknr") {
					str.append (QString (tr("Track Number: %0")).arg (h["tracknr"].toUInt ()));
				} else if (val.at (i) == "filename") {
					str.append (QString (tr("Filename: %0")).arg (h["url"].toString ()));
				}
				str += "\n";
			}
			return QVariant (str);
		}
		
	}
	if (role == Qt::DecorationRole) {
		QSettings s;
		int c = 0;

		if (!s.value ("playlist/albumartplace").toBool ())
			c = 1;
		
		if (index.column () == c) {
			QIcon i = m_client->cache ()->get_icon (id);
			return i;
		}
	}
	return QVariant ();
}

Qt::ItemFlags
FancyPlaylistModel::flags (const QModelIndex &idx) const
{
	if (!idx.isValid ())
		return Qt::ItemIsEnabled;

	if (idx.internalId () == -1) {
		return PlaylistModel::flags (idx);
	}

	return Qt::ItemIsEnabled;
}

