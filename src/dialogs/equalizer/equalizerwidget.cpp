/** 
 *  This file is a part of Esperanza, an XMMS2 Client.
 *
 *  Copyright (C) 2005-2006 XMMS2 Team
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

#include "equalizerwidget.h"

#include <QPainter>
#include <QGraphicsSceneMouseEvent>

EqualizerWidget::EqualizerWidget (QWidget *parent) : QGraphicsView (parent)
{
	QGraphicsScene *sc = new QGraphicsScene (0, 0, 600, 250, this);
	
	/* set options */
	setRenderHints (QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
	
	/* add lines */
	sc->addLine (0, sc->height () / 6.0, sc->width (), sc->height () / 6.0, QPen (Qt::gray));
	sc->addLine (0, sc->height () / 3.0, sc->width (), sc->height () / 3.0, QPen (Qt::gray));
	sc->addLine (0, sc->height () / 2.0, sc->width (), sc->height () / 2.0, QPen (Qt::gray));
	sc->addLine (0, sc->height () / 1.2, sc->width (), sc->height () / 1.2, QPen (Qt::gray));
	sc->addLine (0, sc->height () / 1.5, sc->width (), sc->height () / 1.5, QPen (Qt::gray));
	
	/* do dot configuration */
	float numdots = 15.0;
	
	float realwidth = sc->width () - (6.0 * 4.0);
	float dsp = realwidth / (numdots - 1.0);
	
	float x = 6.0 * 2.0;
		
	/* add the dots */
	for (int i = 0; i < numdots; i ++) {
		EqualizerDot *it = new EqualizerDot (sc, i);
		it->setPos (x - 3.0, (sc->height () / 2.0) - 3.0);
		m_dots[i] = it;
		connect (it, SIGNAL (gainChanged (int, qreal)), this, SLOT (gain_update (int, qreal)));
		
		x += dsp;
	}
	
	/* add the line between the dots */
	QPen pen;
	pen.setWidth (2);
	int startx = 0;
	int starty = sc->height () / 2.0;
	for (int i = 0; i < numdots; i ++) {
		QGraphicsLineItem *line = sc->addLine (0, 0, 0, 0, pen);
		line->setLine (startx, starty, m_dots[i]->pos ().x () + 3.0, m_dots[i]->pos ().y () + 3.0);
		if (i > 0) {
			m_dots[i - 1]->setRight (line);
		}
		m_dots[i]->setLeft (line);
		startx = m_dots[i]->pos ().x () + 3.0;
		starty = m_dots[i]->pos ().y () + 3.0;
	}
	
	/* add the last line */
	EqualizerDot *dot = m_dots[numdots - 1];
	dot->setRight (sc->addLine (dot->pos ().x () + 3.0, dot->pos ().y () + 3.0, sc->width (), sc->height () / 2.0, pen));
	
	/* Set the scene */
	setScene (sc);
	
	/* make sure it's static in size */
	setMaximumSize (sc->width (), sc->height ());
	setMinimumSize (sc->width (), sc->height ());
	setHorizontalScrollBarPolicy (Qt::ScrollBarAlwaysOff);
	setVerticalScrollBarPolicy (Qt::ScrollBarAlwaysOff);
}

void
EqualizerWidget::setValues (const QMap<int, qreal> &vals)
{
	for (int i = 0; i < vals.size (); i ++) {
		m_dots[i]->setValue (vals[i]);
	}
}
