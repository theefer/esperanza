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


#include "preferences.h"
#include "shortcuteditor.h"

ShortCutEditor::ShortCutEditor (QWidget *parent, XClient *client) :
	PreferenceDialog (parent, client)
{
}

QList < QMap < QString, QVariant > >
ShortCutEditor::build_prefvalues ()
{
	qDebug ("apa...");
	QList < QMap < QString, QVariant > > ret;

	PREF_VALUE("shortcut/openmlibwindow", "Open medialib window", T_KEY, "m");

	return ret;
}

