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


#ifndef __MEDIALIBSEARCHMODEL_H__
#define __MEDIALIBSEARCHMODEL_H__

#include "xclient.h"
#include "playlistmodel.h"

#include <QStandardItemModel>

class MedialibSearchModel : public PlaylistModel
{
	Q_OBJECT
	public:
		enum {
			SEARCH_ALL,
			SEARCH_ARTIST,
			SEARCH_ALBUM,
			SEARCH_TITLE,
			SEARCH_YEAR
		};

		MedialibSearchModel (QObject *parent, XClient *client);
		void do_search (uint32_t, const QString &, bool unav=false);
		void got_connection (XClient *client);

		QMimeData *mimeData (const QModelIndexList &list) const;

		Qt::DropActions supportedDropActions () const {
			return Qt::IgnoreAction;
		};
		QStringList mimeTypes () const {
			return QStringList ("application/x-xmms2mlibid");
		};


	signals:
		void searchDone ();

	private:
		bool handle_search (const Xmms::List<Xmms::Dict> &);
};

#endif
