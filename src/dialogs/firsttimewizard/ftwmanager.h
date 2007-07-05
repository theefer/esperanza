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

#include <QErrorMessage>
#include <QList>
#include <QDialog>

#include "xclient.h"
#include "abstract_page.h"
#include "ftwdialog.h"

#ifndef _FTW_MANAGER_H_
#define _FTW_MANAGER_H_

class FTWManager : public QObject {
	Q_OBJECT
public:
	FTWManager(QWidget *parent, XClient *client_);
	bool show();
	XClient* client() { return cli; };

protected slots:
	void lastPageFinished(bool Canceled);

signals:
	void settings_changed();

protected:
	void addPage(AbstractPage* p);

	FTWDialog *ftwDlg;
	XClient *cli;
	AbstractPage* firstPage;
	AbstractPage* lastPage;
	QList<AbstractPage*> pages;
	bool Canceled;
};
#endif
