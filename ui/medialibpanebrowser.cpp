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
    
    connect (m_artists, SIGNAL (filter (const QString &)), m_albums, SLOT (item_filter (const QString &)));
    connect (m_artists, SIGNAL (filter (const QString &)), this, SLOT (artist_filter (const QString &)));
    connect (m_albums, SIGNAL (filter (const QString &)), this, SLOT (album_filter (const QString &)));
}

void
MedialibPaneBrowser::artist_filter (const QString &filter)
{
    m_parent->extern_search (MedialibSearchModel::ARTIST, filter);
}

void
MedialibPaneBrowser::album_filter (const QString &filter)
{
    m_parent->extern_search (MedialibSearchModel::ALBUM, filter);
}
