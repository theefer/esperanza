/**
 *  This file is a part of Esperanza, an XMMS2 Client.
 *
 *  Copyright (C) 2005-2006 XMMS2 Team
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


#include <xmmsclient/xmmsclient++.h>

#include <QMainWindow>
#include <QMenu>
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
#include <QErrorMessage>
#include <QTimer>
#include <QIcon>

#include "playerwidget.h"
#include "playerbutton.h"
#include "fancyplaylistview.h"
#include "progressframe.h"
#include "filedialog.h"
#include "preferences.h"
#include "volumebar.h"
#include "textdialog.h"
#include "systemtray.h"
#include "infowindow.h"
#include "minimode.h"
#include "jumptofiledialog.h"
#include "lastfm.h"
#include "medialibdialog.h"
#include "streamingdialog.h"
#include "shortcutmanager.h"
/*#include "collections/manager.h"*/


PlayerWidget::PlayerWidget (QWidget *parent, XClient *client) : QMainWindow (parent)
{
	QSettings s;

	m_client = client;

	setWindowTitle ("Esperanza");
	setFocusPolicy (Qt::StrongFocus);
	setAttribute (Qt::WA_DeleteOnClose);

	set_colors ();

	QWidget *main_w = new QWidget (this);
	setCentralWidget (main_w);

	QWidget *dummy = new QWidget (main_w);

	QGridLayout *layout = new QGridLayout (main_w);
	m_playlist = new FancyPlaylistView (this, client);
	layout->addWidget (m_playlist, 1, 0, 1, 3);

	QHBoxLayout *pflay = new QHBoxLayout ();
	pflay->setMargin (0);

	m_pf = new ProgressFrame (this, client);
	pflay->addWidget (m_pf);
	pflay->setStretchFactor (m_pf, 1);

	PlayerButton *min = new PlayerButton (dummy, ":images/minmax.png");
	connect (min, SIGNAL (clicked (QMouseEvent *)),
			 this, SLOT (min_pressed ()));
	pflay->addWidget (min);

	layout->addLayout (pflay, 0, 0, 1, 3);

	dummy = new QWidget (main_w);
	QHBoxLayout *hbox = new QHBoxLayout (dummy);

	PlayerButton *plus = new PlayerButton (dummy, ":images/plus.png");
	connect (plus, SIGNAL (clicked (QMouseEvent *)),
			 this, SLOT (plus_pressed (QMouseEvent *)));

	PlayerButton *minus = new PlayerButton (dummy, ":images/minus.png");
	connect (minus, SIGNAL (clicked (QMouseEvent *)),
			 this, SLOT (minus_pressed (QMouseEvent *)));

	m_playbutt = new PlayerButton (dummy, ":images/play.png");
	connect (m_playbutt, SIGNAL (clicked (QMouseEvent *)),
			 this, SLOT (play_pressed ()));

	PlayerButton *back = new PlayerButton (dummy, ":images/back.png");
	connect (back, SIGNAL (clicked (QMouseEvent *)),
			 this, SLOT (back_pressed ()));

	PlayerButton *fwd = new PlayerButton (dummy, ":images/forward.png");
	connect (fwd, SIGNAL (clicked (QMouseEvent *)),
			 this, SLOT (fwd_pressed ()));

	PlayerButton *sett = new PlayerButton (dummy, ":images/settings.png");
	connect (sett, SIGNAL (clicked (QMouseEvent *)),
			 this, SLOT (snett_pressed (QMouseEvent *)));

	PlayerButton *info = new PlayerButton (dummy, ":images/info.png");
	connect (info, SIGNAL (clicked (QMouseEvent *)),
			 this, SLOT (info_pressed (QMouseEvent *)));

	VolumeButton *volume = new VolumeButton (dummy, m_client);

	m_playstop = new PlayerButton (dummy, ":images/playstop.png");
	connect (m_playstop, SIGNAL (clicked (QMouseEvent *)),
			 this, SLOT (playstop_pressed ()));

	hbox->addWidget (back);
	hbox->addWidget (m_playbutt);

	hbox->addWidget (m_playstop);
	if (!s.value ("ui/showstop").toBool ())
		m_playstop->hide ();

	hbox->addWidget (fwd);

	hbox->addStretch (1);

	hbox->addWidget (volume);
	hbox->addWidget (plus);
	hbox->addWidget (minus);
	hbox->addWidget (info);
	hbox->addWidget (sett);

	layout->addWidget (dummy, 2, 0, 1, 3);

	layout->setRowStretch (1, 1);
	layout->setRowStretch (0, 0);
	layout->setColumnStretch (1, 1);
	layout->setColumnStretch (2, 1);
	layout->setColumnStretch (0, 0);
	layout->setMargin (1);

	m_status = Xmms::Playback::STOPPED;

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

	/* mac specific code to show menus */
#ifdef Q_WS_MACX
	QMenu *m = menuBar ()->addMenu (tr ("&File"));
	m->addAction (tr ("Preferences"), this, SLOT (open_pref ()));
	m->addAction (tr ("About"), this, SLOT (open_about ()));
	m = menuBar ()->addMenu (tr ("Playlist"));
	m->addAction (tr ("Add local file"), this, SLOT (add_local_file ()));
	m->addAction (tr ("Add local dir"), this, SLOT (add_local_dir ()));
	m->addSeparator ();
	/*
	m->addAction (tr ("Add remote file"), this, SLOT (add_remote_file ()));
	*/
	m = menuBar ()->addMenu (tr ("Help"));
	m->addAction (tr ("Esperanza Help"), this, SLOT (open_short_help ()));
#endif

	// System Tray setup
	if (QSystemTrayIcon::isSystemTrayAvailable ()) {
		m_systray = new SystemTray (this, m_client);
	} else {
		m_systray = NULL;
	}

	/* create the info window */
	/*
	m_info = new InfoWindow (this, m_client);
	m_info->hide ();
	connect (m_playlist, SIGNAL (selectedID (uint32_t)), m_info, SLOT (set_current_id (uint32_t)));
	*/

	/* last.fm */
	m_lastfm = new LastFmDialog (this, m_client);
	m_lastfm->hide ();
	connect (m_playlist, SIGNAL (selectedID (uint32_t)), m_lastfm, SLOT (new_id (uint32_t)));

	m_mini = new MiniMode (this, m_client);

    m_streaming = NULL;

	ShortcutManager *sm = ShortcutManager::instance ();

	sm->connect (this, "shortcuts/remove", "Del", SLOT (remove_selected ()));
	sm->connect (this, "shortcuts/openmedialib", "M", SLOT (mlib_pressed ()));
	sm->connect (this, "shortcuts/shuffle", "S", SLOT (shuffle_pressed ()));
	sm->connect (this, "shortcuts/addfile", "A", SLOT (add_local_file ()));
	sm->connect (this, "shortcuts/adddir", "D", SLOT (add_local_dir ()));
	sm->connect (this, "shortcuts/removeall", "C", SLOT (remove_all ()));
	sm->connect (this, "shortcuts/play", "Space", SLOT (play_pressed ()));
	sm->connect (this, "globalshortcuts/play", "Ctrl+Shift+C", SLOT (play_pressed ()), true);
	sm->connect (this, "shortcuts/forward", "B", SLOT (fwd_pressed ()));
	sm->connect (this, "globalshortcuts/forward", "Ctrl+Shift+V", SLOT (fwd_pressed ()), true);
	sm->connect (this, "shortcuts/back", "V", SLOT (back_pressed ()));
	sm->connect (this, "globalshortcuts/back", "Ctrl+Shift+X", SLOT (back_pressed ()), true);
	sm->connect (this, "shortcuts/openpref", "P", SLOT (open_pref ()));
	sm->connect (this, "shortcuts/jump", "J", SLOT (jump_pressed ()));
	sm->connect (this, "shortcuts/lastfm", "L", SLOT (lastfm_pressed ()));
	sm->connect (this, "shortcuts/hide", "Esc", SLOT (check_hide ()));
	sm->connect (this, "shortcuts/jumppos", "Return", SLOT (jump_pos ()));
	sm->connect (this, "shortcuts/minmax", "Ctrl+M", SLOT (min_pressed ()));
	sm->connect (this, "globalshortcuts/stop", "Ctrl+Shift+B", SLOT (playstop_pressed ()), true);
	sm->connect (this, "globalshortcuts/show_hide", "Ctrl+Shift+M", SLOT (toggle_hide ()), true);
}

