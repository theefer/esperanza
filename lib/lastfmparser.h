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
