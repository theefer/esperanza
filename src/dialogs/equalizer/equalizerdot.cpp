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

#include "equalizerdot.h"

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>

EqualizerDot::EqualizerDot (QGraphicsScene *scene, int gainlevel) : QObject (scene), QGraphicsEllipseItem (0, 0, 6.0, 6.0)
{
	m_gainlevel = gainlevel;
	scene->addItem (this);
	setAcceptsHoverEvents (true);
	m_ismoving = false;
	m_left = NULL;
	m_right = NULL;
	m_text = scene->addText ("0.0 dB");
	m_text->hide ();

	m_label = scene->addText (QString ("Band: ") + level_to_name (gainlevel));
	m_label->setPos (scene->width () / 2.0 - (m_label->textWidth () / 2.0), 10);
	m_label->hide ();
}

void
EqualizerDot::setValue (qreal value2)
{
	if (m_ismoving) {
		return;
	}
	
	qreal zero = scene ()->height () / 2.0;

	qreal x = pos ().x ();
	qreal y = zero - 3;

	if (value2 != 0) {
		if (value2 > 0) {
			y = ((20.0 - value2) / 20.0) * zero;
		} else {
			y = ((-value2 / 20.0) * zero) + zero;
		}
	}

	setPos (x, y);

	QString s = QString ("%1 dB").arg (value ());
	m_text->setPlainText (s);

	m_left->setLine (m_left->line ().x1 (), m_left->line ().y1 (), x + 3.0, y + 3.0);
	m_right->setLine (x + 3.0, y + 3.0, m_right->line ().x2 (), m_right->line ().y2 ());
}

qreal
EqualizerDot::value ()
{
	qreal zero = scene ()->height () / 2.0;
	qreal ret = 0.0;
	qreal p = pos ().y () + 3;
	if (p < zero) {
		ret = 20.0 - (20.0 * (p / zero));
	} else {
		/* negative */
		ret = -(20.0 * ((p - zero) / zero));
	}

	return ret;
}

void
EqualizerDot::hoverEnterEvent (QGraphicsSceneHoverEvent *ev)
{
	Q_UNUSED (ev);
	m_label->show ();
}

void
EqualizerDot::hoverLeaveEvent (QGraphicsSceneHoverEvent *ev)
{
	Q_UNUSED (ev);
	m_label->hide ();
}

void
EqualizerDot::mousePressEvent (QGraphicsSceneMouseEvent *ev)
{
	Q_UNUSED (ev);
	m_ismoving = true;
	m_text->show ();
	m_text->setPos (pos ().x (), pos ().y () + 10);
}

void
EqualizerDot::mouseReleaseEvent (QGraphicsSceneMouseEvent *ev)
{
	Q_UNUSED (ev);
	m_ismoving = false;
	m_text->hide ();
}

void
EqualizerDot::mouseMoveEvent (QGraphicsSceneMouseEvent *ev)
{
	if (!m_ismoving)
		return;

	QPointF m = mapToParent (ev->pos ()) - mapToParent (ev->lastPos ());

	qreal y = pos ().y () + 3;
	qreal x = pos ().x ();

	if (y + m.y () < 0) {
		return;
	}

	if (y + m.y () > scene ()->height ()) {
		return;
	}

	moveBy (0, m.y ());

	y = pos ().y ();
	x = pos ().x ();

	qreal val = value ();
	emit gainChanged (m_gainlevel, val);
	QString s = QString ("%1 dB").arg (val);
	m_text->setPlainText (s);

	qreal textx = 0.0;
	qreal texty = 0.0;

	if (y > (scene ()-> height () / 2.0)) {
		texty = y + 10;
	} else {
		texty = y - 20;
	}

	if (y - 25 < 0) {
		textx = x + 20;
		texty = 0;
	} else if (y + 30 > scene ()->height ()){
		textx = x + 20;
		texty = scene ()->height () - 20;
	} else {
		textx = x;
	}

	if (textx + 30 > scene ()->width ()) {
		textx -= 60;
	}


	m_text->setPos (textx, texty);

	/* move the lines */
	m_left->setLine (m_left->line ().x1 (), m_left->line ().y1 (), x + 3.0, y + 3.0);
	m_right->setLine (x + 3.0, y + 3.0, m_right->line ().x2 (), m_right->line ().y2 ());
}

