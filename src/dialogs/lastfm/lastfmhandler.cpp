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


#include "lastfmhandler.h"

#include <QXmlContentHandler>

LastFmHandler::LastFmHandler () : QXmlDefaultHandler ()
{
}

bool
LastFmHandler::characters (const QString &s)
{
	if (m_type == SIMILARARTIST) {
		m_artist.setNextAttrib (s);
	}

	return true;
}

bool
LastFmHandler::endDocument ()
{
	return true;
}

bool
LastFmHandler::endElement (const QString &namespaceURI, const QString &localName, const QString &qName)
{
	if (qName == "artist") {
		m_artists.append (m_artist);
		if (m_artists.count () > 19)
			return false;
	} else {
		m_artist.setNextAttribName (QString ());
	}
	return true;
}

bool
LastFmHandler::startElement (const QString &namespaceURI,
							 const QString &localName,
							 const QString &qName,
							 const QXmlAttributes &atts)
{
	if (qName == "similarartists") {
		m_type = SIMILARARTIST;
	}

	if (m_type == SIMILARARTIST) {
		if (qName == "artist") {
			m_artist = LastFmArtist ();
		} else if (qName == "name") {
			m_artist.setNextAttribName ("name");
		} else if (qName == "mbid") {
			m_artist.setNextAttribName ("mbid");
		} else if (qName == "match") {
			m_artist.setNextAttribName ("match");
		} else if (qName == "url") {
			m_artist.setNextAttribName ("url");
		} else if (qName == "image") {
			m_artist.setNextAttribName ("image");
		} else {
			/* we don't want it */
			m_artist.setNextAttribName (QString ());
		}
	}
	return true;
}

