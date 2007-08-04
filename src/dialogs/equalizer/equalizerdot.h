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

#ifndef __EQUALIZERDOT_H__
#define __EQUALIZERDOT_H__

#include <QGraphicsEllipseItem>

class EqualizerDot : public QObject, public QGraphicsEllipseItem
{
	Q_OBJECT
	public:
		EqualizerDot (QGraphicsScene *, int);

		void mousePressEvent (QGraphicsSceneMouseEvent *);
		void mouseReleaseEvent (QGraphicsSceneMouseEvent *);
		void mouseMoveEvent (QGraphicsSceneMouseEvent *);

		void setRight (QGraphicsLineItem *r) {
			m_right = r;
		};

		void setLeft (QGraphicsLineItem *l) {
			m_left = l;
		};

		qreal value ();

		void hoverEnterEvent (QGraphicsSceneHoverEvent *);
		void hoverLeaveEvent (QGraphicsSceneHoverEvent *);


		static QString level_to_name (int level)
		{
			/* 25,40,63,100,160,250,400,630,1000,1600,2500,4000,6300,10000,16000 */
			switch (level) {
				case 0:
					return QString ("24");
				case 1:
				return QString ("40");
				case 2:
				return QString ("63");
				case 3:
				return QString ("100");
				case 4:
				return QString ("160");
				case 5:
				return QString ("250");
				case 6:
				return QString ("400");
				case 7:
				return QString ("630");
				case 8:
				return QString ("1k");
				case 9:
				return QString ("1.6k");
				case 10:
				return QString ("2.5k");
				case 11:
				return QString ("4k");
				case 12:
				return QString ("6.3k");
				case 13:
				return QString ("10k");
				case 14:
				return QString ("16k");
				default:
				return QString ("Unknown");
			}
		};

		int gain () const
		{
			return m_gainlevel;
		};

		void setValue (qreal);

	signals:
		void gainChanged (int, qreal);

	private:
		int m_gainlevel;
		bool m_ismoving;

		QGraphicsLineItem *m_right;
		QGraphicsLineItem *m_left;
		QGraphicsTextItem *m_text;
		QGraphicsTextItem *m_label;
};


#endif
