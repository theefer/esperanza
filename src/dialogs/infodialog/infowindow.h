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


#ifndef __INFOWINDOW_H__
#define __INFOWINDOW_H__

#include "xclient.h"

#include <QWidget>
#include <QTabWidget>
#include <QLabel>
#include <QUrl>
#include <QTextBrowser>
#include <QTextDocument>

class InfoBrowser : public QTextBrowser
{
	Q_OBJECT
	public:
		InfoBrowser (QWidget *parent);
		void setSource (const QUrl &name);
};

class InfoWindow : public QWidget
{
	Q_OBJECT

	public:
		InfoWindow (QWidget *, XClient *);

	private slots:
		void set_current_id (uint32_t id);
		void entry_changed (uint32_t);
		void change_current (int);
		void link_clicked (const QUrl &);

	private:
		QTextDocument *m_doc;
		XClient *m_client;
		QTabWidget *m_tab;
		QLabel *m_label;

		uint32_t m_current_id;
};

#endif
