#ifndef __LASTFM_H__
#define __LASTFM_H__

#include "xclient.h"
#include "lastfmparser.h"
#include "lastfmartist.h"
#include "valuebar.h"

#include <QDialog>
#include <QProgressBar>
#include <QLabel>

class LastFmDialog : public QDialog
{
	Q_OBJECT
	public:
		LastFmDialog (QWidget *, XClient *);

	public slots:
		void new_id (uint32_t);
		void entry_update (uint32_t);

	private slots:
		void update_artists (const QString &);
		void update_error (const QString &);
		void link_context (const QString &);

	private:
		QList< QLabel* > m_labels;
		QList< ValueBar* > m_values;
		QLabel *m_artistl;
		QHash< QString, LastFmArtist> m_artists;

		XClient *m_client;

		LastFmParser *m_parser;
		QProgressBar *m_pb;
		QLabel *m_pl;

		uint32_t m_current;

		bool num_reply (const Xmms::List <Xmms::Dict> &, const QString &);

		QStringList m_has_mlib;
		QString m_current_artist;
};

#endif
