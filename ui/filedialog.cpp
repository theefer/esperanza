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


#include "filedialog.h"

#include <QSettings>
#include <QFileDialog>
#include <QString>
#include <QFileInfo>

/**
 * TODO: this code might not be very smart, because it parses the
 * return strings and takes the last directory from the filenames.
 */


FileDialog::FileDialog (QWidget *parent, const QString &name, const bool &remote) : QFileDialog (parent)
{
	QSettings s;
	QString dir;
	m_name = name;

	if (s.contains ("filedialog/" + name)) {
		setDirectory (s.value ("filedialog/" + name).toString ());
		return;
	}

	if (remote) {
		dir = "/_xmms2/";
	} else {
		dir = QDir::homePath ();
	}

	s.setValue ("filedialog/" + name, dir);
	setDirectory (dir);
}

QStringList
FileDialog::getFiles ()
{
	QSettings s;
	QStringList ret;

	ret = getOpenFileNames (NULL, tr ("Select music files"),
							directory ().absolutePath ());
	if (ret.size () > 0) { 
        QString d = QFileInfo (ret[0]).absolutePath ();
        s.setValue ("filedialog/" + m_name, d);
    }

	qSort (ret);
	return ret;
}

QString
FileDialog::getDirectory ()
{
	QSettings s;
    QString ret;
    
	ret = getExistingDirectory (NULL, tr ("Select music directory"),
							     directory ().absolutePath ());
    QString d = QFileInfo (ret).absolutePath ();
    s.setValue ("filedialog/" + m_name, d);
	return ret;
}
