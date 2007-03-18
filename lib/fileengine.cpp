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


#include "fileengine.h"

#include <QAbstractFileEngine>

RemoteFileEngine::RemoteFileEngine (const QString &file,
									XClient *client,
									const RemoteFileHandler *handler,
									const RemoteFileItem &item) : QAbstractFileEngine ()
{
	qDebug ("in: %s", qPrintable (file));
	if (file.length () < 7) {
		m_file = '/';
	} else {
		m_file = file.mid (6);
	}

	if (!m_file.startsWith ("/")) {
		m_file = "/" + m_file;
	}

	m_client = client;
	m_item = item;
	m_handler = const_cast<RemoteFileHandler *> (handler);
}

const std::string
RemoteFileEngine::get_file () const
{
	std::string str ("file://");
	str += XClient::qToStd (m_file);
	return str;
}

bool
RemoteFileEngine::caseSensitive () const
{
	return true;
}


QStringList
RemoteFileEngine::entryList (QDir::Filters filters, const QStringList &filterNames) const
{
	QStringList ret;
	QHash<QString, RemoteFileItem> h;

	Xmms::List< Xmms::Dict > l = m_client->sync ()->xform.browse (get_file ());

	for (l.first (); l.isValid (); ++l) {
		Xmms::Dict d = *l;
		if (d.contains ("path")) {
			QString p = XClient::stdToQ (d.get<std::string> ("path"));
			QString k = p;

			int slash = p.lastIndexOf (QLatin1Char ('/'));
			if (slash != -1) {
				p = p.mid (slash + 1);
			}
			ret.append (p);

			bool isdir = d.get<int32_t> ("isdir");
			k.replace ("file:///", "/_xmms2/");
			h.insert (k, RemoteFileItem (k, isdir));
		}
	}

	m_handler->set_filemap (h);

	return ret;
}

QAbstractFileEngine::FileFlags
RemoteFileEngine::fileFlags (FileFlags type) const
{
	/* give them standard rights always */
	FileFlags f = 0;

	f |= ReadOwnerPerm | ReadUserPerm | ReadGroupPerm | ExistsFlag | ExeOwnerPerm | ExeUserPerm | ExeGroupPerm;

	if (m_file == "/") {
		f |= RootFlag;
	}

	if (m_item.isdir ()) {
		f |= DirectoryType;
	} else {
		f |= FileType;
	}

	return f;
}

QString
RemoteFileEngine::fileName (QAbstractFileEngine::FileName file) const
{
	QString fn;

	if (file == QAbstractFileEngine::BaseName) {
		int slash = m_file.lastIndexOf (QLatin1Char ('/'));
		if (slash != -1) {
			return m_file.mid (slash + 1);
		}
	} else if (file == PathName || file == AbsolutePathName || file == CanonicalPathName) {
		int slash = m_file.lastIndexOf (QLatin1Char ('/'));
		if (slash != -1) {
			 fn = m_file.left (slash);
		} else {
			fn = "/";
		}
	} else if (file == AbsoluteName) {
		fn = m_file;
	} else if (file == LinkName) {
		return QString ();
	} else {
		fn = m_file;
	}

	qDebug ("->/_xmms2/%s", qPrintable (fn));

	return QString::fromAscii ("/_xmms2") + fn;
}

QDateTime
RemoteFileEngine::fileTime (FileTime time) const
{
	return QDateTime (QDate (1970, 0, 1));
}

bool
RemoteFileEngine::isRelativePath () const
{
	return false;
}

QString
RemoteFileEngine::owner (FileOwner owner) const
{
	return "xmms2d";
}

uint
RemoteFileEngine::ownerId (FileOwner owner) const
{
	return 42;
}

/*
qint64
RemoteFileEngine::pos () const
{
	return 0;
}

qint64
RemoteFileEngine::read (char * data, qint64 maxlen)
{
	return -1;
}

qint64
RemoteFileEngine::readLine (char * data, qint64 maxlen)
{
	return -1;
}

bool
RemoteFileEngine::remove ()
{
	return false;
}

bool
RemoteFileEngine::rename (const QString &newName)
{
	return false;
}

bool
RemoteFileEngine::rmdir (const QString &dirName, bool recurseParentDirectories) const
{
	return false;
}

bool
RemoteFileEngine::seek (qint64 offset)
{
	return false;
}

void
RemoteFileEngine::setFileName (const QString &file)
{
	qDebug ("got filename %s", qPrintable (file));
	m_file = file;
}

bool
RemoteFileEngine::setPermissions (uint perms)
{
	return false;
}

bool
RemoteFileEngine::setSize (qint64 size)
{
	return false;
}

qint64
RemoteFileEngine::size () const
{
	return 0;
}

qint64
RemoteFileEngine::write (const char * data, qint64 len) 
{
	return -1;
}

bool
RemoteFileEngine::close ()
{
	return true;
}

bool
RemoteFileEngine::copy (const QString &newName)
{
	return false;
}

bool
RemoteFileEngine::isSequential () const
{
	qDebug ("is seq?");
	return true;
}

bool
RemoteFileEngine::link (const QString & newName)
{
	return false;
}

bool
RemoteFileEngine::mkdir (const QString &dirName, bool createParentDirectories) const
{
	return false;
}

bool
RemoteFileEngine::open (QIODevice::OpenMode mode)
{
	qDebug ("trying open");
	return false;
}
*/
