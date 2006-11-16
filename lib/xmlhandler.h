#ifndef __XMLHANDLER_H__
#define __XMLHANDLER_H__

#include <QXmlDefaultHandler>
#include "lastfmartist.h"

class XmlHandler : public QXmlDefaultHandler
{
	public:
		XmlHandler ();

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
