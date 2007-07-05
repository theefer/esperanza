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


#ifndef __PLAYERBUTTON_H__
#define __PLAYERBUTTON_H__

#include <QLabel>
#include <QPixmap>
#include <QImage>

class PlayerButton : public QLabel
{
	Q_OBJECT
	public:
		PlayerButton (QWidget *, const QString &);
		void mousePressEvent (QMouseEvent *);
		void setPx (const QString &);

		void enterEvent (QEvent *);
		void leaveEvent (QEvent *);

	signals:
		void clicked (QMouseEvent *);

	private:
		QPalette m_normal;
		QPalette m_hoover;
		QPixmap m_pixmap;
		QPixmap m_pixmap_hoover;
};

#endif
