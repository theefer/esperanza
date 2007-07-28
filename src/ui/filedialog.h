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


#ifndef __FILEDIALOG_H__
#define __FILEDIALOG_H__

#include <QObject>

class FileDialog : public QObject
{
	Q_OBJECT
	public:
		FileDialog (QWidget *parent, const QString &name, const bool &remote=false);
		QString getDirectory ();
		QStringList getFiles ();

	private:
		QString m_name;
		QString m_last_dir;
};

#endif
