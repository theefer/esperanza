/** 
 *  This file is a part of Esperanza, an XMMS2 Client.
 *
 *  Copyright (C) 2005-2006 Cestonaro Thilo <c.thilo@gmx.de>
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
#include <QShortcut>
#include <QSettings>
#include <QWidget>

#include "globalshortcutmanager.h"
#include "shortcutmanager.h"

ShortcutManager* ShortcutManager::instance_ = NULL;
ShortcutManager* ShortcutManager::instance ()
{
	if(!ShortcutManager::instance_)
		ShortcutManager::instance_ = new ShortcutManager();

	return ShortcutManager::instance_;
}

bool ShortcutManager::connect(QWidget* parent, QString name, QString defKey, const char* slot, bool global /*= false*/)
{
	QSettings s;
	QString	key;
	QString tmp;
	int multiKeysIndex;
	bool ret = true;

	key = s.value (name, QVariant (defKey)).toString ();
	if(key == defKey)
	{
		// if we got the same as defkey, check wether there are multiple keys configured
		for ( multiKeysIndex = 1; ; multiKeysIndex++ )
		{
			tmp = name + QString ("_%1").arg (multiKeysIndex);
			key = s.value (tmp, QVariant ("")).toString ();
			if(key == "")
				break;

			// use the configured key
			ret = connect(key, parent, slot, global);

			if(!ret)
				break;
		}

		// if still == 1, we have not configured multiple keys, just use the default key
		if(ret && multiKeysIndex == 1)
		{
			s.setValue (name, QVariant (defKey));
			ret = connect(defKey, parent, slot, global);
		}
	}
	else
		ret = connect(key, parent, slot, global);
	return ret;
}

bool ShortcutManager::connect(QKeySequence key, QWidget* parent, const char* slot, bool global)
{
	QShortcut *shortcut;
	bool ret = false;

	if(!global) {
		shortcut = new QShortcut (key, parent);
		ret = QObject::connect ( shortcut, SIGNAL (activated ()), parent, slot);
	}
	else {
		// no return from connect here. but it will work!! :)
		ret = true;
		GlobalShortcutManager::instance()->connect(key, parent, slot);
	}

	return ret;
}
