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


#include "fancyplaylistview.h"
#include "fancyplaylistmodel.h"
#include "xclient.h"

#include <QHeaderView>
#include <QPainter>
#include <QSettings>
#include <QDebug>

PlaylistDelegate::PlaylistDelegate (QObject *parent, PlaylistModel *model) : QItemDelegate (parent)
{
	m_model = model;
}

void
PlaylistDelegate::paint (QPainter *painter,
						 const QStyleOptionViewItem &option,
						 const QModelIndex &index) const
{
	QSettings s;
	
	QStyleOptionViewItem o (option);
	if (index.data (PlaylistModel::CurrentEntryRole).toBool ()) {
		QFont f (o.font);
		f.setBold (true);
		o.font = f;
	}
	if (index.internalId() != -1) {
		o.state |= QStyle::State_Selected;
		if (s.value ("ui/contextareabright").toBool ()) {
			QPalette p (o.palette);
			p.setColor (QPalette::Highlight, p.highlight ().color ().light ());
			o.palette = p;
		}
	} 

	QItemDelegate::paint (painter, o, index);
}

FancyPlaylistView::FancyPlaylistView (QWidget *parent, XClient *client) : PlaylistView (parent, client)
{
	FancyPlaylistModel *model = new FancyPlaylistModel (this, m_client);
	setModel (model);

	setItemDelegate (new PlaylistDelegate (this, model));

	connect (selectionModel (), SIGNAL (selectionChanged (const QItemSelection &, const QItemSelection &)),
			 this, SLOT (item_selected (const QItemSelection &, const QItemSelection &)));

	connect (this, SIGNAL (doubleClicked (const QModelIndex &)),
			 this, SLOT (jump_pos (const QModelIndex &)));

	connect (m_client->settings (), SIGNAL (settingsChanged ()),
			 this, SLOT (changed_settings ()));

	setIconSize (QSize (75, 75));
}

void
FancyPlaylistView::mousePressEvent (QMouseEvent *ev)
{
	if (ev->buttons () & Qt::RightButton || ev->buttons () & Qt::MidButton) {
		ev->ignore ();
		return;
	}

	if (!indexAt (ev->pos ()).isValid () || indexAt (ev->pos ()).internalId () != -1) {
		ev->ignore ();
		return;
	}

	QTreeView::mousePressEvent (ev);
}

void
FancyPlaylistView::changed_settings ()
{
	QSettings s;

	collapseAll ();

	if (!s.value ("playlist/compactmode").toBool ()) {
		QModelIndexList l = getSelection ();
		if (l.size () < 1 || l.size () > 1)
			return;
		
		setExpanded (l[0], true);
	}
}

void
FancyPlaylistView::item_selected (const QItemSelection & selected, const QItemSelection & deselected)
{
	QSettings s;

	if (s.value ("playlist/compactmode").toBool ())
		return;

	collapseAll ();

	QModelIndexList l = getSelection ();

	/* emit current id */
	if (l.count () > 1 || l.count () < 1) {
		emit selectedID (0);
	} else {
		QModelIndex n = l[0];
		
		if (n.internalId () != -1) {
			setCurrentIndex (n.parent ());
			return;
		}

		setExpanded (n, true);

		QModelIndex c = n.child (0, 0);
		scrollTo (c);
		
		emit selectedID (n.data (PlaylistModel::MedialibIdRole).toUInt ());
	}
	
	update ();
}

