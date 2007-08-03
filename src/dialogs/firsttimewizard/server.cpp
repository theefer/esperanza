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

#include <xmmsclient/xmmsclient++.h>

#include <QSystemTrayIcon>
#include <QSettings>
#include <QMovie>

#include "server.h"
#include "xclient.h"
#include "ftwmanager.h"

#include "misc.h"
#include "debug.h"

#define NOTCONNECTED "<html><body><span style=\" color:#8b0000;\">not Connected</span></body></html>"
#define CONNECTED "<html><body><span style=\" color:#008b00;\">Connected</span></body></html>"

ServerPage::ServerPage(FTWDialog *parent_)
: Page(parent_)
{
	Ui::server::setupUi(this->Content);
    connect(Ui::server::pbTry, SIGNAL(clicked()), SLOT(tryit()));
	connect(Ui::server::serverConPath, SIGNAL(textChanged(QString)), SLOT(textChanged(QString)));
	Ui::server::conStatus->setText(tr(NOTCONNECTED));
	parent = parent_;
}

/*
 * \brief this function needs to be implemented when a dialog
 * should save some settings which the user has configured.
 */
void ServerPage::saveSettings()
{
	QSettings s;
	QString sDefName, sTmp;
	QMap<QString, QVariant> m;

	sDefName = "default";
	sTmp = serverConPath->text();
	if(sTmp.isEmpty())
		sTmp = "local";
		
	m[sDefName] = QVariant(sTmp);
	s.setValue ("serverbrowser/default", sDefName);
	s.setValue ("serverbrowser/list", m);
}

void ServerPage::textChanged(QString sText)
{
	Q_UNUSED (sText);
	QSettings s;

	Ui::server::conStatus->setText(tr(NOTCONNECTED));
	parent->manager()->client()->disconnect ();

	s.setValue ("serverbrowser/default", "");
	s.setValue ("serverbrowser/list", "");

}

void ServerPage::nextPage()
{
	tryit();
	if(parent->manager()->client()->isConnected())
		Page::nextPage();
}

void ServerPage::tryit()
{
	QString sTmp = serverConPath->text();
	bool b = false;

	Ui::server::conStatus->setMovie (new QMovie (":images/progress.mng", NULL, Ui::DefaultPage::pbNext));
	Ui::server::conStatus->movie ()->start ();	

	if (!sTmp.isEmpty() && sTmp != "local") {
		b = parent->manager ()->client ()->connect (sTmp.toStdString ().c_str (), false, parent);
	}
	else {
		b = parent->manager ()->client ()->connect (NULL, false, parent);
	}

	Ui::server::conStatus->movie ()->stop ();	

	if (b)
	{
		Ui::server::conStatus->setText(tr(CONNECTED));
		saveSettings();
		return;
	}

	Ui::server::conStatus->setText(tr(NOTCONNECTED));
}

void ServerPage::showEvent(QShowEvent *ev)
{
	parent->setWindowTitle("Server Connection Path Setting");
	this->header->setText("<b>Here you can set your server connection path.</b>");
}
