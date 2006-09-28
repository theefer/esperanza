#include <xmmsclient/xmmsclient++.h>

#include <QWidget>
#include <QGridLayout>
#include <QPixmap>
#include <QLabel>
#include <QPalette>
#include <QFont>
#include <QHBoxLayout>
#include <QMenu>
#include <QProgressBar>
#include <QProgressDialog>

#include "playerwidget.h"
#include "playerbutton.h"
#include "playlistview.h"
#include "progressframe.h"
#include "filedialog.h"
#include "browsedialog.h"
#include "medialibdialog.h"
#include "preferences.h"
#include "volumebar.h"

PlayerWidget::PlayerWidget (QWidget *parent, XClient *client) : QWidget (parent)
{
	QSettings s;

	m_client = client;
	m_unindexed = NULL;

	setFocusPolicy (Qt::StrongFocus);

	m_volbar = new VolumeBar (NULL, client);

	QWidget *dummy = new QWidget (this);

	m_playlist = new PlaylistView (this, client);
	QGridLayout *layout = new QGridLayout (this);

	m_pf = new ProgressFrame (this);

	layout->addWidget (m_pf, 0, 0, 1, 3);
	layout->addWidget (m_playlist, 1, 0, 1, 3);

	dummy = new QWidget (this);
	QHBoxLayout *hbox = new QHBoxLayout (dummy);

	PlayerButton *plus = new PlayerButton (dummy, ":images/plus.png");
	connect (plus, SIGNAL (clicked (QMouseEvent *)),
			 this, SLOT (plus_pressed (QMouseEvent *)));

	PlayerButton *minus = new PlayerButton (dummy, ":images/minus.png");
	connect (minus, SIGNAL (clicked (QMouseEvent *)),
			 this, SLOT (minus_pressed (QMouseEvent *)));

	m_playbutt = new PlayerButton (dummy, ":images/play.png");
	connect (m_playbutt, SIGNAL (clicked (QMouseEvent *)),
			 this, SLOT (play_pressed (QMouseEvent *)));

	PlayerButton *back = new PlayerButton (dummy, ":images/back.png");
	connect (back, SIGNAL (clicked (QMouseEvent *)),
			 this, SLOT (back_pressed (QMouseEvent *)));

	PlayerButton *fwd = new PlayerButton (dummy, ":images/forward.png");
	connect (fwd, SIGNAL (clicked (QMouseEvent *)),
			 this, SLOT (fwd_pressed (QMouseEvent *)));

	PlayerButton *sett = new PlayerButton (dummy, ":images/settings.png");
	connect (sett, SIGNAL (clicked (QMouseEvent *)),
			 this, SLOT (snett_pressed (QMouseEvent *)));

	PlayerButton *volume = new PlayerButton (dummy, ":images/volume.png");
	connect (volume, SIGNAL (clicked (QMouseEvent *)),
			 this, SLOT (volume_pressed (QMouseEvent *)));

	m_playstop = new PlayerButton (dummy, ":images/playstop.png");
	connect (m_playstop, SIGNAL (clicked (QMouseEvent *)),
			 this, SLOT (playstop_pressed (QMouseEvent *)));

	hbox->addWidget (back);
	hbox->addWidget (m_playbutt);

	hbox->addWidget (m_playstop);
	if (!s.value ("ui/showstop", false).toBool ())
		m_playstop->hide ();

	hbox->addWidget (fwd);

	hbox->addStretch (1);

	hbox->addWidget (volume);
	hbox->addWidget (plus);
	hbox->addWidget (minus);
	hbox->addWidget (sett);

	layout->addWidget (dummy, 2, 0, 1, 3);

	layout->setRowStretch (1, 1);
	layout->setRowStretch (0, 0);
	layout->setColumnStretch (1, 1);
	layout->setColumnStretch (2, 1);
	layout->setColumnStretch (0, 0);
	layout->setMargin (1);

	m_current_id = 0;
	connect (client, SIGNAL (gotConnection (XClient *)),
			 this, SLOT (got_connection (XClient *))); 


	resize (s.value ("player/windowsize", QSize (550, 350)).toSize());
	if (s.contains ("player/position"))
		move (s.value ("player/position").toPoint ());

	connect (m_client->cache (), SIGNAL (entryChanged (uint32_t)),
			 this, SLOT (entry_changed (uint32_t)));

	connect (m_client->settings (), SIGNAL (settingsChanged ()),
			 this, SLOT (changed_settings ()));

	/* run it once first time */
	changed_settings ();
}

