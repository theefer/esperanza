#ifndef __XMEDIAINFOCACHE_H__
#define __XMEDIAINFOCACHE_H__

class XMediainfoCache;

#include "xclient.h"

#include <QObject>
#include <QIcon>
#include <QPixmap>
#include <QHash>
#include <QList>
#include <QVariant>
#include <QPixmapCache>

class XMediainfoCache : public QObject
{
	Q_OBJECT
	public:
		XMediainfoCache (QObject *, XClient *);

		QHash<QString, QVariant> get_info (uint32_t id);
		QIcon get_icon (uint32_t id);
		QPixmap get_pixmap (uint32_t id);

	signals:
		void entryChanged (uint32_t);

	public slots:
		void got_connection (XClient *);

	private:
		bool handle_medialib_info (const Xmms::PropDict &info);
		bool handle_mlib_entry_changed (const uint32_t &id);
		bool handle_bindata (const Xmms::bin &, const QString &);

		QHash< uint32_t, QHash<QString, QVariant> > m_info;

		QHash < QString, QList <uint32_t> > m_icon_map;

		XClient *m_client;
};
	
#endif
