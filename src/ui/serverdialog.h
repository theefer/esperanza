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


#ifndef __SERVERDIALOG_H__
#define __SERVERDIALOG_H__

#include "xclient.h"
#include "mdns.h"

#include <QDialog>
#include <QListWidget>
#include <QWidget>
#include <QLineEdit>
#include <QKeyEvent>

class AddServerDialog : public QDialog
{
	Q_OBJECT
	public:
		AddServerDialog (QWidget *);
		QLineEdit *m_path;
		QLineEdit *m_name;
};

class ServerDialog : public QDialog
{
	Q_OBJECT
	public:
		ServerDialog (QWidget *);
		QString get_path ();
		QString get_default ();

		void keyPressEvent (QKeyEvent *);

	private slots:
		void add_server ();
		void remove_server ();
		void mdns_server_update ();

	private:
		QListWidget *m_list;
		MDNSQuery m_mdns;
		QList<QListWidgetItem *> m_mdns_servers;
};

#endif
