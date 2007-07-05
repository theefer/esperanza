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


#include "growl.h"

#include "mac_growl.h"

#include <QDataStream>
#include <QByteArray>
#include <QIODevice>
#include <QHash>
#include <QVariant>
#include <QPixmap>

GrowlNotifier::GrowlNotifier (QObject *parent,
							  const QString &appname,
							  const QStringList &notifications) :
	QObject (parent)
{
	m_appname = appname.toUtf8 ();
	m_notifications = notifications;
}

void
GrowlNotifier::do_notification (const QString &notif,
								const QString &title,
								const QString &desc,
								const QPixmap &img)
{
	QHash<QString, QVariant> note;

	note["ApplicationName"] = QVariant (QString (m_appname));
	note["NotificationName"] = QVariant (notif);
	note["NotificationTitle"] = QVariant (title);
	note["NotificationDescription"] = QVariant (desc);
	note["NotificationIcon"] = QVariant (img);

#ifdef Q_OS_MACX
	mac_growl_post_notification ("GrowlNotification", note);
#endif
}

void
GrowlNotifier::do_registration ()
{
	QHash<QString, QVariant> note;

	note["ApplicationName"] = QVariant (QString (m_appname));
	note["AllNotifications"] = QVariant (m_notifications);
	note["DefaultNotifications"] = QVariant (m_notifications);
//	note["ApplicationIcon"] = QVariant (QImage (":images/test.jpg"));

#ifdef Q_OS_MACX
	mac_growl_post_notification ("GrowlApplicationRegistrationNotification", note);
#endif
}

