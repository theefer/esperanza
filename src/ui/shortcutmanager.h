/** 
 *  This file is a part of Esperanza, an XMMS2 Client.
 *
 *  Copyright (C) 2005-2007 Cestonaro Thilo <c.thilo@gmx.de>
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

#ifndef __SHORTCUTMANAGER_H__
#define __SHORTCUTMANAGER_H__

#include <QWidget>

class ShortcutManager : public QObject
{
	Q_OBJECT
	public:
		static ShortcutManager* instance ();
		bool connect(QObject* parent, QString name, QString defKey, const char* slot, bool global = false, QObject *receiver = NULL);

	protected:
		bool connect(QKeySequence key, QObject* parent, const char* slot, bool global, QObject *receiver);
		ShortcutManager () {};
		~ShortcutManager () {};
		static ShortcutManager* instance_;
};

#endif
