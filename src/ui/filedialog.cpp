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


#include "filedialog.h"

#include <QSettings>
#include <QFileDialog>
#include <QString>
#include <QFileInfo>

/**
 * TODO: this code might not be very smart, because it parses the
 * return strings and takes the last directory from the filenames.
 */


FileDialog::FileDialog (QWidget *parent, const QString &name, const bool &remote) : QObject (parent)
{
	QSettings s;
	QString dir;
	m_name = name;

	if (s.contains ("filedialog/" + name)) {
		m_last_dir = s.value ("filedialog/" + name).toString ();
		return;
	}

	if (remote) {
		m_last_dir = "/_xmms2/";
	} else {
		m_last_dir = QDir::homePath ();
	}

	s.setValue ("filedialog/" + name, m_last_dir);
}

QStringList
FileDialog::getFiles ()
{
	QSettings s;
	QStringList ret;

	ret = QFileDialog::getOpenFileNames (dynamic_cast<QWidget *>(parent ()), tr ("Select music files"),
							QDir(m_last_dir).absolutePath ());
	if (ret.size () > 0) { 
        m_last_dir = QFileInfo (ret[0]).absolutePath ();
        s.setValue ("filedialog/" + m_name, m_last_dir);
    }

	qSort (ret);
	return ret;
}

QString
FileDialog::getDirectory ()
{
	QSettings s;
    QString ret;
    
	ret = QFileDialog::getExistingDirectory (dynamic_cast<QWidget *>(parent ()), tr ("Select music directory"),
							     QDir(m_last_dir).absolutePath ());
    m_last_dir = QFileInfo (ret).absolutePath ();
    s.setValue ("filedialog/" + m_name, m_last_dir);
	return ret;
}
