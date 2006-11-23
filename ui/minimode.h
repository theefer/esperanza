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


#ifndef __MINIMODE_H__
#define __MINIMODE_H__

#include "xclient.h"
#include "progressframe.h"
#include "playerbutton.h"
#include "volumebar.h"

#include <QWidget>
#include <QFrame>

class MiniMode : public QFrame
{
	Q_OBJECT
	public:
		MiniMode (QWidget *, XClient *);
		void update_playbutton (const QString &s)
		{
			m_playbutt->setPx (s);
		};

		void setText (const QString &s)
		{
			m_progress->setText (s);
		};

		void setMaximum (uint32_t max)
		{
			m_progress->setMaximum (max);
		};

		void setValue (uint32_t val)
		{
			m_progress->setValue (val);
		};

		void moveEvent (QMoveEvent *);
		void showEvent (QShowEvent *);

		void keyPressEvent (QKeyEvent *);

	private slots:
		void min_pressed ();
		void changed_settings ();

	private:
		XClient *m_client;
		ProgressFrame *m_progress;
		QWidget *m_parent;

		PlayerButton *m_playbutt;
		PlayerButton *m_stop;
		VolumeButton *m_volume;
};

#endif
