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

	connect (selectionModel (), SIGNAL (currentRowChanged (const QModelIndex &, const QModelIndex &)),
			 this, SLOT (item_selected (const QModelIndex &, const QModelIndex &)));

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

	if (!indexAt (ev->pos ()).isValid ()) {
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
		if (getSelection ().size () > 1)
			return;
		setExpanded (selectionModel ()->currentIndex (), true);
	}
}

void
FancyPlaylistView::item_selected (const QModelIndex &n, const QModelIndex &old)
{
	QSettings s;

	if (s.value ("playlist/compactmode").toBool ())
		return;

	if (n.internalId () != -1) {
		setCurrentIndex (n.parent ());
		return;
	}

	QModelIndexList l = getSelection ();
	
	if (l.count () < 1) {
		setCurrentIndex (old);
	} else if (l.count () > 1) {
		collapseAll ();
	} else {
		setCurrentIndex (n);

		collapseAll ();

		setExpanded (n, true);

		QModelIndex c = n.child (0, 0);
		scrollTo (c);
	}

	/* emit current id */
	if (l.count () > 1 || l.count () < 1) {
		emit selectedID (0);
	} else {
		emit selectedID (l[0].data (PlaylistModel::MedialibIdRole).toUInt ());
	}
}

