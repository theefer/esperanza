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

#include <xmmsclient/xmmsclient++.h>
#include <QSettings>
#include <QString>
#include <QErrorMessage>

#include "misc.h"
#include "serverdialog.h"
#include "xclient.h"
#include "debug.h"

bool connectXmms2(XClient *client, QWidget *parent)
{
	QSettings s;
	QString path;
	bool b;

browser:
	if (!getenv ("XMMS_PATH")) {
		ServerDialog sd (parent);
		if (!s.value ("serverdialog/show").toBool ()) {
			path = sd.get_default ();
		} else {
			path = sd.get_path ();
		}

	} else {
		path = getenv ("XMMS_PATH");
	}

	if (path.isNull ()) {
		return false;
	}
	else if (!path.isEmpty() && path != "local") {
		DBGOUT ("trying to connect to:" << path);
		b = client->connect (path.toStdString ().c_str (), false, parent);
	}
	else {
		DBGOUT ("trying to connect to: \"\"");
		b = client->connect (NULL, false, parent);
	}

	if (!b) {
		if (!getenv ("XMMS_PATH")) {
			goto browser;
		} else {
			QErrorMessage *err = new QErrorMessage (parent);
			err->showMessage ("Your XMMS_PATH enviroment sucks. Fix it and restart the Application");
			err->exec ();
			return false;
		}
	}
	return true;
}
