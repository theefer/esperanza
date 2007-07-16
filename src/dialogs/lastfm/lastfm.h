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
		void settings_pressed (QMouseEvent *);

		void set_artists ();
		void set_top_albums ();
		void set_top_tracks ();

		void showEvent (QShowEvent *);

	private:
		QList< QLabel* > m_labels;
		QList< ValueBar* > m_values;
		QLabel *m_artistl;
		QHash< QString, LastFmArtist> m_artists;

		XClient *m_client;

		LastFmParser *m_parser;
		QProgressBar *m_pb;
		QLabel *m_pl;

        bool num_reply (Xmms::List<unsigned int> const &list, const QString &artist);

		QString m_current_artist;

		uint32_t m_current_type;
};

#endif
