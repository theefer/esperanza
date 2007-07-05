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


#include "valuebar.h"

#include <QWidget>
#include <QPainter>
#include <QColor>
#include <QPalette>
#include <QRect>
#include <QLinearGradient>

ValueBar::ValueBar (QWidget *parent) : QWidget (parent)
{
	m_value = 0;
	setMaximumHeight (22);
}

void
ValueBar::paintEvent (QPaintEvent *ev)
{
	QPainter paint;
	paint.begin (this);

	int w = (int)((float) m_value / (float) 100 * width ());

	QPalette p = palette ();

	QColor cactivetop = p.highlight ().color ();
	QColor cactivemiddle = cactivetop.dark ();
	QColor cactivebottom = cactivemiddle.dark ();

	QLinearGradient gradientActive (0, 0, 0, height ());
	gradientActive.setColorAt (0, cactivetop);
	gradientActive.setColorAt (0.5, cactivemiddle);
	gradientActive.setColorAt (1, cactivebottom);

	paint.fillRect (rect (), QBrush (Qt::white));
	QRect r (0, 0, w, height());
	paint.fillRect (r, gradientActive);

	paint.drawRect (0, 0, width () - 1, height () - 1);

	paint.end ();
}
