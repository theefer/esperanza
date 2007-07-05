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

#include <QApplication>
#include <QWidget>
#include <QMessageBox>

#include "abstract_page.h"
#include "ui_finish.h"

#ifndef _FINISH_H_
#define _FINISH_H_

class Finish : public AbstractPage, public Ui::Finish {
	Q_OBJECT

public:
	Finish(QWidget * parent = NULL)
	: AbstractPage(parent)
	{
		setupUi(this);
	}
};
#endif
