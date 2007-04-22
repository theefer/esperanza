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



#ifndef __MEDIALIBVIEW_H__
#define __MEDIALIBVIEW_H__

#include "xclient.h"
#include "collectionmodel.h"

#include <QTreeView>
#include <QEvent>

class MedialibView : public QTreeView
{
	Q_OBJECT
	public:
		MedialibView (QWidget *, XClient *);

		void do_search (uint32_t p, const QString &s, bool a)
		{
		};

		QList<uint32_t> get_selection ();
		QList<uint32_t> get_all ();

		enum {
			SEARCH_ALL,
			SEARCH_ARTIST,
			SEARCH_TITLE,
			SEARCH_ALBUM,
			SEARCH_YEAR
		};

	private slots:
		void search_done ();
		void add_id (const QModelIndex &);
		void head_size (int, int, int);

	signals:
		void searchDone ();

	private:
		XClient *m_client;
		CollectionModel *m_model;
		QItemSelectionModel *m_selections;
};

#endif
