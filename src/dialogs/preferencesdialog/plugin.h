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

#include "esperanza_plugin.h"
#include "preferencesdialog.h"

namespace Preferences {
	class Plugin : public EsperanzaMain::EsperanzaDialog {
		Q_OBJECT
		Q_INTERFACES(EsperanzaMain::EsperanzaDialog)
		public:
			virtual QString label () const { return tr ("Preferences"); }
			virtual EsperanzaMain::DialogItem item () const { return EsperanzaMain::DialogSettings; }
			virtual QString shortcut () const { return "P"; }
		
			virtual QDialog *create (QWidget *parent, XClient *client) const {
				return new PreferencesDialog (parent, client);
			}
	};
}

