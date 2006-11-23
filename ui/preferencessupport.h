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


#ifndef __PREFERENCESSUPPORT_H__
#define __PREFERENCESSUPPORT_H__

#include <QMainWindow>
#include <QTableWidget>
#include <QHash>
#include <QMap>
#include <QVariant>
#include <QLabel>
#include <QDialog>
#include <QLabel>

class ColorButton : public QLabel
{
	Q_OBJECT
	public:
		ColorButton (QWidget *, const QColor &);
		QColor current_color () const;
		void mouseDoubleClickEvent (QMouseEvent *);

	private:
		QColor m_color;
};

class KeyInputDialog : public QDialog
{
	Q_OBJECT
	public:
		KeyInputDialog (QWidget *, const QString & = QString ());
		void keyPressEvent (QKeyEvent *);
		QString get_key () const {
			return m_key;
		};

	private:
		QLabel *m_l;
		QString m_key;
};

class KeyWidget : public QLabel
{
	public:
		KeyWidget (QWidget *, const QString & = QString ());
		void mouseDoubleClickEvent (QMouseEvent *);
		QString get_key () const {
			return m_key;
		};

	private:
		QString m_key;
};


#endif
