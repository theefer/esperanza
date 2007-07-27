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


#include "lastfm.h"
#include "lastfmparser.h"
#include "valuebar.h"
#include "lastfmartist.h"
#include "playerbutton.h"
#include "playerwidget.h"

#include <QDialog>
#include <QSettings>
#include <QGridLayout>
#include <QPixmap>
#include <QProgressBar>
#include <QMenu>
#include <QCursor>
#include <QDesktopServices>

#define LASTFM_SEARCH_URL "http://www.last.fm/music/?m=artists&q=%1"
#define OINK_SEARCH_URL   "http://oink.cd/browse.php?search=%1"
#define MUSICBRAINZ_URL   "http://musicbrainz.org/artist/%1.html"

LastFmDialog::LastFmDialog (QWidget *parent, XClient *client) : QDialog (parent)
{
	QGridLayout *grid = new QGridLayout (this);
	setLayout (grid);

	setWindowTitle ("Esperanza - Last.fm browser");

	m_client = client;

	QLabel *l = new QLabel (this);
	l->setPixmap (QPixmap (":images/lastfm.png"));
	l->setAlignment (Qt::AlignCenter);
	grid->addWidget (l, 0, 0, 2, 1);

	l = new QLabel (tr ("Similar artists to:"), this);
	l->setAlignment (Qt::AlignCenter);
	grid->addWidget (l, 0, 1, 1, 3);

	m_artistl = new QLabel (tr ("None"), this);
	m_artistl->setAlignment (Qt::AlignCenter);
	grid->addWidget (m_artistl, 1, 1, 1, 3);
	connect (m_artistl, SIGNAL (linkActivated (const QString &)),
			 this, SLOT (link_context (const QString &)));

	QFrame *f = new QFrame (this);
	f->setFrameStyle (QFrame::Plain | QFrame::HLine);
	grid->addWidget (f, 2, 0, 1, 4);

	QWidget *bottom = new QWidget (this);
	QGridLayout *grid2 = new QGridLayout (bottom);
	bottom->setLayout (grid2);

	for (int i = 0; i < 10; i ++) {
		l = new QLabel (tr ("No data ..."), bottom);
		grid2->addWidget (l, i, 0, 1, 2);
		connect (l, SIGNAL (linkActivated (const QString &)),
				 this, SLOT (link_context (const QString &)));

		ValueBar *v = new ValueBar (bottom);
		grid2->addWidget (v, i, 2, 1, 2);

		m_labels.append (l);
		m_values.append (v);
	}

	grid->addWidget (bottom, 3, 0, 1, 4);
	grid->setRowStretch (3, 1);

	f = new QFrame (this);
	f->setFrameStyle (QFrame::Plain | QFrame::HLine);
	grid->addWidget (f, 4, 0, 1, 4);

	m_parser = new LastFmParser (this);
	connect (m_parser, SIGNAL (gotData (const QString &)),
			 this, SLOT (update_artists (const QString &)));
	connect (m_parser, SIGNAL (gotError (const QString &)),
			 this, SLOT (update_error (const QString &)));

	connect (m_client->cache (), SIGNAL (entryChanged (uint32_t)),
			 this, SLOT (entry_update (uint32_t)));

	QWidget *dummy = new QWidget (this);
	//dummy->setMargin (1);
	QHBoxLayout *h = new QHBoxLayout (dummy);
	dummy->setLayout (h);

	m_pb = new QProgressBar (dummy);
	m_pb->setMaximum (1);
	m_pb->setMinimum (0);

	m_pl = new QLabel ("", dummy);

	h->addWidget (m_pb);
	h->addWidget (m_pl);

	PlayerButton *settings = new PlayerButton (dummy, ":images/settings.png");
	connect (settings, SIGNAL (clicked (QMouseEvent *)),
			 this, SLOT (settings_pressed (QMouseEvent *)));

	h->addWidget (settings);

	PlayerButton *close = new PlayerButton (dummy, ":images/stop.png");
	connect (close, SIGNAL (clicked (QMouseEvent *)),
			 this, SLOT (close ()));
			
	connect (parent, SIGNAL (selectedID (uint32_t)), this, SLOT (new_id (uint32_t)));

	h->addWidget (close);

	grid->addWidget (dummy, 5, 0, 1, 4);

	resize (300, 400);
	m_current_type = 0;
}

void
LastFmDialog::settings_pressed (QMouseEvent *ev)
{
	QAction *a;
	QMenu m;
	a = m.addAction (tr ("Artists that are similar to ..."),
					 this, SLOT (set_artists ()));
	a->setCheckable (true);
	if (m_current_type == 0)
		a->setChecked (true);
	a = m.addAction (tr ("Most played tracks by ..."),
					 this, SLOT (set_top_tracks ()));
	a->setCheckable (true);
	if (m_current_type == 1)
		a->setChecked (true);
	a = m.addAction (tr ("Most played albums by ..."),
					 this, SLOT (set_top_albums ()));
	a->setCheckable (true);
	if (m_current_type == 2)
		a->setChecked (true);
	m.exec (ev->globalPos ());
}

