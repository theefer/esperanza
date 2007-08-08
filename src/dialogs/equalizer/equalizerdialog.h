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

#ifndef __EQUALIZERDIALOG_H__
#define __EQUALIZERDIALOG_H__

#include "xclient.h"
#include "equalizerwidget.h"
#include <QDialog>
#include <QCheckBox>
#include <QSlider>

class EqualizerConfig : public QObject
{
	Q_OBJECT
	public:
		EqualizerConfig (XClient *);
		static EqualizerConfig *instance (XClient *);
		
	signals:
		void config_changed (const QHash<QString, QVariant> &);
	
	private slots:
		bool config_cb (const Xmms::Dict &d) {
			emit config_changed (XClient::convert_dict (d));
			return true;
		};
		
	private:
		static EqualizerConfig *instance_;
};

class EqualizerDialog : public QDialog
{
	Q_OBJECT
	public:
		EqualizerDialog (QWidget *, XClient *);
		
	private slots:
		bool effects (const Xmms::List< Xmms::Dict > &);
		bool value_list (const Xmms::Dict &);
		void gain_updated (int, qreal);
		void cbChanged (int);
		void reset ();
		
		void preamp_changed (int);
		
		void config_changed (const QHash<QString, QVariant> &);
		
	private:
		XClient *m_client;
		QCheckBox *m_cb;
		EqualizerWidget *m_eq;
		QSlider *m_preamp;
	
};

#endif
