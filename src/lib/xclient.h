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
#include <QDir>
#include <QWidget>

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

		bool connect (const char *path = NULL, const bool &sync = false, QWidget* parent = NULL);
		static void propDictToQHash (const std::string &key,
									 const Xmms::Dict::Variant &value,
									 const std::string &source,
									 QHash<QString, QVariant> &hash);
									 
 		static void dictToQHash (const std::string &key,
                                 const Xmms::Dict::Variant &value,
                         		 QHash<QString, QVariant> &hash);

		static QHash<QString, QVariant> convert_propdict (const Xmms::PropDict &);
		static QHash<QString, QVariant> convert_dict (const Xmms::Dict &);

		XClientCache *cache () const {
			return m_cache;
		};

		XSettings *settings () const {
			return m_settings;
		};

		const Xmms::Client *sync () const {
			return &m_sync;
		};

		static QString stdToQ (const std::string &);
		static std::string qToStd (const QString &);
		
		bool isConnected () const {
            return m_isconnected;
        };
        
        static QDir esperanza_dir ();

	signals:
		void gotConnection (XClient *);

	private:
		XClientCache *m_cache;
		XSettings *m_settings;
        bool m_isconnected;

		Xmms::Client m_sync;

};


#endif
