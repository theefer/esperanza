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

#include <xmmsclient/xmmsclient++.h>

#include "medialibsearchmodel.h"

MedialibSearchModel::MedialibSearchModel (QObject *parent, XClient *client) : CollectionModel (parent, client)
{
    
}

std::string
MedialibSearchModel::key_as_string (uint32_t key)
{
    if (key == ARTIST) {
        return std::string ("artist");
    } else if (key == ALBUM) {
        return std::string ("album");
    } else if (key == TITLE) {
        return std::string ("title");
    } else if (key == YEAR) {
        return std::string ("year");
    }
    
    qDebug ("not supported!");
    return std::string ();
}

void
MedialibSearchModel::search (uint32_t key, const QString &search, const bool &active)
{
    QString sp (search);
    sp.push_front ('%');
    sp.push_back ('%');
    
    std::string val = XClient::qToStd (sp);
    
    /* create a collection */
    Xmms::Coll::Universe un;
    Xmms::Coll::Coll *univ = &un;
    Xmms::Coll::Equals eq (un, "status", "1");
    
    if (!active) {
        univ = &eq;
    }
    
    if (key == ALL) {
        Xmms::Coll::Union uni;
        Xmms::Coll::Match mart (*univ, "artist", val);
        Xmms::Coll::Match malb (*univ, "album", val);
        Xmms::Coll::Match mtit (*univ, "title", val);
        
        uni.addOperand (mart);
        uni.addOperand (malb);
        uni.addOperand (mtit);
            
        set_collection (uni);
    } else {
        Xmms::Coll::Match match (*univ, key_as_string (key), val);
        set_collection (match);
    }
    
}