void
PlayerWidget::toggle_mini () const
{
	QSettings s;

	if (m_mini->isVisible () &&
			(
				!s.value("ui/alwaysontop").toBool () && m_mini->isActiveWindow () ||
				s.value("ui/alwaysontop").toBool ()
			)
		) {
		m_mini->hide ();
	} else {
		m_mini->hide ();
		m_mini->show ();
	}
}

void
PlayerWidget::closeEvent (QCloseEvent *ev)
{
	QSettings s;
	if( !s.value("ui/hideOnClose").toBool () || !s.value("core/systray").toBool () )
		qApp->quit ();
	else {
		hide ();
		ev->ignore ();
	}
}

void
PlayerWidget::set_colors ()
{
}

void
PlayerWidget::min_pressed ()
{
	QSettings s;
	s.setValue ("ui/minimode", true);
	m_mini->show ();
	hide ();
}

void
PlayerWidget::changed_settings ()
{
	QSettings s;

	set_colors ();

	if (!s.value ("ui/showstop").toBool ())
		m_playstop->hide ();
	else
		m_playstop->show ();

	if (s.value ("ui/reverseplaytime").toBool ())
		m_pf->setReverse (true);

	if (m_systray) {
		if ( s.value ("core/systray").toBool ())
			m_systray->show();
		else
			m_systray->hide();
	}

	setWindowFlags ();
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
PlayerWidget::check_hide ()
{
	if (isHidden ())
		return;

	QSettings s;

	if (m_systray && s.value ("core/systray").toBool ()) {
		hide ();
	}
}

void
PlayerWidget::jump_pos ()
{
	m_playlist->jump_pos (QModelIndex ());
}

void
PlayerWidget::mlib_pressed ()
{
	MedialibDialog *d = new MedialibDialog (this, m_client);
	d->show ();
}

void
PlayerWidget::lastfm_pressed ()
{
	m_lastfm->show ();
}

void
PlayerWidget::jump_pressed ()
{
	/*
	JumpToFileDialog d (this, m_playlist->model ());
	if (d.exec ()) {
		QModelIndex idx = d.sel_item ();
		if (idx.isValid ()) {
			m_playlist->setCurrentIndex (idx);
		}
	}*/
}

void
PlayerWidget::play_pressed ()
{
	if (m_status == Xmms::Playback::PLAYING)
		m_client->playback.pause () ();
	else
		m_client->playback.start () ();
}

void
PlayerWidget::fwd_pressed ()
{
	m_client->playlist.setNextRel (1) ();
	m_client->playback.tickle () ();
}

void
PlayerWidget::back_pressed ()
{
	m_client->playlist.setNextRel (-1) ();
	m_client->playback.tickle () ();
}

void
PlayerWidget::plus_pressed (QMouseEvent *ev)
{
	QMenu m;

	m.addAction (tr ("Add local file"), this, SLOT (add_local_file ()));
	m.addAction (tr ("Add local dir"), this, SLOT (add_local_dir ()));
	m.addAction (tr ("Search medialib"), this, SLOT (mlib_pressed ()));
	m.addSeparator ();
	/*
	m.addAction (tr ("Add remote file"), this, SLOT (add_remote_file ()));
	*/
	m.addAction (tr ("Add URL"), this, SLOT (add_url ()));

	m.exec (ev->globalPos ());
}

void
PlayerWidget::info_pressed (QMouseEvent *ev)
{
	QMenu m;
	m.addAction (tr ("Last.fm browser"), this, SLOT (lastfm_pressed ()));
	m.addAction (tr ("Stream directory"), this, SLOT (streaming_pressed ()));
/*	m.addAction (tr ("Collection Manager"), this, SLOT (coll_pressed ()));*/
	m.addSeparator ();
	m.addAction (tr ("Keyboard shortcuts"), this, SLOT (open_short_help ()));
	m.addAction (tr ("About Esperanza"), this, SLOT (open_about ()));

	m.exec (ev->globalPos ());
}

void
PlayerWidget::streaming_pressed ()
{
    if (m_streaming) {
        if (!m_streaming->isVisible ()) {
            m_streaming->show ();
        } else {
            m_streaming->hide ();
        }
    } else {
	    m_streaming = new StreamingDialog (this, m_client);
	    m_streaming->show ();
    }
}

void
PlayerWidget::coll_pressed ()
{
	/*
	CollectionManager *coll = new CollectionManager (this, m_client);
	coll->show ();
	*/
}

void
PlayerWidget::snett_pressed (QMouseEvent *ev)
{
	QMenu m;
	m.addAction (tr ("Preferences"), this, SLOT (open_pref ()));
	m.addSeparator ();
	m.addAction (tr ("Shuffle"), this, SLOT (shuffle_pressed ()));

	m.exec (ev->globalPos ());
}

void
PlayerWidget::shuffle_pressed ()
{
	m_client->playlist.shuffle () ();
}

/*
void
PlayerWidget::open_sceditor ()
{
	ShortCutEditor *sc = new ShortCutEditor (this, m_client);
	sc->show ();
}
*/

void
PlayerWidget::open_about ()
{
	TextDialog dia (this);
	dia.read_file (":text/about.html");
	dia.exec ();
}

void
PlayerWidget::open_short_help ()
{
	TextDialog dia (this);
	dia.read_file (":text/shortcuts.html");
	dia.exec ();
}

void
PlayerWidget::open_pref ()
{
	PreferenceDialog *pd = new PreferenceDialog (this, m_client);
	pd->show ();
}

void
PlayerWidget::add_url ()
{
	bool ok;
	QString url = QInputDialog::getText (this, tr ("URL dialog"), tr ("Enter URL:"),
										 QLineEdit::Normal, "http://", &ok);
	if (ok && !url.isEmpty ()) {
		m_client->playlist.addUrl (XClient::qToStd (url)) ();
	}
}

void
PlayerWidget::add_remote_file ()
{
	FileDialog fd (this, "remote_file", true);
	QStringList files = fd.getFiles ();

	for (int i = 0; i < files.count(); i++) {
		m_client->playlist.addUrlEncoded (files.value (i).toStdString ()) ();
	}

}

void
PlayerWidget::add_local_dir ()
{
	FileDialog fd (this, "playlist_add_dir");
	QString dir = fd.getDirectory ();
	if (!dir.isNull ())
		m_client->playlist.addRecursive (XClient::qToStd ("file://" + dir)) ();
}

void
PlayerWidget::add_local_file ()
{
	QStringList files;
	FileDialog fd (this, "playlist_add_file");

	files = fd.getFiles ();

	for (int i = 0; i < files.count(); i++) {
		QString s = "file://" + files.at (i);
		m_client->playlist.addUrl (XClient::qToStd (s)) ();
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
	QModelIndexList itm = m_playlist->getSelection ();
	QList<uint32_t> idlist;

	for (int i = 0; i < itm.size (); i++) {
		QModelIndex idx = itm.at (i);
		if (idx.column () != 0)
			continue;

		idlist.append (idx.row ());
	}

	qSort (idlist);
	for (int i = idlist.size () - 1; i > -1; i --) {
		m_client->playlist.removeEntry (idlist.at (i)) ();
	}
}

void
PlayerWidget::playstop_pressed ()
{
	m_client->playback.stop () ();
}

void
PlayerWidget::remove_all ()
{
	m_client->playlist.clear () ();
}

void
PlayerWidget::got_connection (XClient *client)
{
	m_client = client;

	client->playback.signalPlaytime () (Xmms::bind (&PlayerWidget::handle_playtime, this));
	client->playback.getPlaytime () (Xmms::bind (&PlayerWidget::handle_playtime, this));

	client->playback.getStatus () (Xmms::bind (&PlayerWidget::handle_status, this));
	client->playback.broadcastStatus () (Xmms::bind (&PlayerWidget::handle_status, this));

	client->setDisconnectCallback (boost::bind (&PlayerWidget::handle_disconnect, this));

	client->playback.broadcastCurrentID () (Xmms::bind (&PlayerWidget::handle_current_id, this));
	client->playback.currentID () (Xmms::bind (&PlayerWidget::handle_current_id, this));

	/* XXX: broken in c++ bindings
	client->stats.broadcastMediainfoReaderStatus (Xmms::bind (&PlayerWidget::handle_index_status, this));
	client->stats.signalMediainfoReaderUnindexed (Xmms::bind (&PlayerWidget::handle_unindexed, this));
	*/
}

void
PlayerWidget::handle_disconnect ()
{
	QErrorMessage *err = new QErrorMessage (this);
	err->showMessage (tr ("Server died. Esperanza will close."));
	err->exec ();
	QApplication::quit ();
}

bool
PlayerWidget::handle_status (const Xmms::Playback::Status &st)
{
	m_status = st;

	if (st == Xmms::Playback::STOPPED ||
		st == Xmms::Playback::PAUSED) {
		m_playbutt->setPx (":images/play.png");
		m_mini->update_playbutton (":images/play.png");
	} else {
		m_playbutt->setPx (":images/pause.png");
		m_mini->update_playbutton (":images/pause.png");
	}

	if (st == Xmms::Playback::STOPPED) {
		m_pf->setValue (0);
	}

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
	QString s;
	if (!h.contains ("title")) {
		s = h["url"].toString ();
	} else {
		s = QString ("%1 - %2")
			.arg(h["artist"].toString ())
			.arg(h["title"].toString ());
	}

	m_pf->setText (s);
	m_mini->setText (s);

	if (h.contains ("duration")) {
		uint32_t dur = h["duration"].toUInt ();
		m_pf->setMaximum (dur / 1000);
		m_pf->setValue (0);

		m_mini->setMaximum (dur / 1000);
		m_mini->setValue (0);
	}
	if (m_systray &&
		m_current_id == h["id"].toUInt () &&
        m_status == Xmms::Playback::PLAYING &&
        h["id"].toUInt () != 0) {
		m_systray->do_notification (tr ("Esperanza is now playing:"), s, m_client->cache ()->get_pixmap (m_current_id));
	}
}

bool
PlayerWidget::handle_playtime (const unsigned int &tme)
{
	m_pf->setValue (tme / 1000);
	m_mini->setValue (tme / 1000);
	return true;
}

bool
PlayerWidget::handle_current_id (const unsigned int &id)
{
	m_current_id = id;
	new_info (m_client->cache ()->get_info (id));
	return true;
}

void
PlayerWidget::setWindowFlags()
{
	QSettings s;
	Qt::WindowFlags f = 0;

	if(s.value ("ui/toolwindow").toBool () && s.value ("core/systray").toBool ())
		f |= Qt::Tool;
	else
		f |= Qt::Window;

	if(s.value("ui/alwaysontop").toBool ())
		f |= Qt::WindowStaysOnTopHint;

	QMainWindow::setWindowFlags (f);

	if(!s.value("ui/minimode").toBool ())
		show();
}

void
PlayerWidget::toggle_hide ()
{
	QSettings s;
	if (s.value ("ui/minimode", false).toBool ()) {
		toggle_mini ();
	} else {
		if (isVisible () &&
			(
				!s.value("ui/alwaysontop").toBool () && isActiveWindow () ||
				s.value("ui/alwaysontop").toBool ()
			)
		) {
			hide ();
		} else {
			hide ();
			show ();
		}
	}
}
