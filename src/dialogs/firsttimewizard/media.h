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

#ifndef _MEDIA_H_
#define _MEDIA_H_

#include "page.h"
#include "ftwdialog.h"

#include "ui_media.h"

class MediaPage : public Page, public Ui::media {
	Q_OBJECT;
public:
	MediaPage(FTWDialog *parent = NULL);
	void saveSettings();

public slots:
	void showEvent(QShowEvent *ev);
	void importFile();
	void importFolder();
protected:
	FTWDialog *par;
};
#endif
