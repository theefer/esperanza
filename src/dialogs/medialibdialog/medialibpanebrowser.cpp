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

#include "medialibpanebrowser.h"
#include "medialibdialog.h"
#include "medialibsearchmodel.h"

#include <QHBoxLayout>

MedialibPaneBrowser::MedialibPaneBrowser (QWidget *parent, XClient *client) :
    QWidget (parent)
{
    m_client = client;
    m_box = new QHBoxLayout (this);
    m_parent = dynamic_cast<MedialibDialog*> (parent);
    
    m_artists = new PaneBrowserView (this, m_client, "artist");
    m_box->addWidget (m_artists);
    
    m_albums = new PaneBrowserView (this, m_client, "album");
    m_box->addWidget (m_albums);
    
    connect (m_artists, SIGNAL (filter (const Xmms::Coll::Equals &)),
             m_albums, SLOT (item_filter (const Xmms::Coll::Equals &)));
             
    connect (m_artists, SIGNAL (filter (const Xmms::Coll::Equals &)),
             this, SLOT (set_filter (const Xmms::Coll::Equals &)));
             
    connect (m_albums, SIGNAL (filter (const Xmms::Coll::Equals &)),
             this, SLOT (set_filter (const Xmms::Coll::Equals &)));
}

void
MedialibPaneBrowser::set_filter (const Xmms::Coll::Equals &coll)
{
    m_parent->set_list_coll (coll);
}
