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
#include <QAction>
#include <QSettings>
#include <QWidget>

#include "globalshortcut/globalshortcutmanager.h"
#include "shortcutmanager.h"

ShortcutManager* ShortcutManager::instance_ = NULL;
ShortcutManager* ShortcutManager::instance ()
{
	if(!ShortcutManager::instance_)
		ShortcutManager::instance_ = new ShortcutManager();

	return ShortcutManager::instance_;
}

bool ShortcutManager::connect(QObject* parent, QString name, QString defKey, const char* slot, bool global /*= false*/, QObject *receiver /*= NULL*/)
{
	QSettings s;
	QString	key;
	QString tmp;
	int multiKeysIndex;
	bool ret = true;

	if(!receiver)
		receiver = parent;

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
			ret = connect(key, parent, slot, global, receiver);

			if(!ret)
				break;
		}

		// if still == 1, we have not configured multiple keys, just use the default key
		if(ret && multiKeysIndex == 1)
		{
			s.setValue (name, QVariant (defKey));
			ret = connect(defKey, parent, slot, global, receiver);
		}
	}
	else
		ret = connect(key, parent, slot, global, receiver);
	return ret;
}

bool ShortcutManager::connect(QKeySequence key, QObject* parent, const char* slot, bool global, QObject *receiver)
{
	QAction *a;

	if(!global) {
		a = new QAction (parent);
		a->setShortcut (key);
		if (parent->isWidgetType ())
			((QWidget*) parent)->addAction (a);
		receiver->connect ( a, SIGNAL (triggered ()), slot);
	}
	else
		GlobalShortcutManager::instance()->connect(key, receiver, slot);

	return true;
}
