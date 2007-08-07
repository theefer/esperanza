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

#ifndef __EQULIZERWIDGET_H__
#define __EQULIZERWIDGET_H__

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>

#include "equalizerdot.h"

class EqualizerWidget : public QGraphicsView
{
	Q_OBJECT
	public:
		EqualizerWidget (QWidget *);
		void setValues (const QMap<int, qreal> &);
		void setValue (int gain, qreal val) {
			m_dots[gain]->setValue (val);
		};
		
	signals:
		void gainChanged (int, qreal);
	
	private slots:
		void gain_update (int g, qreal v) {
			emit gainChanged (g, v);
		};
		
	private:
		QMap<int, EqualizerDot *> m_dots;
		QMap<int, QGraphicsLineItem *> m_lines;
};

#endif

