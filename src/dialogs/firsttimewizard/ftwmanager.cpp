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

#include "ftwmanager.h"

// pages
#include "welcome.h"
#include "server.h"
#include "media.h"
#include "gui.h"
#include "finish.h"

FTWManager::FTWManager(QWidget *parent, XClient *client_)
{
	cli = client_;
	firstPage = NULL;
	lastPage = NULL;
	ftwDlg = new FTWDialog(parent, this);
	addPage(new Welcome(ftwDlg));
	addPage(new ServerPage(ftwDlg));
	addPage(new MediaPage(ftwDlg));
	addPage(new GuiPage(ftwDlg));
	addPage(new Finish(ftwDlg));
	Canceled = true;
}

void FTWManager::addPage(AbstractPage *p)
{
	if(!firstPage)
		firstPage = p;
	else
		lastPage->setNext(p);
	p->setPrevious(lastPage);
	lastPage = p;

	connect(p, SIGNAL(lastClosed(bool)), this, SLOT(lastPageFinished(bool)));
	pages.append(p);
	p->hide();
}

bool FTWManager::show()
{
	if(firstPage) {
		firstPage->show();
		ftwDlg->exec();
	}
	return !Canceled;
}

void FTWManager::lastPageFinished(bool Canceled_)
{
	AbstractPage *p;
	Canceled = Canceled_;

	if(!Canceled)
		while (!pages.isEmpty()) {
			p = pages.takeFirst();
			p->saveSettings();
		}
	ftwDlg->close();
}
