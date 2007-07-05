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


#ifndef __LASTFMHANDLER_H__
#define __LASTFMHANDLER_H__

#include <QXmlDefaultHandler>
#include "lastfmartist.h"

class LastFmHandler : public QXmlDefaultHandler
{
	public:
		LastFmHandler ();

		enum {
			NONE,
			SIMILARARTIST
		};

		bool characters (const QString &s);
		bool endDocument ();
		bool endElement (const QString &, const QString &, const QString &);
		bool startElement (const QString &, const QString &, const QString &, const QXmlAttributes &);

		int type () const {
			return m_type;
		};

		QList<LastFmArtist> artist_list () const {
			return m_artists;
		};

		void reset () {
			m_artist = LastFmArtist ();
			m_artists.clear ();
		};

	private:
		LastFmArtist m_artist;
		QList<LastFmArtist> m_artists;

		int m_type;
};

#endif
