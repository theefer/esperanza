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



#include "medialibview.h"
#include "medialibsearchmodel.h"
#include <QSettings>
#include <QHeaderView>

MedialibView::MedialibView (QWidget *parent, XClient *client) : QTreeView (parent)
{
	m_client = client;
	m_model = new MedialibSearchModel (this, client);
	setModel (m_model);

	setIndentation (0);
	setAlternatingRowColors (true);
	setItemsExpandable (false);
	setRootIsDecorated (false);

    setSelectionMode (QAbstractItemView::ExtendedSelection);
    setSelectionBehavior (QAbstractItemView::SelectRows);

    m_selections = new QItemSelectionModel (m_model);
	setSelectionModel (m_selections);

	QHeaderView *head = header ();
	QSettings s;

	head->resizeSection (0, s.value ("medialib/section0", 150).toInt ());
	head->resizeSection (1, s.value ("medialib/section1", 150).toInt ());
	connect (head, SIGNAL (sectionResized (int, int, int)), this, SLOT (head_size (int, int, int)));

	setIconSize (QSize (75, 75));

	connect (m_model, SIGNAL (searchDone ()), this, SLOT (search_done ()));

	connect (this, SIGNAL (doubleClicked (const QModelIndex &)),
			 this, SLOT (add_id (const QModelIndex &)));
}

void
MedialibView::head_size (int c, int o, int n)
{
	QSettings s;
	s.setValue (QString ("medialib/section%1").arg (c), n);
}

void
MedialibView::add_id (const QModelIndex &idx)
{
	uint32_t id = idx.data (CollectionModel::MedialibIdRole).toUInt ();
    m_client->playlist ()->addId (id) ();
}

void
MedialibView::search_done ()
{
	emit searchDone ();
}

QList<uint32_t>
MedialibView::get_selection ()
{
	QList<uint32_t> ret;

	QModelIndexList lst = m_selections->selectedIndexes ();
	for (int i = 0; i < lst.size (); i++) {
		QModelIndex idx = lst.at (i);
		if (idx.column () != 0)
			continue;

		ret.append (idx.data (CollectionModel::MedialibIdRole).toUInt ());
	}

	return ret;
}

QList<uint32_t>
MedialibView::get_all ()
{
    return m_model->get_idlist ();
}
