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


#include "playlistview.h"
#include "fancyplaylistmodel.h"
#include "playlistmodel.h"
#include "xclient.h"

#include <QHeaderView>
#include <QPainter>
#include <QSettings>

PlaylistView::PlaylistView (QWidget *parent, XClient *client) : QTreeView (parent)
{
	m_client = client;
	m_parent = parent;

	setIndentation (0);
	setAlternatingRowColors (true);
	setItemsExpandable (false);
	setRootIsDecorated (false);
	setTabKeyNavigation (false);
	setTextElideMode (Qt::ElideNone);

	setVerticalScrollBarPolicy (Qt::ScrollBarAlwaysOn);

	setDragEnabled (true);
	setAcceptDrops (true);
	setDropIndicatorShown (true);
//	setDragDropMode (QAbstractItemView::DragDrop);

    setSelectionMode (QAbstractItemView::ExtendedSelection);
    setSelectionBehavior (QAbstractItemView::SelectRows);

	connect (client, SIGNAL(gotConnection (XClient *)),
			 this, SLOT (got_connection (XClient *))); 

	setIconSize (QSize (75, 75));

	QHeaderView *head = header ();
	connect (head, SIGNAL (sectionResized (int, int, int)), this, SLOT (head_size (int, int, int)));
}

void
PlaylistView::setModel (QAbstractItemModel *model)
{
	m_model = dynamic_cast<PlaylistModel *> (model);
	QTreeView::setModel (m_model);

	connect (m_model, SIGNAL (rowsInserted (const QModelIndex &, int, int)),
			 this, SLOT (rows_inserted ()));

	connect (m_model, SIGNAL (entryMoved (const QModelIndex &, const QModelIndex &)),
			 this, SLOT (moved (const QModelIndex &, const QModelIndex &)));

    m_selections = new QItemSelectionModel (m_model);
	setSelectionModel (m_selections);

	QHeaderView *head = header ();
	QSettings s;
	head->setResizeMode (0, QHeaderView::Interactive);
	head->resizeSection (0, s.value ("playlist/section0", 180).toInt ());
}

void
PlaylistView::rows_inserted ()
{
	QModelIndex idx = m_model->index (0, 0);
	if (!idx.isValid ())
		return;
	if (!m_model->cached_size (idx.column ()).isValid () && idx.internalId () == -1) {
		m_model->set_cached_size (idx.column (), sizeHintForIndex (idx));
	}
	idx = m_model->index (0, 1);
	if (!m_model->cached_size (idx.column ()).isValid () && idx.internalId () == -1) {
		m_model->set_cached_size (idx.column (), sizeHintForIndex (idx));
	}
}

void
PlaylistView::keyPressEvent (QKeyEvent *ev)
{
	QModelIndex idx = m_selections->currentIndex ();
	QModelIndex nidx;

	switch (ev->key ()) {
		case Qt::Key_Up:
			nidx = m_model->index (idx.row () - 1, idx.column (), QModelIndex ());
			break;
		case Qt::Key_Down:
			nidx = m_model->index (idx.row () + 1, idx.column (), QModelIndex ());
			break;
		case Qt::Key_PageDown:
		case Qt::Key_PageUp:
			QTreeView::keyPressEvent (ev);
			return;
		default:
			ev->ignore ();
			return;
	}

	if (nidx.isValid ())
		setCurrentIndex (nidx);
}

void
PlaylistView::moved (const QModelIndex &o, const QModelIndex &n)
{
	if (m_selections->isSelected (o)) {
		/* why the fuc??? */
//		m_selections->select (o, QItemSelectionModel::Deselect | QItemSelectionModel::Rows);
//		m_selections->select (n, QItemSelectionModel::ClearAndSelect  | QItemSelectionModel::Rows);
		setCurrentIndex (n);
	}
}

void
PlaylistView::got_connection (XClient *client)
{
	m_client = client;
	client->playback ()->broadcastCurrentID () (Xmms::bind (&PlaylistView::handle_update_pos, this));
	client->playback ()->currentID () (Xmms::bind (&PlaylistView::handle_update_pos, this));
	
	client->playback ()->getStatus () (Xmms::bind (&PlaylistView::handle_status, this));
	client->playback ()->broadcastStatus () (Xmms::bind (&PlaylistView::handle_status, this));
}

bool
PlaylistView::handle_status (const Xmms::Playback::Status &st)
{
	m_status = st;
	return true;
}

bool
PlaylistView::handle_update_pos (const uint32_t &id)
{
	QSettings s;

	QModelIndex idx = m_model->current_playlist_pos ();
	if (idx.isValid () && s.value ("playlist/jumptocurrent").toBool ())
		setCurrentIndex (idx);

	return true;
}

QModelIndexList
PlaylistView::getSelection ()
{
	QModelIndexList lst = m_selections->selectedIndexes ();
	QModelIndexList ret;

	for (int i = 0; i < lst.size (); i++) {
		QModelIndex idx = lst.at (i);
		if (idx.column () != 0)
			continue;

		ret.append (idx);
	}

	return ret;
}

void
PlaylistView::head_size (int c, int o, int n)
{
	if (c == 0) {
		QSettings s;
		s.setValue ("playlist/section0", n);
	}
}

void
PlaylistView::jump_pos (const QModelIndex &i)
{
	QModelIndex idx = i;

	if (!idx.isValid ())
		idx = currentIndex ();

	uint32_t row = idx.row ();
	if (idx.internalId () != -1)
		row = idx.parent ().row ();

	m_client->playlist ()->setNext (row) ();
	/* Note. tickle before checking status is a good
	 * idea here. It seems to bork on linux platform
	 * otherwise
	 */
	m_client->playback ()->tickle () ();

	if (m_status != Xmms::Playback::PLAYING) {
		m_client->playback ()->start () ();

		if (m_status == Xmms::Playback::PAUSED) {
			m_client->playback ()->tickle () ();
		}
	}

}

