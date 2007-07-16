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


#include "volumebar.h"
#include "playerbutton.h"

#include <QVBoxLayout>
#include <QSettings>
#include <QMouseEvent>
#include <QApplication>
#include <QSettings>

VolumeButton::VolumeButton (QWidget *parent, XClient *client) : QWidget (parent)
{
	m_client = client;

	setMaximumSize (100, 20);
	setToolTip ("Volumebar");

	/* Create the slider */
	m_slider = new QSlider (Qt::Horizontal, this);
	m_slider->hide ();
	m_slider->setMaximum (100);
	m_slider->setMinimum (0);
	m_slider->setTickInterval (5);

	m_volbar = new VolumeBar (this);

	connect (m_client, SIGNAL (gotConnection (XClient *)), this, SLOT (got_connection (XClient *)));
	connect (m_slider, SIGNAL (valueChanged (int)), this, SLOT (set_volume (int)));

	/* we add both and let change_settings decide */
	m_hbox = new QHBoxLayout (this);
	m_hbox->setMargin (0);
	m_button = new PlayerButton (this, ":images/volume.png");
	m_button->hide ();

	connect (m_button, SIGNAL (clicked (QMouseEvent *)),
			 this, SLOT (volume_pressed (QMouseEvent *)));

	m_hbox->addWidget (m_button);
	m_hbox->addWidget (m_slider);

	connect (m_client->settings (), SIGNAL (settingsChanged ()),
			 this, SLOT (changed_settings ()));

	changed_settings ();
}

void
VolumeButton::changed_settings ()
{
	QSettings s;

	if (s.value ("ui/volumepopup").toBool ()) {
		m_slider->hide ();
		m_button->show ();
	} else {
		m_button->hide ();
		m_slider->show ();
	}

	if (s.value ("ui/volumeinteractive").toBool ()) {
		m_slider->setTracking (true);
	} else { 
		m_slider->setTracking (false);
	}

}

void
VolumeButton::volume_pressed (QMouseEvent *ev)
{
	m_volbar->move (ev->globalPos ());
	m_volbar->show ();
}

void
VolumeButton::got_connection (XClient *c)
{
	m_client->playback ()->volumeGet () (Xmms::bind (&VolumeButton::handle_volume, this));
	m_client->playback ()->broadcastVolumeChanged () (Xmms::bind (&VolumeButton::handle_volume, this));
}

bool
VolumeButton::handle_volume (const Xmms::Dict &d)
{
	/* XXX: this function really should do each and sum / split the channels */
	if (d.contains ("master")) {
		if (!m_slider->isSliderDown ()) {
			m_slider->setValue (d.get<uint32_t> ("master"));
			m_volbar->setValue (d.get<uint32_t> ("master"));
		}
		m_channels = 1;
	} else if (d.contains ("left") && d.contains ("right")) {
		if (!m_slider->isSliderDown ()) {
			m_slider->setValue ((d.get<uint32_t> ("left") + d.get<uint32_t> ("right")) / 2);
			m_volbar->setValue ((d.get<uint32_t> ("left") + d.get<uint32_t> ("right")) / 2);
		}
		m_channels = 2;
	}
	return true;
}

void
VolumeButton::set_volume (int vol)
{
	if (m_channels == 1) {
		m_client->playback ()->volumeSet ("master", vol) ();
	} else {
		m_client->playback ()->volumeSet ("left", vol) ();
		m_client->playback ()->volumeSet ("right", vol) ();
	}
}

VolumeBar::VolumeBar (QWidget *parent) : QWidget (NULL)
{
	setWindowFlags (Qt::FramelessWindowHint | Qt::Popup);

	m_parent = parent;

	m_box = new QVBoxLayout (this);

	m_slider = new QSlider (Qt::Vertical, this);
	m_slider->setMaximum (100);
	m_slider->setMinimum (0);
	m_slider->setTickInterval (5);
	m_slider->setTracking (false);
	connect (m_slider, SIGNAL (valueChanged (int)), parent, SLOT (set_volume (int)));
	resize (m_slider->sizeHint ());

	m_box->addWidget (m_slider);

	setFocusPolicy (Qt::StrongFocus);

	hide ();
}

bool
VolumeBar::eventFilter (QObject *obj, QEvent *ev)
{
	if (isVisible ()) {
		if (ev->type () == QEvent::MouseButtonPress) {
			QMouseEvent *mev = (QMouseEvent *) ev;
			QPoint globalPos = mapToGlobal (mev->pos ());
			if (!(QApplication::widgetAt (globalPos) == this ||
				  QApplication::widgetAt (globalPos) == m_slider)) {

				hide ();
				return true;
			}
		}
	}
	return QWidget::eventFilter (obj, ev);
}

void
VolumeBar::showEvent (QShowEvent *ev)
{
	foreach (QWidget *widget, QApplication::topLevelWidgets()) {
		widget->installEventFilter (this);
	}

	raise ();
}

void
VolumeBar::hideEvent (QHideEvent *ev)
{
	foreach (QWidget *widget, QApplication::topLevelWidgets()) {
		widget->removeEventFilter (this);
	}
}

/*
void
VolumeBar::focusOutEvent (QFocusEvent *ev)
{
	hide ();
}
*/

