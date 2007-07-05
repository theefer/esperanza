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


#ifndef __GROWL_H__
#define __GROWL_H__

#include <QObject>
#include <QStringList>
#include <QPixmap>

class GrowlNotifier : public QObject
{
	Q_OBJECT
	public:
		GrowlNotifier (QObject *parent, const QString &, const QStringList &);
		void do_registration ();
		void do_notification (const QString &, const QString &, const QString &, const QPixmap &img = QPixmap ());

	private:
		QByteArray m_appname;
		QStringList m_notifications;
};

#endif
