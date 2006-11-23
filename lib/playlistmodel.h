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


#ifndef __PLAYLIST_MODEL_H__
#define __PLAYLIST_MODEL_H__

#include <xmmsclient/xmmsclient++.h>

#include <QAbstractTableModel>
#include <QVariant>
#include <QHash>
#include <QIcon>

#include "xclient.h"

class PlaylistModel : public QAbstractItemModel
{
	Q_OBJECT
	public:
		PlaylistModel (QObject *parent, XClient *client);

		enum {
			DisplayRole = Qt::DisplayRole,
			ToolTipRole = Qt::ToolTipRole,
			CurrentEntryRole = 200,
			MedialibIdRole,
			AvailableRole
		};

		int rowCount (const QModelIndex &parent) const;
		int columnCount (const QModelIndex &parent) const;
		QVariant data (const QModelIndex &index, int role = DisplayRole) const;
		QVariant playlist_data (const QModelIndex &index, int role = DisplayRole) const;
		QVariant decoration_data (const QModelIndex &index, int role = DisplayRole) const;
		QVariant headerData (int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
		Qt::ItemFlags flags (const QModelIndex &) const;
		QModelIndex parent (const QModelIndex &) const;
		QModelIndex index (int row, int column, const QModelIndex &idx = QModelIndex ()) const;
		QMimeData *mimeData (const QModelIndexList &list) const;
		bool dropMimeData (const QMimeData * data, Qt::DropAction action, int row, int column, const QModelIndex & parent);
		Qt::DropActions supportedDropActions () const;
		QStringList mimeTypes () const;
		
		void setColumns (const QList <QString> &);
		void setColumnFallback (const QList <QString> &);

		QList<QString> columns () const;

		XClient *m_client;
		QList < unsigned int > m_plist;

		QList<uint32_t> get_all_id ();

		void set_cached_size (int i, const QSize &size) {
			m_cached_size[i] = size;
		};

		QSize cached_size (int i) const {
			return m_cached_size[i];
		};

		QModelIndexList get_idxlist_by_id (uint32_t);

		QModelIndex current_playlist_pos () const {
			return index (m_current_pos, 0);
		};

	signals:
		void entryMoved (const QModelIndex &, const QModelIndex &);

	public slots:
		void got_connection (XClient *);
		void entry_changed (uint32_t);

	private:
		bool handle_list (const Xmms::List< unsigned int > &list);
		bool handle_change (const Xmms::Dict &chg);
		bool handle_update_pos (const unsigned int &pos);

		void getInfo (unsigned int id) const;
		QList < uint32_t > getPosById (uint32_t id);

		QList < QString > m_columns;
		QList < QString > m_colfallback;

		uint32_t m_current_pos;

		QList<QSize> m_cached_size;

};

#endif
