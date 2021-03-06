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


#ifndef __XCLIENTCACHE_H__
#define __XCLIENTCACHE_H__

class XClientCache;

#include "xclient.h"

#include <QObject>
#include <QIcon>
#include <QPixmap>
#include <QHash>
#include <QList>
#include <QVariant>
#include <QPixmapCache>

class XClientCache : public QObject
{
	Q_OBJECT
	public:
		XClientCache (QObject *, XClient *);

		QHash<QString, QVariant> get_info (uint32_t id);
		QIcon get_icon (uint32_t id);
		QPixmap get_pixmap (uint32_t id);
		QVariant extra_info_get (uint32_t, const QString &);

		void extra_info_set (uint32_t, const QString &, const QVariant &);
		void invalidate (uint32_t);
        void invalidate_all ();

		bool extra_info_has (uint32_t id, const QString &s) {
			if (m_extra_info.contains (id))
				if (m_extra_info[id].contains (s))
					return true;
			return false;
		};

	signals:
		void entryChanged (uint32_t);
        void entryRemoved (uint32_t);
		void playtime (uint32_t);

	public slots:
		void got_connection (XClient *);

	private:
		bool handle_medialib_info (const Xmms::PropDict &info);
        bool handle_medialib_info_error (const std::string &, uint32_t);
        
		bool handle_mlib_entry_changed (const uint32_t &id);
		bool handle_bindata (const Xmms::bin &, const QString &);

		bool handle_playtime (const unsigned int &tme);

		QHash< uint32_t, QHash<QString, QVariant> > m_info;

		QHash < QString, QList <uint32_t> > m_icon_map;
		QHash < int, QHash < QString, QVariant > > m_extra_info;

		XClient *m_client;
};
	
#endif
