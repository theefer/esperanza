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

#include "xclient.h"

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
    m_client = client;

	setVerticalScrollBarPolicy (Qt::ScrollBarAlwaysOn);
	
    m_model = new CollectionInfoModel (this, client, true);
    m_model->set_collection (Xmms::Coll::Universe (), QStringList (view), QStringList (view), QStringList (view));
    setModel (m_model);
    
    m_selection = new QItemSelectionModel (m_model);
    setSelectionModel (m_selection);
    
    m_parentcoll = &m_univ;
        
    connect (this, SIGNAL (clicked (const QModelIndex &)), this, SLOT (item_clicked (const QModelIndex &)));
    connect (this, SIGNAL (doubleClicked (const QModelIndex &)), this, SLOT (item_dbclicked (const QModelIndex &)));
}

void
PaneBrowserView::item_dbclicked (const QModelIndex &idx)
{
    const char * const order[] = {"artist", "album", "tracknr", "id"};
    static std::list<std::string> o (order, order + 4);
    
    QString val = idx.data(0).toString ();
    if (val == tr ("[ All ]")) {
        Xmms::Coll::Equals eq (*m_parentcoll, "status", "1");
        m_client->playlist ()->addCollection (eq, o) ();
    } else {
        Xmms::Coll::Equals eq (*m_parentcoll, XClient::qToStd (m_view),
                               XClient::qToStd (val));    
        m_client->playlist ()->addCollection (eq, o) ();                   
    }
}

void
PaneBrowserView::item_clicked (const QModelIndex &idx)
{
    QString val = idx.data(0).toString ();
    if (val == tr ("[ All ]")) {
        Xmms::Coll::Equals eq (*m_parentcoll, "status", "1");
        emit filter (eq);
    } else {
        Xmms::Coll::Equals eq (*m_parentcoll, XClient::qToStd (m_view),
                               XClient::qToStd (val));                       
        emit filter (eq);
    }
}

void
PaneBrowserView::item_filter (const Xmms::Coll::Equals &coll)
{
    QStringList sl (m_view);
    
    m_equals = coll;
    m_parentcoll = &m_equals;
    
    m_model->set_collection (coll, sl, sl, sl);
}

