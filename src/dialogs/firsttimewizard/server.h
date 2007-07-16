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

#include "page.h"

#include "ui_server.h"
#include "ftwdialog.h"

#ifndef _SERVER_H_
#define _SERVER_H_

class ServerPage : public Page, public Ui::server {
	Q_OBJECT;
public:
	ServerPage(FTWDialog *parent_ = NULL);
	void saveSettings();

public slots:
	void textChanged(QString);
	void showEvent(QShowEvent *ev); 
	void tryit();
	void nextPage();

protected:
	FTWDialog *parent;
};
#endif
