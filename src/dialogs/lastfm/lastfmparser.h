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


#ifndef __LASTFM_PARSER_H__
#define __LASTFM_PARSER_H__

#include <QHttp>
#include <QHash>
#include <QBuffer>
#include <QUrl>
#include <QXmlSimpleReader>

#include "lastfmartist.h"

class LastFmParser : public QObject
{
	Q_OBJECT
	public:
		LastFmParser (QObject *);
		QList<LastFmArtist> similar_artist (const QString &artist);

		void abort () {
			m_http->abort ();
		};

	private slots:
		void request_done (int, bool);

	signals:
		void gotData (const QString &);
		void gotError (const QString &);

	private:
		int make_request (const QString &, const QUrl &);
		bool parse_xml (const QByteArray &b, int);

		QHttp *m_http;
		QHash< int, QBuffer * > m_requests;
		QHash< QString, QList<LastFmArtist> > m_cache;
		QHash< int, QString > m_cache_map;
		QXmlSimpleReader m_xml;
};

#endif
