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


#ifndef __LASTFM_ARTIST_H__
#define __LASTFM_ARTIST_H__

#include <QString>
#include <QHash>
#include <QMetaType>

class LastFmArtist
{
	public:
		LastFmArtist () { };
		void setNextAttribName (const QString &s) {
			m_nextattr = s;
		};
		void setNextAttrib (const QString &v) {
			if (!m_nextattr.isNull ()) {

				if (m_attribs.contains (m_nextattr))
					return;

				m_attribs.insert (m_nextattr, v);
			} 
		};

		QString name () const {
			return m_attribs["name"];
		};
		
		float match () const {
			return m_attribs["match"].toFloat ();
		};

		QString prop (const QString &s) const {
			return m_attribs[s];
		};

		bool contains (const QString &s) const {
			return m_attribs.contains (s);
		};

	private:
		QHash<QString, QString> m_attribs;
		QString m_nextattr;
};

Q_DECLARE_METATYPE(LastFmArtist)

#endif
