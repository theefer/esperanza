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

#include "filehandler.h"
#include "fileengine.h"

RemoteFileHandler::RemoteFileHandler (XClient *client) : QAbstractFileEngineHandler ()
{
	m_client = client;
}

QAbstractFileEngine *
RemoteFileHandler::create (const QString &fileName) const
{
	if (fileName.toLower().startsWith ("//_xmms2"))
		return new RemoteFileEngine ("/_xmms2/", m_client, this);

	if (fileName.toLower().startsWith ("/_xmms2")) {
		if (fileName.count() < 8) {
			return new RemoteFileEngine ("/_xmms2/", m_client, this);
		} else {
			if (m_filemap.contains (fileName)) {
				return new RemoteFileEngine (fileName, m_client, this, m_filemap[fileName]);
			}
			return new RemoteFileEngine (fileName, m_client, this);
		}
	}
	return 0;
}

