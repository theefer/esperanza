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
		
		int match () const {
			return m_attribs["match"].toInt ();
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
