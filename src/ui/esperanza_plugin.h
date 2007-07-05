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

#ifndef __ESPERANZA_PLUGIN_H__
#define __ESPERANZA_PLUGIN_H__

#include "xclient.h"

#include <QWidget>
#include <QDialog>

namespace EsperanzaMain {
	enum DialogItem {
		DialogNone,
		DialogInfo,
		DialogPlaylist,
		DialogSettings
	};
	
	class EsperanzaDialog : public QObject
	{
		Q_OBJECT
		public:
			EsperanzaDialog () { init (); }
			virtual ~EsperanzaDialog () { };
			void init (QWidget *parent_ = NULL, XClient *client_ = NULL) { parent = parent_; client = client_; };
			virtual QString label () const = 0;
			virtual EsperanzaMain::DialogItem item () const = 0;
			virtual QString shortcut () const = 0;
			
			virtual QDialog *create (QWidget *, XClient *) const = 0;

		public slots:
			void showit () {
				QDialog *d;
				if(!parent || !client)
					return;
				d = create (parent, client);
				if (d)
					d->show ();
			};
		private:
			QWidget *parent;
			XClient *client;
	};
}

Q_DECLARE_INTERFACE(EsperanzaMain::EsperanzaDialog, "org.xmms.clients.Esperanza.EsperanzaDialog/0.4")

#endif
