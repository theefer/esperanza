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

#include "icecasthandler.h"
#include <QXmlDefaultHandler>

IcecastHandler::IcecastHandler () : QXmlDefaultHandler ()
{
    
}

bool
IcecastHandler::characters (const QString &s)
{
    if (!m_state == ENTRY) {
        return true;
    }
    
    if (s.isEmpty ()) {
        return true;
    }
    
    switch (m_entry_state) {
        case NAME:
            m_current.setName (s);
            break;
        case URL:
            m_current.setUrl (s);
            break;
        case GENRE:
            m_current.setGenre (s);
            break;
        case BITRATE:
            m_current.setBitrate (s);
            break;
        default:
            break;
    }
	return true;
}

bool
IcecastHandler::endDocument ()
{
	return true;
}

bool
IcecastHandler::endElement (const QString &namespaceURI,
                            const QString &localName,
                            const QString &qName)
{
    if (qName == "entry") {
        /*
        qDebug ("done with %s (url: %s, br: %s, genre: %s)", qPrintable (m_current.name ()), qPrintable (m_current.url ()), qPrintable (m_current.bitrate()), qPrintable (m_current.genre ()));
        */
        if (!m_current.name ().isEmpty ()) {
            m_channels.append (IcecastChannel (m_current));
        }
        m_current.reset ();
        m_state = EMPTY;
    }
    m_entry_state = EMPTY;
    return true;
}

bool
IcecastHandler::startElement (const QString &namespaceURI,
							  const QString &localName,
							  const QString &qName,
							  const QXmlAttributes &atts)
{
    if (qName == "entry") {
        m_state = ENTRY;
        return true;
    }
    
    if (m_state == ENTRY) {
        if (qName == "server_name") {
            m_entry_state = NAME;
        } else if (qName == "listen_url") {
            m_entry_state = URL;
        } else if (qName == "bitrate") {
            m_entry_state = BITRATE;
        } else if (qName == "genre") {
            m_entry_state = GENRE;
        } else {
            m_entry_state = EMPTY;
        }
    }
	return true;
}
