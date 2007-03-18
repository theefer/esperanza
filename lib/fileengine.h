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

#ifndef __FILEENGINE_H__
#define __FILEENGINE_H__

#include "xclient.h"

#include <QAbstractFileEngine>
#include <QFile>
#include <QDir>
#include <QDateTime>
#include <QDate>

class RemoteFileItem
{
	public:
		RemoteFileItem () {
		};

		RemoteFileItem (const QString &path, const bool isdir=false) {
			m_path = path;
			m_isdir = isdir;
		};

		bool isdir () const {
			return m_isdir;
		};

		QString path () const {
			return m_path;
		};

	private:
		QString m_path;
		bool m_isdir;
};

#include "filehandler.h"

class RemoteFileEngine : public QAbstractFileEngine
{
	public:
		RemoteFileEngine (const QString &path, XClient *, const RemoteFileHandler *,
						  const RemoteFileItem &item = RemoteFileItem ());

		bool caseSensitive () const;
		QStringList entryList (QDir::Filters filters, const QStringList & filterNames) const;
		QFile::FileError error () const;
		QString errorString () const;
		FileFlags fileFlags (FileFlags type = FileInfoAll) const;
		QString fileName (FileName file = DefaultName) const;
		QDateTime fileTime (FileTime time) const;
		bool isRelativePath () const;
		QString owner (FileOwner owner) const;
		uint ownerId (FileOwner owner) const;

		/*
		qint64 pos () const;
		qint64 read (char * data, qint64 maxlen);
		qint64 readLine (char * data, qint64 maxlen);
		bool remove ();
		bool rename (const QString & newName);
		bool rmdir (const QString & dirName, bool recurseParentDirectories) const;
		bool seek (qint64 offset);
		void setFileName (const QString & file);
		bool setPermissions (uint perms);
		bool setSize (qint64 size);
		qint64 size () const;
		qint64 write (const char * data, qint64 len);
		bool close ();
		bool copy (const QString & newName);
		bool isSequential () const;
		bool link (const QString & newName);
		bool mkdir (const QString & dirName, bool createParentDirectories) const;
		bool open (QIODevice::OpenMode mode);
		*/

	private:
		QString m_file;
		XClient *m_client;
		RemoteFileItem m_item;

		const std::string get_file () const;
		RemoteFileHandler *m_handler;
};

#endif
