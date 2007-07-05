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

#ifndef _FTWDIALOG_H_
#define _FTWDIALOG_H_

#include <QDialog>

#include "abstract_page.h"
#include "ui_ftwdialog.h"

class FTWManager;

class FTWDialog : public QDialog, public Ui::FTWDialog {
public:
	FTWDialog(QWidget *parent, FTWManager *ftwman_)
	: QDialog(parent)
	{
		setupUi(this);
		ftwman = ftwman_;
		setModal(true);
	}

	FTWManager* manager() { return ftwman; };

protected:
	FTWManager *ftwman;
};
#endif