void
PlayerWidget::changed_settings ()
{
	QSettings s;

	/* XXX: remove this ugly duplication! */
	QFont f = QApplication::font ();
	f.setPixelSize (s.value ("ui/fontsize", 10).toInt ());
	QApplication::setFont (f);

	/* base palette */
	QPalette p (QApplication::palette ());
	p.setColor (QPalette::Highlight,
				s.value ("ui/highlight", QColor (80, 80, 80)).value<QColor> ());
	p.setColor (QPalette::HighlightedText,
				s.value ("ui/highlightedtext", QColor (Qt::black)).value<QColor> ());
	p.setColor (QPalette::Inactive, QPalette::Text, QColor (Qt::black));
	QApplication::setPalette (p);

	if (!s.value ("ui/showstop", false).toBool ())
		m_playstop->hide ();
	else
		m_playstop->show ();

	if (s.value ("ui/reverseplaytime", true).toBool ())
		m_pf->setReverse (true);

	update ();
}

void
PlayerWidget::moveEvent (QMoveEvent *ev)
{
	QSettings s;
	s.setValue ("player/position", pos ());
}

void
PlayerWidget::resizeEvent (QResizeEvent *ev)
{
	QSettings s;
	s.setValue ("player/windowsize", ev->size ());
}

void
PlayerWidget::volume_pressed (QMouseEvent *ev)
{
	m_volbar->move (ev->globalPos ());
	m_volbar->show ();
}

void
PlayerWidget::keyPressEvent (QKeyEvent *ev)
{
	switch (ev->key ()) {
		case Qt::Key_Backspace:
		case Qt::Key_Delete:
			remove_selected ();
			break;
		case Qt::Key_A:
			add_local_file ();
			break;
		case Qt::Key_R:
			add_remote_file ();
			break;
		case Qt::Key_D:
			add_local_dir ();
			break;
		case Qt::Key_C:
			remove_all ();
			break;
		case Qt::Key_M:
			show_medialib ();
			break;
	}
}

void
PlayerWidget::play_pressed (QMouseEvent *ev)
{
	if (m_status == Xmms::Playback::PLAYING)
		m_client->playback.pause (&XClient::log);
	else
		m_client->playback.start (&XClient::log);
}

static bool
dummy_uint (const uint32_t &)
{
	return false;
}

void
PlayerWidget::fwd_pressed (QMouseEvent *ev)
{
	m_client->playlist.setNextRel (1, &dummy_uint);
	m_client->playback.tickle (&XClient::log);
}

void
PlayerWidget::back_pressed (QMouseEvent *ev)
{
	m_client->playlist.setNextRel (-1, &dummy_uint);
	m_client->playback.tickle (&XClient::log);
}

void
PlayerWidget::show_medialib ()
{
	MedialibDialog *mb = new MedialibDialog (this, m_client);
	mb->show ();
}

void
PlayerWidget::plus_pressed (QMouseEvent *ev)
{
	QMenu m;

	m.addAction (tr ("Add local file"), this, SLOT (add_local_file ()));
	m.addAction (tr ("Add local dir"), this, SLOT (add_local_dir ()));
	m.addSeparator ();
	m.addAction (tr ("Add remote file"), this, SLOT (add_remote_file ()));
	m.addSeparator ();
	m.addAction (tr ("Add files from medialib"), this, SLOT (show_medialib ()));

	m.exec (ev->globalPos ());
}

void
PlayerWidget::snett_pressed (QMouseEvent *ev)
{
	QMenu m;
	m.addAction (tr ("Preferences"), this, SLOT (open_pref ()));
	QMenu *pm = m.addMenu (tr ("Playlist Options"));
	pm->addAction (tr ("Shuffle"));
	pm->addAction (tr ("Random"))->setCheckable (true);
	pm->addAction (tr ("Stop after play"))->setCheckable (true);

	m.exec (ev->globalPos ());
}

void
PlayerWidget::open_pref ()
{
	PreferenceDialog *pd = new PreferenceDialog (this, m_client);
	pd->show ();
}

void
PlayerWidget::add_remote_file ()
{
	BrowseDialog bd (window (), m_client);
	QStringList files = bd.getFiles ();

	for (int i = 0; i < files.count(); i++) {
		m_client->playlist.addUrlEncoded (files.value (i).toStdString (), &XClient::log);
	}

}

void
PlayerWidget::add_local_dir ()
{
	FileDialog fd (this, "playlist_add_dir");
	QString dir = fd.getDirectory ();
	if (!dir.isNull ())
		m_client->playlist.addRecursive (("file://" + dir).toStdString (), &XClient::log);
}