void
LastFmDialog::set_artists ()
{
}

void
LastFmDialog::set_top_tracks ()
{
}

void
LastFmDialog::set_top_albums ()
{
}

void
LastFmDialog::link_context (const QString &l)
{
	LastFmArtist a;
	if (l == QString (LASTFM_SEARCH_URL).arg (m_current_artist)) {
		a.setNextAttribName("name");
		a.setNextAttrib(m_current_artist);
		
		a.setNextAttribName("url");
		a.setNextAttrib(l);		
	} else {
		a = m_artists[l];
	}
	
	QSettings s;

	QAction *ac;

	QMenu m (this);

	m.addSeparator ();
	ac = m.addAction (tr ("Open artist page at Last.fm"));
	ac->setData (3);
	if (a.contains ("mbid")) {
		ac = m.addAction (tr ("Open artist page at Musicbrainz"));
		ac->setData (4);
	}
	if (s.value ("lastfm/showoink").toBool ()) {
		ac = m.addAction (tr ("Search for artist on OiNK"));
		ac->setData (5);
	}

	ac = m.exec (QCursor::pos ());

	if (!ac)
		return;

	if (ac->data ().toInt () == 2) {
//		pw->add_random (l);
	} else if (ac->data ().toInt () == 3) {
		QDesktopServices::openUrl (QUrl (a.prop ("url")));
	} else if (ac->data ().toInt () == 4) {
		QDesktopServices::openUrl (QUrl (QString (MUSICBRAINZ_URL).arg (a.prop ("mbid"))));
	} else if (ac->data ().toInt () == 5) {
		QDesktopServices::openUrl (QUrl (QString (OINK_SEARCH_URL).arg (a.name ())));
	}


}

void
LastFmDialog::entry_update (uint32_t id)
{
	PlayerWidget *pw = dynamic_cast<PlayerWidget *> (parent ());
	if (pw->currentID () == id) {
		new_id (id);
	}
}

void
LastFmDialog::update_error (const QString &artist)
{
	m_pb->setMaximum (1);
	m_pl->setText (tr ("Error - try again"));
}

void
LastFmDialog::update_artists (const QString &artist)
{
	QList<LastFmArtist> l = m_parser->similar_artist (artist);

	m_artists.clear ();

	for (int i = 0; i < 10; i ++) {
		if (i >= l.count ()) {
			m_labels[i]->setText ("");
			m_values[i]->setValue (0);
		} else {
			LastFmArtist a = l.at (i);
			m_labels[i]->setText (QString ("<a href='%1'>%2</a>").arg (a.prop ("url"), a.name ()));
			m_values[i]->setValue ((uint32_t)a.match ());
			m_artists[a.prop ("url")] = a;

			/* When collections support it we want to use some aggregation here instead */
            Xmms::Coll::Universe univ;
            Xmms::Coll::Match m (univ, "artist", XClient::qToStd (a.name ()));
            m_client->collection ()->queryIds (m) (boost::bind (&LastFmDialog::num_reply, this, _1, a.name ()));
		}
	}

	m_pb->setMaximum (1);
	m_pl->setText ("");
	m_pb->reset ();
}

bool
LastFmDialog::num_reply (Xmms::List<unsigned int> const &list, const QString &artist)
{
    int num = 0;
    /* we have to loop the list inorder to get the lenght :-( */
	for (list.first (); list.isValid (); ++list) {
		num ++;
	}
	
	if (num == 0) {
		return false;
	}

	QString s = QString ("'>%1</a>").arg (artist);

	for (int i = 0; i < m_labels.count (); i ++) {
		if (m_labels.at(i)->text().endsWith(s)) {
			m_labels[i]->setText ("<b>" + m_labels.at(i)->text() + QString(" (%1)</b>").arg(num));			
		}
	}
	return true;
}

void
LastFmDialog::showEvent (QShowEvent *ev)
{
	PlayerWidget *pw = dynamic_cast<PlayerWidget *> (parent ());
	new_id (pw->currentID ());
}

void
LastFmDialog::new_id (uint32_t id)
{
	QHash<QString, QVariant> minfo = m_client->cache ()->get_info (id);

	if (
		!isVisible () ||
		!minfo.contains ("artist") ||
		m_current_artist == minfo["artist"].toString ()
	) {
		return;
	}

	m_parser->abort ();
	m_current_artist = minfo["artist"].toString ();
	QList<LastFmArtist> l = m_parser->similar_artist (minfo["artist"].toString ());
	update_artists (minfo["artist"].toString ());

	if (l.count () == 0) {
		m_pb->setMaximum (0);
		m_pl->setText (tr ("Loading data"));
	}

	m_artistl->setText (QString(QString ("<a href='") + QString(LASTFM_SEARCH_URL) + QString("'>%1</a>")).arg (minfo["artist"].toString ()));
}

