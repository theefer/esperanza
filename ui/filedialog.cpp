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


FileDialog::FileDialog (QWidget *parent, const QString &name) : QFileDialog (parent)
{
	QSettings s;
	m_name = name;

	if (!s.contains("filedialog/" + name))
		s.setValue ("filedialog/" + name, QDir::homePath ());

	setDirectory (s.value("filedialog/" + name).toString ());
}

QStringList
FileDialog::getFiles ()
{
	QSettings s;
	QStringList ret = getOpenFileNames (NULL, "Choose files");
	if (ret.size() > 0)
		s.setValue ("filedialog/" + m_name, directory ().absolutePath ());

	qSort (ret);

	return ret;

}

QString
FileDialog::getDirectory ()
{
	QSettings s;
	QString ret = getExistingDirectory (NULL, "Choose directory");
	if (!ret.isNull())
		s.setValue ("filedialog/" + m_name, directory ().absolutePath ());

	return ret;

}
