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


#ifndef __XCLIENT_H__
#define __XCLIENT_H__

#include <xmmsclient/xmmsclient++.h>

class XClient;

#include <QObject>
#include <QHash>
#include <QVariant>

#include "xclientcache.h"

class XSettings : public QObject
{
	Q_OBJECT
	public:
		XSettings (QObject *);
		void change_settings ();

	signals:
		void settingsChanged ();
};

class XClient : public QObject, public Xmms::Client
{
	Q_OBJECT
	public:
		XClient (QObject *, const std::string &);

		static bool log ();
		static bool dummy_uint (const uint32_t &);
		bool connect (const char *path = NULL);
		static void propDictToQHash (const std::string &key,
									 const Xmms::Dict::Variant &value,
									 const std::string &source,
									 QHash<QString, QVariant> &hash);

		static QHash<QString, QVariant> convert_propdict (const Xmms::PropDict &);

		XClientCache *cache () const {
			return m_cache;
		};

		XSettings *settings () const {
			return m_settings;
		};

		static QString stdToQ (const std::string &);
		static std::string qToStd (const QString &);

	signals:
		void gotConnection (XClient *);

	private:
		XClientCache *m_cache;
		XSettings *m_settings;

};


#endif
