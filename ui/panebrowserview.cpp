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

#include "panebrowserview.h"
#include "collectioninfomodel.h"

#include <QTreeView>

PaneBrowserView::PaneBrowserView (QWidget *parent, XClient *client, const QString &view) : QTreeView (parent)
{
	setIndentation (0);
	setAlternatingRowColors (true);
	setItemsExpandable (false);
	setRootIsDecorated (false);
	setTabKeyNavigation (false);
	setTextElideMode (Qt::ElideNone);
	
    m_view = view;

	setVerticalScrollBarPolicy (Qt::ScrollBarAlwaysOn);
	
    m_model = new CollectionInfoModel (this, client, true);
    m_model->set_collection (Xmms::Coll::Universe (), QStringList (view), QStringList (view), QStringList (view));
    setModel (m_model);
    
    m_selection = new QItemSelectionModel (m_model);
    setSelectionModel (m_selection);
    
    connect (this, SIGNAL (clicked (const QModelIndex &)), this, SLOT (item_clicked (const QModelIndex &)));
}

void
PaneBrowserView::item_clicked (const QModelIndex &idx)
{
    emit filter (idx.data(0).toString ());
}

void
PaneBrowserView::item_filter (const QString &filter)
{
    QString f (filter);
    f.push_front ("%");
    f.push_back ("%");
    
    Xmms::Coll::Universe un;
    /* FIXME: shortcut fulhack */
    if (filter != tr ("[ All ]")) {
        Xmms::Coll::Match match (un, "artist", XClient::qToStd (f));
        m_model->set_collection (match, QStringList (m_view), QStringList (m_view), QStringList (m_view));
    } else {
        m_model->set_collection (un, QStringList (m_view), QStringList (m_view), QStringList (m_view));
    }
}