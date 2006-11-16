#include "xmlhandler.h"

#include <QXmlContentHandler>

XmlHandler::XmlHandler () : QXmlDefaultHandler ()
{
}

bool
XmlHandler::characters (const QString &s)
{
	if (m_type == SIMILARARTIST) {
		m_artist.setNextAttrib (s);
	}

	return true;
}

bool
XmlHandler::endDocument ()
{
	return true;
}

bool
XmlHandler::endElement (const QString &namespaceURI, const QString &localName, const QString &qName)
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
XmlHandler::startElement (const QString &namespaceURI,
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

