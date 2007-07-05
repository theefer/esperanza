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


#include "aboutdialog.h"
#include "aboutscroller.h"

#include <QDialog>
#include <QGridLayout>
#include <QPushButton>

AboutDialog::AboutDialog (QWidget *parent) : QDialog (parent)
{
	QGridLayout *g = new QGridLayout (this);

	QPushButton *close = new QPushButton (tr ("Close"), this);
	connect (close, SIGNAL (clicked ()), this, SLOT (accept ()));

	AboutScroller *aboutscroll = new AboutScroller (this);

	g->addWidget (aboutscroll, 0, 0, 3, 3);
	g->addWidget (close, 3, 2, 1, 1);
}