void
PlayerWidget::add_local_file ()
{
	QStringList files;
	FileDialog fd (this, "playlist_add_file");

	files = fd.getFiles ();

	for (int i = 0; i < files.count(); i++) {
		QString s = "file://" + files.at (i);
		m_client->playlist.addUrl (s.toStdString (), &XClient::log);
	}

}

void
PlayerWidget::minus_pressed (QMouseEvent *ev)
{
	QMenu m;

	m.addAction (tr ("Remove selected files"), this, SLOT (remove_selected ()));
	m.addAction (tr ("Clear playlist"), this, SLOT (remove_all ()));

	m.exec (ev->globalPos ());
}

void
PlayerWidget::remove_selected ()
{
	QList<uint32_t> itm = m_playlist->getSelection ();
	qSort (itm);
	for (int i = itm.size () - 1; i > -1; i --) {
		m_client->playlist.remove (itm.at (i), &XClient::log);
	}
}

void
PlayerWidget::playstop_pressed (QMouseEvent *ev)
{
	m_client->playback.stop (&XClient::log);
}

void
PlayerWidget::remove_all ()
{
	m_client->playlist.clear (&XClient::log);
}

void
PlayerWidget::got_connection (XClient *client)
{
	m_client = client;

	client->playback.broadcastCurrentID (Xmms::bind (&PlayerWidget::handle_current_id, this));
	client->playback.currentID (Xmms::bind (&PlayerWidget::handle_current_id, this));

	client->playback.signalPlaytime (Xmms::bind (&PlayerWidget::handle_playtime, this));
	client->playback.getPlaytime (Xmms::bind (&PlayerWidget::handle_playtime, this));

	client->playback.getStatus (Xmms::bind (&PlayerWidget::handle_status, this));
	client->playback.broadcastStatus (Xmms::bind (&PlayerWidget::handle_status, this));


	/* XXX: broken in c++ bindings
	client->stats.broadcastMediainfoReaderStatus (Xmms::bind (&PlayerWidget::handle_index_status, this));
	client->stats.signalMediainfoReaderUnindexed (Xmms::bind (&PlayerWidget::handle_unindexed, this));
	*/
}

bool
PlayerWidget::handle_index_status (const Xmms::Stats::ReaderStatus &rs)
{
	if (rs == Xmms::Stats::IDLE) {
		if (!m_unindexed) {
			m_unindexed = new QProgressDialog (tr ("Indexing files"), tr ("Hide window"), 0, 1, this);
			QProgressBar *b = new QProgressBar (m_unindexed);
			b->setInvertedAppearance (true);
			b->setRange (0, 1);
			m_unindexed->setBar (b);
		}
	} else if (rs == Xmms::Stats::RUNNING) {
		if (m_unindexed) {
			m_unindexed->cancel ();
			delete m_unindexed;
			m_unindexed = NULL;
		}
	}

	return true;
}

bool
PlayerWidget::handle_unindexed (const uint32_t &i)
{
	if (m_unindexed) {
		if (m_unindexed->maximum () == 1) {
			m_unindexed->setRange (0, i);
		}
		m_unindexed->setValue (m_unindexed->maximum()-i);
	}
	return true;
}

bool
PlayerWidget::handle_status (const Xmms::Playback::Status &st)
{
	m_status = st;

	if (st == Xmms::Playback::STOPPED ||
		st == Xmms::Playback::PAUSED)
		m_playbutt->setPx (":images/play.png");
	else
		m_playbutt->setPx (":images/pause.png");

	return true;
}

void
PlayerWidget::entry_changed (uint32_t id)
{
	if (id == m_current_id) {
		new_info (m_client->cache ()->get_info (id));
	}
}

void
PlayerWidget::new_info (const QHash<QString,QVariant> &h)
{
	QString s = QString ("%1. %2 - %3")
		.arg(h["id"].toUInt ())
		.arg(h["artist"].toString ())
		.arg(h["title"].toString ());
	m_pf->setText (s);
	if (h.contains ("duration")) {
		uint32_t dur = h["duration"].toUInt ();
		m_pf->setMaximum (dur / 1000);
		m_pf->setValue (0);
	}
}

bool
PlayerWidget::handle_playtime (const unsigned int &tme)
{
	m_pf->setValue (tme / 1000);
	return true;
}

bool
PlayerWidget::handle_current_id (const unsigned int &id)
{
	m_current_id = id;
	new_info (m_client->cache ()->get_info (id));
	return true;
}
