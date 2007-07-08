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


#include "xclient.h"
#include "minimode.h"
#include "playerbutton.h"
#include "progressframe.h"
#include "playerwidget.h"
#include "volumebar.h"
#include "shortcutmanager.h"

#include <QHideEvent>
#include <QWidget>
#include <QGridLayout>
#include <QMoveEvent>
#include <QKeyEvent>
#include <QVariant>

MiniMode::MiniMode (QWidget *parent, XClient *client) : QFrame (NULL)
{
	setWindowTitle (tr ("Esperanza"));
	setAttribute (Qt::WA_QuitOnClose, false);
	m_client = client;
	m_parent = parent;

	QSettings s;

	setFrameStyle (QFrame::Plain | QFrame::StyledPanel);
	setLineWidth (2);

	QGridLayout *g = new QGridLayout (this);
	setLayout (g);
	m_progress = new ProgressFrame (this, client, false);
	m_progress->setMove (true);
	
	m_playbutt = new PlayerButton (this, ":images/play.png");
	connect (m_playbutt, SIGNAL (clicked (QMouseEvent *)),
			 parent, SLOT (play_pressed ()));

	PlayerButton *back = new PlayerButton (this, ":images/back.png");
	connect (back, SIGNAL (clicked (QMouseEvent *)),
			 parent, SLOT (back_pressed ()));

	PlayerButton *fwd = new PlayerButton (this, ":images/forward.png");
	connect (fwd, SIGNAL (clicked (QMouseEvent *)),
			 parent, SLOT (fwd_pressed ()));

	PlayerButton *sett = new PlayerButton (this, ":images/settings.png");
	connect (sett, SIGNAL (clicked (QMouseEvent *)),
			 parent, SLOT (snett_pressed (QMouseEvent *)));

	m_stop = new PlayerButton (this, ":images/playstop.png");
	connect (m_stop, SIGNAL (clicked (QMouseEvent *)),
			 parent, SLOT (playstop_pressed ()));

	PlayerButton *minmax = new PlayerButton (this, ":images/minmax.png");
	connect (minmax, SIGNAL (clicked (QMouseEvent *)), this, SLOT (min_pressed ()));

	g->addWidget (back, 0, 0);
	g->addWidget (m_playbutt, 0, 1);
	g->addWidget (m_stop, 0, 2);

	if (!s.value ("ui/showstop").toBool ())
		m_stop->hide ();

	
	g->addWidget (fwd, 0, 3);

	m_volume = new VolumeButton (this, m_client);
	g->addWidget (m_volume, 0, 4);
	if (!s.value ("ui/volumepopup").toBool ())
		m_volume->hide ();

	g->addWidget (sett, 0, 5);
	g->addWidget (m_progress, 0, 6);
	g->addWidget (minmax, 0, 7);

	g->setMargin (1);

	connect (m_client->settings (), SIGNAL (settingsChanged ()),
			 this, SLOT (changed_settings ()));

	move (s.value ("minimode/pos", parent->pos ()).toPoint ());

	ShortcutManager *sm = ShortcutManager::instance ();

	sm->connect (this, "shortcuts/shuffle", "S", SLOT (shuffle_pressed ()));
	sm->connect (this, "shortcuts/addfile", "A", SLOT (add_local_file ()));
	sm->connect (this, "shortcuts/adddir", "D", SLOT (add_local_dir ()));
	sm->connect (this, "shortcuts/play", "Space", SLOT (play_pressed ()));
	sm->connect (this, "shortcuts/forward", "B", SLOT (fwd_pressed ()));
	sm->connect (this, "shortcuts/back", "V", SLOT (back_pressed ()));
	sm->connect (this, "shortcuts/hide", "Esc", SLOT (check_hide ()));
	sm->connect (this, "shortcuts/minmax", "Ctrl+M", SLOT (min_pressed ()));
}

void 
MiniMode::play_pressed () {
 	PlayerWidget *pw = dynamic_cast<PlayerWidget *> (m_parent);
	pw->play_pressed ();
}

void 
MiniMode::fwd_pressed () {
 	PlayerWidget *pw = dynamic_cast<PlayerWidget *> (m_parent);
	pw->fwd_pressed ();
}

void 
MiniMode::back_pressed () {
 	PlayerWidget *pw = dynamic_cast<PlayerWidget *> (m_parent);
	pw->back_pressed ();
}

void 
MiniMode::shuffle_pressed () {
 	PlayerWidget *pw = dynamic_cast<PlayerWidget *> (m_parent);
	pw->shuffle_pressed ();
}

void 
MiniMode::add_local_file () {
 	PlayerWidget *pw = dynamic_cast<PlayerWidget *> (m_parent);
	pw->add_local_file ();
}

void 
MiniMode::add_local_dir () {
 	PlayerWidget *pw = dynamic_cast<PlayerWidget *> (m_parent);
	pw->add_local_dir ();
}

void 
MiniMode::check_hide () {
 	PlayerWidget *pw = dynamic_cast<PlayerWidget *> (m_parent);
	pw->toggle_mini ();
}

void
MiniMode::showEvent (QShowEvent *sh)
{
	resize (m_parent->size ().width (), 22);
}

void
MiniMode::changed_settings ()
{
	QSettings s;
	if (!s.value ("ui/showstop").toBool ())
		m_stop->hide ();
	else
		m_stop->show ();

	if (!s.value ("ui/volumepopup").toBool ())
		m_volume->hide ();
	else
		m_volume->show ();

	setWindowFlags ();
}

void
MiniMode::moveEvent (QMoveEvent *ev)
{
	QSettings s;
	s.setValue ("minimode/pos", ev->pos ());
}

void
MiniMode::min_pressed ()
{
	QSettings s;
	s.setValue ("ui/minimode", false);
	m_parent->show ();
	m_parent->activateWindow ();
	hide ();
}

void
MiniMode::setWindowFlags ()
{
	QSettings s;
	Qt::WindowFlags f;
	f = Qt::FramelessWindowHint;

	if(s.value ("ui/toolwindow").toBool () && s.value ("core/systray").toBool ())
		f |= Qt::Tool;

	if(s.value("ui/alwaysontop").toBool ())
		 f |= Qt::WindowStaysOnTopHint;

	QFrame::setWindowFlags (f);
	
	if(s.value("ui/minimode").toBool ())
	{
		show();
		activateWindow ();
	}
}
