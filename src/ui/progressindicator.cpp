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


#include "progressindicator.h"

#include <QLabel>

ProgressIndicator::ProgressIndicator (QWidget *parent) : QLabel (parent)
{
	m_movie = new QMovie (":images/progress.mng", NULL, this);
	setMovie (m_movie);
	hide ();
}

void
ProgressIndicator::setStatus (bool go)
{
	if (go) {
		show ();
		m_movie->start ();
	} else {
		hide ();
		m_movie->stop ();
	}
}

