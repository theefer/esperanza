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
#include <QMessageBox>

#include "server.h"
#include "xclient.h"
#include "ftwmanager.h"

#include "misc.h"

ServerPage::ServerPage(FTWDialog *parent_)
: Page(parent_)
{
	Ui::server::setupUi(this->Content);
    connect(Ui::server::pbTry, SIGNAL(clicked()), SLOT(tryit()));
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
		
	qDebug(qPrintable(QString("Trying Connection to: %1").arg(sTmp)));

	m[sDefName] = QVariant(sTmp);
	s.setValue ("serverbrowser/default", sDefName);
	s.setValue ("serverbrowser/list", m);
}

void ServerPage::nextPage()
{
	if(!parent->manager()->client()->isConnected())
		tryit();

	if(parent->manager()->client()->isConnected())
		Page::nextPage();
}

void ServerPage::tryit()
{
	QString sTmp = serverConPath->text();
	char *p = NULL;

	if (sTmp == "local" || sTmp.isEmpty()) {
		p = NULL;
	} else {
		p = sTmp.toAscii().data();
	}

	if(parent->manager()->client()->connect(p, false, parent))
	{
		QMessageBox::information(this, tr("Connected"), tr("You successfully connected Xmms2d."));
		saveSettings();
	}
}

void ServerPage::showEvent(QShowEvent *ev)
{
	parent->setWindowTitle("Server Connection Path Setting");
	this->header->setText("<b>Here you can set your server connection path.</b>");
}
