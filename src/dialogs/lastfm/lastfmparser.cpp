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


#include "lastfmparser.h"
#include "lastfmhandler.h"
#include "lastfmartist.h"

#include <QHttp>

#define ARTIST_SIMILAR "http://ws.audioscrobbler.com/1.0/artist/%1/similar.xml"

LastFmParser::LastFmParser (QObject *parent) : QObject (parent)
{
	m_http = new QHttp (this);
	connect (m_http, SIGNAL (requestFinished (int, bool)), this, SLOT (request_done (int, bool)));
	m_xml.setContentHandler (new LastFmHandler ());
}

void
LastFmParser::request_done (int id, bool err)
{
	if (err) {
		m_cache.remove (m_cache_map[id]);
		m_cache_map.remove (id);
		emit gotError (m_cache_map[id]);
		return;
	}
	QBuffer *b = m_requests.take (id);

	if (!b) {
		return;
	}
	
	parse_xml (b->buffer (), id);
	delete b;
}

bool
LastFmParser::parse_xml (const QByteArray &b, int id)
{
	LastFmHandler *x = dynamic_cast<LastFmHandler*> (m_xml.contentHandler ());
	QXmlInputSource s;
	s.setData (b);

	x->reset ();

	m_xml.parse (s);

	if (x->type () == LastFmHandler::SIMILARARTIST) {
		QList<LastFmArtist> l = x->artist_list ();
		m_cache[m_cache_map[id]] = l;
		emit gotData (m_cache_map[id]);
	}
	return true;
}

int
LastFmParser::make_request (const QString &artist,
							const QUrl &url)
{
	QBuffer *buff = new QBuffer (this);
	buff->open (QIODevice::WriteOnly);

	m_http->setHost (url.host (), url.port () != -1 ? url.port () : 80);
	int id = m_http->get (QString (url.toEncoded ()), buff);

	m_requests[id] = buff;
	m_cache_map[id] = artist;

	return id;
}

QList<LastFmArtist>
LastFmParser::similar_artist (const QString &artist)
{
	if (m_cache.contains (artist)) {
		return m_cache[artist];
	}

	m_cache[artist] = QList<LastFmArtist> ();

	QString q = QString (ARTIST_SIMILAR).arg (artist);
	make_request (artist, QUrl (q));

	return QList<LastFmArtist> ();
}

