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

#ifndef __MEDIALIB_SEARCH_MODEL_H__
#define __MEDIALIB_SEARCH_MODEL_H__

#include "xclient.h"
#include "collectionmodel.h"

class MedialibSearchModel : public CollectionModel
{
	Q_OBJECT
	public:
	    enum Key {
            ARTIST,
            ALBUM,
            TITLE,
            YEAR,
            ALL
        };
		MedialibSearchModel (QObject *parent, XClient *client);
        void search (uint32_t key, const QString &, const bool &);
    private:
        std::string key_as_string (uint32_t);
};

#endif

