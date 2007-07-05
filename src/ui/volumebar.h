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


#ifndef __VOLUMEBAR_H__
#define __VOLUMEBAR_H__

#include "xclient.h"
#include "playerbutton.h"

#include <QWidget>
#include <QSlider>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMouseEvent>
#include <QShowEvent>
#include <QHideEvent>

class VolumeBar : public QWidget
{
	Q_OBJECT
	public:
		VolumeBar (QWidget *);
		/*
		void focusOutEvent (QFocusEvent *);
		*/
		bool eventFilter (QObject *obj, QEvent *ev);

		void showEvent (QShowEvent *);
		void hideEvent (QHideEvent *);

		void setValue (uint32_t i) {
			m_slider->setValue (i);
		};

	private:
		QWidget *m_parent;
		QVBoxLayout *m_box;
		QSlider *m_slider;
};

class VolumeButton : public QWidget
{
	Q_OBJECT
	public:
		VolumeButton (QWidget *, XClient *);

	private slots:
		void changed_settings ();
		void volume_pressed (QMouseEvent *);
		void set_volume (int vol);
		void got_connection (XClient *);

	private:
		PlayerButton *m_button;
		QSlider *m_slider;

		XClient *m_client;

		uint32_t m_channels;
		bool handle_volume (const Xmms::Dict &);

		QHBoxLayout *m_hbox;
		VolumeBar *m_volbar;
};

#endif
