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

#include "equalizerdialog.h"
#include "playerbutton.h"

#include <QGridLayout>
#include <QSlider>
#include <QHBoxLayout>

/* 
 * I have a instance here that takes care of the broadcast it seems like there is no
 * way remove the broadcast. So we create a instance that will be here even if we delete
 * the dialog.
 */
EqualizerConfig* EqualizerConfig::instance_ = NULL;

EqualizerConfig* EqualizerConfig::instance (XClient *client)
{

	if (!EqualizerConfig::instance_)
		EqualizerConfig::instance_ = new EqualizerConfig (client);

	return EqualizerConfig::instance_;
}

EqualizerConfig::EqualizerConfig (XClient *client) : QObject (NULL)
{
	client->config ()->broadcastValueChanged () (Xmms::bind (&EqualizerConfig::config_cb, this));
}

EqualizerDialog::EqualizerDialog (QWidget *parent, XClient *client) : QDialog (parent)
{
	setAttribute (Qt::WA_DeleteOnClose);
	setWindowTitle (tr ("Esperanza - Equalizer"));
	
	m_client = client;
	m_client->stats ()->pluginList () (Xmms::bind (&EqualizerDialog::effects, this));
	m_client->config ()->valueList () (Xmms::bind (&EqualizerDialog::value_list, this));

	EqualizerConfig *ec = EqualizerConfig::instance (m_client);
	connect (ec, SIGNAL (config_changed (const QHash<QString, QVariant> &)),
		     this, SLOT (config_changed (const QHash<QString, QVariant> &)));
	
	QGridLayout *g = new QGridLayout (this);
	
	m_preamp = new QSlider (Qt::Vertical, this);
	g->addWidget (m_preamp, 0, 0, 2, 1);
	m_preamp->setMaximum (20);
	m_preamp->setMinimum (-20);
	m_preamp->setTracking (true);
	m_preamp->setSingleStep (1);
	m_preamp->setPageStep (10);
	m_preamp->setToolTip (tr ("Pre Amplification"));
	connect (m_preamp, SIGNAL (valueChanged (int)), this, SLOT (preamp_changed (int)));
	
	m_eq = new EqualizerWidget (this);
	g->addWidget (m_eq, 0, 1, 2, 2);
	g->setRowStretch (0, 1);
	connect (m_eq, SIGNAL (gainChanged (int, qreal)),
			 this, SLOT (gain_updated (int, qreal)));

	QHBoxLayout *hbox = new QHBoxLayout (this);
	g->addLayout (hbox, 2, 0, 1, 3);

	m_cb = new QCheckBox (tr ("Enable"));
	m_cb->setEnabled (false);
	hbox->addWidget (m_cb);
	hbox->addStretch (1);
	connect (m_cb, SIGNAL (stateChanged (int)), this, SLOT (cbChanged (int)));

	PlayerButton *c = new PlayerButton (this, tr ("Reset"));
	connect (c, SIGNAL(clicked (QMouseEvent *)), this, SLOT (reset ()));
	hbox->addWidget (c);

	PlayerButton *stop = new PlayerButton (this, ":images/stop.png");
	connect (stop, SIGNAL (clicked (QMouseEvent *)),
			 this, SLOT (close ()));
	hbox->addWidget (stop);
}

void
EqualizerDialog::config_changed (const QHash<QString, QVariant> &hash)
{
	QString key = hash.keys ()[0];
	if (key.startsWith ("equalizer.gain")) {
		QString v = key.right (2);
		m_eq->setValue (v.toFloat (), hash[key].toDouble ());
	}
	if (key == "equalizer.preamp") {
		if (!m_preamp->isSliderDown ()) {
			m_preamp->setValue (hash[key].toDouble ());
		}
	}
}

void
EqualizerDialog::reset ()
{
	for (int i = 0; i < 15; i ++) {
		QString v = QString ("equalizer.gain%1").arg (i, 2, 10, QChar ('0'));
		m_client->config ()->valueSet (v.toStdString (), "0.0");
	}
	m_client->config ()->valueSet ("equalizer.preamp", "0.0");
}

void
EqualizerDialog::cbChanged (int state)
{
	if (state == Qt::Unchecked) {
		m_client->config ()->valueSet ("equalizer.enabled", "0") ();
	} else if (state == Qt::Checked) {
		m_client->config ()->valueSet ("equalizer.enabled", "1") ();
		m_client->config ()->valueSet ("effect.order.0", "equalizer") ();
		m_client->config ()->valueSet ("equalizer.bands", "15") ();
	}
}

void
EqualizerDialog::preamp_changed (int gain)
{
	QString s = QString::number (gain);
	m_client->config ()->valueSet ("equalizer.preamp", s.toStdString ()) ();
}

void
EqualizerDialog::gain_updated (int gain, qreal newvalue)
{
	QString s = QString ("equalizer.gain%1").arg (gain, 2, 10, QChar ('0'));
	QString v = QString ("%1").arg (newvalue);
	m_client->config ()->valueSet (s.toStdString (), v.toStdString ()) ();
}

bool
EqualizerDialog::effects (const Xmms::List< Xmms::Dict > &dl)
{
	bool effects = false;
	
	for (dl.first (); dl.isValid (); ++dl) {
		Xmms::Dict d = *dl;
		if (d.contains ("shortname")) {
			if (d.get<std::string> ("shortname") == "equalizer") {
				effects = true;
			}
		}
	}
	
	if (effects) {
		m_cb->setEnabled (true);
	}
	
	return true;
}

bool
EqualizerDialog::value_list (const Xmms::Dict &d)
{
	/* check if enabled */
	/* TODO: equalizer could be anywhere in the effect order */
	if (d.contains ("effect.order.0")) {
		if (d.get<std::string> ("effect.order.0") == "equalizer") {
			if (d.contains ("equalizer.enabled")) {
				if (d.get<std::string> ("equalizer.enabled") == "1") {
					if (d.contains ("equalizer.bands")) {
						if (d.get<std::string> ("equalizer.bands") == "15") {
							m_cb->setCheckState (Qt::Checked);
						}
					}
				}
			}
		}
	}

	QMap<int, qreal> ret;
	for (int i = 0; i < 15; i ++) {
		QString v = QString ("equalizer.gain%1").arg (i, 2, 10, QChar ('0'));
		if (d.contains (v.toStdString ())) {
			QString val = QString::fromStdString (d.get<std::string> (v.toStdString ()));
			qreal value = val.toFloat ();
			ret[i] = value;
		} else {
			qDebug ("Couldn't get %s", qPrintable (v));
			return true;
		}
	}
	
	m_eq->setValues (ret);
	
	if (d.contains ("equalizer.preamp")) {
		QString val = QString::fromStdString (d.get<std::string> ("equalizer.preamp"));
		m_preamp->setValue (val.toInt ());
	}
	
	return true;
}
