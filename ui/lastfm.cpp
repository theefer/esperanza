#include "lastfm.h"
#include "lastfmparser.h"
#include "valuebar.h"
#include "lastfmartist.h"
#include "medialibdialog.h"
#include "playerwidget.h"

#include <QDialog>
#include <QSettings>
#include <QGridLayout>
#include <QPixmap>
#include <QProgressBar>
#include <QMenu>
#include <QCursor>
#include <QDesktopServices>

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

	m_pb = new QProgressBar (this);
	m_pb->setMaximum (1);
	m_pb->setMinimum (0);

	m_pl = new QLabel ("", this);

	grid->addWidget (m_pb, 5, 0, 1, 2);
	grid->addWidget (m_pl, 5, 2, 1, 2);

	resize (300, 400);
}

void
LastFmDialog::link_context (const QString &l)
{
	LastFmArtist a = m_artists[l];
	QSettings s;

	int num = s.value("core/numrandomsongs").toInt ();

	QAction *ac;

	QMenu m (this);
	ac = m.addAction (tr ("Search for in Medialib"));
	ac->setData (1);
/*	ac = m.addAction (tr ("Add %1 random songs by artist").arg (num));
	ac->setData (2);*/
	m.addSeparator ();
	ac = m.addAction (tr ("Open artist at Last.fm"));
	ac->setData (3);
	if (a.contains ("mbid")) {
		ac = m.addAction (tr ("Open artist at Musicbrainz"));
		ac->setData (4);
	}

	ac = m.exec (QCursor::pos ());

	if (!ac)
		return;

	PlayerWidget *pw = dynamic_cast<PlayerWidget*>(parent ());

	if (ac->data ().toInt () == 1) {
		MedialibDialog *mb = new MedialibDialog (pw, m_client);
		mb->search (MedialibSearchModel::SEARCH_ARTIST, l);
		mb->show ();
	} else if (ac->data ().toInt () == 2) {
//		pw->add_random (l);
	} else if (ac->data ().toInt () == 3) {
		QDesktopServices::openUrl (QUrl (a.prop ("url")));
	} else if (ac->data ().toInt () == 4) {
		QDesktopServices::openUrl (QUrl (QString ("http://musicbrainz.org/artist/%1.html").arg (a.prop ("mbid"))));
	}

}

void
LastFmDialog::entry_update (uint32_t id)
{
	if (m_current == id) {
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
			m_labels[i]->setText (QString ("<a href='%1'>%2</a>").arg (a.name ()).arg (a.name ()));
			m_values[i]->setValue (a.match ());
			m_artists[a.name ()] = a;
		}
	}

	m_pb->setMaximum (1);
	m_pl->setText ("");
	m_pb->reset ();
}

void
LastFmDialog::new_id (uint32_t id)
{
	QHash<QString, QVariant> minfo = m_client->cache ()->get_info (id);

	m_current = id;

	if (!minfo.contains ("artist"))
		return;

	QList<LastFmArtist> l = m_parser->similar_artist (minfo["artist"].toString ());
	update_artists (minfo["artist"].toString ());

	if (l.count () == 0) {
		m_pb->setMaximum (0);
		m_pl->setText (tr ("Loading data"));
	}

	m_artistl->setText (minfo["artist"].toString ());
}

