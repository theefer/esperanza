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

#ifndef __ABOUTSCROLLER_H__
#define __ABOUTSCROLLER_H__

#include <QWidget>
#include <QPixmap>
#include <QTimeLine>
#include <QLinearGradient>
#include <QResizeEvent>

class AboutScroller : public QWidget
{
	Q_OBJECT

	public:
		AboutScroller (QWidget *parent);

		void paintEvent (QPaintEvent *ev);
		void update_pixmap ();

	private slots:
		void advance_y (int);
        void done_y ();

	private:
		int m_y;
		QPixmap m_p;
		QPixmap preparePixmap ();
		QPixmap m_orgpix;
		QTimeLine *m_time;
		QLinearGradient m_grad;
};

#endif
