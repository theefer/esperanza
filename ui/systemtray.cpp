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


#include "xclient.h"
#include "systemtray.h"

#include <QSystemTrayIcon>
#include <QSettings>

#include "playerwidget.h"

SystemTray::SystemTray (QObject *parent, XClient *client) : QSystemTrayIcon (parent)
{
	QSettings s;

	m_client = client;

	connect (this, SIGNAL (activated (QSystemTrayIcon::ActivationReason)),
			 this, SLOT (systray_trigger (QSystemTrayIcon::ActivationReason)));
	
	setIcon (QIcon (":images/systray.png"));
	
	PlayerWidget *pw = dynamic_cast<PlayerWidget*> (parent);

	QAction *a;
	QMenu *systray_menu = new QMenu ();

	m_display_action = systray_menu->addAction (tr ("Nothing"));
	m_display_action->setEnabled (false);
	m_hide_action = systray_menu->addAction (tr ("Hide main window"), this, SLOT (toggle_hide ()));
	systray_menu->addSeparator ();

	m_play_action = systray_menu->addAction (tr ("Play/Pause"), pw, SLOT (play_pressed ()));
	m_play_action->setIcon (QIcon (":images/play.png"));

	if (s.value ("ui/showstop").toBool ()) {
		a = systray_menu->addAction (tr ("Stop"), pw, SLOT (playstop_pressed ()));
		a->setIcon (QIcon (":images/playstop.png"));
	}

	a = systray_menu->addAction (tr ("Next"), pw, SLOT (fwd_pressed ()));
	a->setIcon (QIcon (":images/forward.png"));
	a = systray_menu->addAction (tr ("Previous"), pw, SLOT (back_pressed ()));
	a->setIcon (QIcon (":images/back.png"));
	systray_menu->addSeparator ();
	a = systray_menu->addAction (tr ("Exit"), qApp, SLOT (quit ()));
	a->setIcon (QIcon (":images/stop.png"));

	connect (systray_menu, SIGNAL (aboutToShow ()), this, SLOT (build_menu ()));

	setContextMenu (systray_menu);

#ifdef Q_WS_MACX
	m_growl = new GrowlNotifier (this, "Esperanza", QStringList ("New song"));
	m_growl->do_registration ();
#else
	m_growl = NULL;
#endif

}

void
SystemTray::build_menu ()
{
	QSettings s;
	
	PlayerWidget *pw = dynamic_cast<PlayerWidget*> (parent ());
	if (pw->status () != Xmms::Playback::PLAYING) {
		m_play_action->setIcon (QIcon (":images/play.png"));
		m_play_action->setText (tr ("Play"));
	} else {
		m_play_action->setIcon (QIcon (":images/pause.png"));
		m_play_action->setText (tr ("Pause"));
	}

	QString show = tr ("Show main window");
	QString hide = tr ("Hide main window");
	QString st;

	if (s.value ("ui/minimode", false).toBool ()) {
		if (pw->mini_isvisible () && pw->mini_isactive ()) {
			st = hide;
		} else {
			st = show;
		}
	} else {
		if (pw->isVisible () && pw->isActiveWindow ()) {
			st = hide;
		} else {
			st = show;
		}
	}
	
	m_hide_action->setText (st);
}

void
SystemTray::toggle_hide ()
{
	QSettings s;
	PlayerWidget *pw = dynamic_cast<PlayerWidget*> (parent ());
	if (s.value ("ui/minimode", false).toBool ()) {
		pw->toggle_mini ();
	} else {
		if (pw->isVisible () && pw->isActiveWindow ()) {
			pw->hide ();
		} else {
			pw->hide ();
			pw->show ();
		}
	}
}

void
SystemTray::do_notification (const QString &title, const QString &message,
							 const QPixmap &img, MessageIcon icon,
							 int milliseconds)
{
	QSettings s;
	if (m_last_notif_str != message) {
		m_last_notif_str = message;
		if (m_display_action) {
			m_display_action->setText (message);
		}
		setToolTip(message);
		
		if (s.value ("core/donotification").toBool ()) {
			if (m_growl) {
				m_growl->do_notification ("New song", title, message, img);
			} else {
				showMessage (title, message, icon, milliseconds);
			}
		}
	}
}

void 
SystemTray::systray_trigger (QSystemTrayIcon::ActivationReason reason)
{
#ifndef Q_WS_MACX
	PlayerWidget *pw = dynamic_cast<PlayerWidget*> (parent ());
	QSettings s;

	if (reason == QSystemTrayIcon::Trigger)
	{
		if (s.value ("ui/minimode", false).toBool ()) {
			pw->toggle_mini ();
		} else {
			if (pw->isHidden() || !pw->isActiveWindow ()) {
				pw->hide();
				pw->show();
			}
			else
				pw->hide();
		}
	}
#endif
}
