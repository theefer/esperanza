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


#include "preferencemanager.h"

#include <QObject>
#include <QSystemTrayIcon>
#include <QSettings>

PreferenceManager* PreferenceManager::instance_ = NULL;

PreferenceManager* PreferenceManager::instance ()
{

	if (!PreferenceManager::instance_)
		PreferenceManager::instance_ = new PreferenceManager ();

	return PreferenceManager::instance_;
}

QList<PreferenceValue *>
PreferenceManager::values (const PreferenceValue::ValueSection &sec) const
{
	if (sec == PreferenceValue::All) {
		return m_values.values ();
	}

	return QList<PreferenceValue *> ();
}

QStringList
PreferenceManager::keys () const
{
	return m_values.keys ();
}

void
PreferenceManager::registerVal (const QString &key,
								const QString &help,
								const PreferenceValue::ValueType &type,
								const QVariant &defval,
								const QVariant &data,
								const PreferenceValue::ValueSection &section)
{
	PreferenceValue *val = new PreferenceValue (key, help, type, defval, data, section);
	m_values[key] = val;
	m_sections[section] = val;

	QSettings s;
	if (!s.contains (key)) {
		qDebug ("creating config value %s", qPrintable (key));
		s.setValue (key, defval);
	}
}

PreferenceManager::PreferenceManager () : QObject (NULL)
{
	/* Right now we set all variables here. In the futurue we will
	 * make sure that each component has their own way to set them
	 */


	registerVal ("core/autostart", tr ("Autostart xmms2d if it's not running"), PreferenceValue::Bool, true, QVariant (), PreferenceValue::Core);
	registerVal ("core/ignoredesktopsettings", tr ("Ignore the desktop settings (use this if everything is black)"),
			PreferenceValue::Bool, false, QVariant (), PreferenceValue::Core);
	registerVal ("core/skiplocales", tr ("Ignore locale settings and use default language"), PreferenceValue::Bool, false, QVariant (), PreferenceValue::Core);
	registerVal ("serverdialog/show", tr ("Show the server browser on startup"), PreferenceValue::Bool, true, QVariant (), PreferenceValue::Core);
	registerVal ("playlist/jumptocurrent", tr ("Jump to the current entry in the playlist on song change"),
			PreferenceValue::Bool, true, QVariant (), PreferenceValue::Feel);
	registerVal ("playlist/compactmode", tr ("Use compact playlist mode (no context area)"), PreferenceValue::Bool, false, QVariant (), PreferenceValue::Look);
	registerVal ("playlist/albumartplace", tr ("Show album art under artist"), PreferenceValue::Bool, true, QVariant (), PreferenceValue::Look);
	registerVal ("ui/showstop", tr ("Show a stop button"), PreferenceValue::Bool, false, QVariant (), PreferenceValue::Look);
	registerVal ("ui/reverseplaytime", tr ("Show time remaining instead of elapsed"), PreferenceValue::Bool, true, QVariant (), PreferenceValue::Look);
	registerVal ("core/pixmapcache", tr ("Size of album art cache in kB"), PreferenceValue::Num, 12040, QVariant (), PreferenceValue::Core);

	QMap <QString, QVariant> contextvalues;
	contextvalues[tr("Album")] = "album";
	contextvalues[tr("Timesplayed")] = "timesplayed";
	contextvalues[tr("Duration")] = "duration";
	contextvalues[tr("Artist")] = "artist";
	contextvalues[tr("Title")] = "title";
	contextvalues[tr("Genre")] = "genre";
	contextvalues[tr("Tracknumber")] = "tracknr";
	contextvalues[tr("Filename")] = "filename";
	registerVal ("ui/contextvalues", tr ("Information to be shown in the context area"),
			PreferenceValue::MultiSelection, "album,timesplayed,duration", QVariant (contextvalues), PreferenceValue::Look);

	registerVal ("ui/contextareabright", tr ("Draw the context area in a lighter color"), PreferenceValue::Bool, true, QVariant (), PreferenceValue::Look);
	registerVal ("ui/titlelighter", tr ("Paint the progress bar in a lighter color"), PreferenceValue::Bool, false, QVariant (), PreferenceValue::Look);
	registerVal ("ui/volumepopup", tr ("Show the volume slider in a popup"), PreferenceValue::Bool, false, QVariant (), PreferenceValue::Feel);
	registerVal ("ui/volumeinteractive", tr ("Change volume interactively (could cause problems)"), PreferenceValue::Bool, false, QVariant (), PreferenceValue::Feel);
	registerVal ("ui/alwaysontop", tr ("Show the playlist (and minimode window) always on top."), PreferenceValue::Bool, true, QVariant (), PreferenceValue::Feel);
	registerVal ("lastfm/showoink", tr ("Show OiNK search in Last.FM view"), PreferenceValue::Bool, false, QVariant (), PreferenceValue::Feel);
	registerVal ("medialib/completion", tr ("Load artists and albums for completion in Medialib window"),
			PreferenceValue::Bool, true, QVariant (), PreferenceValue::Core);
	if (QSystemTrayIcon::isSystemTrayAvailable ()) {
		registerVal ("core/systray", tr ("Show icon in system tray"), PreferenceValue::Bool, true, QVariant (), PreferenceValue::Core);
		registerVal ("core/donotification", tr ("Show popup notification on song change"), PreferenceValue::Bool, true, QVariant (), PreferenceValue::Feel);
		registerVal ("ui/hideOnClose", tr ("Do not quit esperanza on close, rather then hide it (needs System tray active)."),
				PreferenceValue::Bool, true, QVariant (), PreferenceValue::Feel);
		registerVal ("ui/toolwindow", tr ("Show the playerwindow as a toolwindow (no windowlist entry)."),
				PreferenceValue::Bool, false, QVariant (), PreferenceValue::Feel);
		QMap <QString, QVariant> map;
		map[tr("Single Click")] = QSystemTrayIcon::Trigger;
		map[tr("Double Click")] = QSystemTrayIcon::DoubleClick;
		registerVal ("ui/activateTray", tr ("Show mainwindow on single or double click on the tray icon."),
				PreferenceValue::Selection, QSystemTrayIcon::Trigger, map, PreferenceValue::Feel);
	}
	registerVal ("globalshortcuts/play", tr ("Global shortcut to play / pause the playback."),
				PreferenceValue::Key, "Ctrl+Shift+C", QVariant (), PreferenceValue::Shortcuts);
}

