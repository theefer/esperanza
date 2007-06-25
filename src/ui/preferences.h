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


#ifndef __PREFERENCES_H__
#define __PREFERENCES_H__

#include "xclient.h"

#include <QMainWindow>
#include <QTableWidget>
#include <QHash>
#include <QMap>
#include <QVariant>
#include <QLabel>
#include <QDialog>
#include <QLabel>

#include "preferencessupport.h"

#define PREF_VALUE(v,h,t,d,r) { QMap<QString, QVariant> m; m["value"]=QVariant(v); m["help"]=QVariant(h); m["type"]=QVariant(t);m["default"]=QVariant(d);m["range"]=QVariant(r);ret.append(m); }

class PreferenceDialog : public QMainWindow
{
	Q_OBJECT
	public:
		enum {
			T_BOOL,
			T_NUM,
			T_COLOR,
			T_STR,
			T_SELECTION,
			T_MULTI_SELECTION,
			T_KEY
		};

		PreferenceDialog (QWidget *, XClient *);
		static QList < QMap < QString, QVariant > > build_prefvalues ();
		static void save_defaults ();
		void fill_list ();
		void showEvent (QShowEvent *);

	private slots:
		void on_save ();
		void on_cancel ();
		void on_reset ();

	private:
		QTableWidget *m_table;
		XClient *m_client;
		QWidget *m_base;
};

#endif
