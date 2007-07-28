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


#ifndef _SYSTEMTRAY_H
#define _SYSTEMTRAY_H

#include "xclient.h"

#include <QSystemTrayIcon>
#include <QAction>

#include "growl.h"

class SystemTray : public QSystemTrayIcon
{
	Q_OBJECT
	public:
		SystemTray (QObject *, XClient *);

		void do_notification (const QString &title, const QString &message, const QPixmap &img = QPixmap (), MessageIcon icon = Information, int milliseconds = 10000);

	private slots:
		void systray_trigger (QSystemTrayIcon::ActivationReason reason);
		void toggle_hide ();
		void build_menu ();
		void got_connection (XClient *);

	private:
		bool handle_current_id (const unsigned int &id);

		QAction *m_display_action;
		QAction *m_hide_action;
		QAction *m_play_action;
		GrowlNotifier *m_growl;
		QString m_last_notif_str;
		XClient *m_client;
};

#endif
