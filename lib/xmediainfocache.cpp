#include "xmediainfocache.h"

#include <QObject>
#include <QIcon>
#include <QPixmap>
#include <QHash>
#include <QList>
#include <QVariant>
#include <QPixmapCache>

XMediainfoCache::XMediainfoCache (QObject *parent, XClient *client) : QObject (parent)
{
	connect (client, SIGNAL (gotConnection (XClient *)), this, SLOT (got_connection (XClient *))); 
	QPixmapCache::setCacheLimit (4096 * 5);
}

void
XMediainfoCache::got_connection (XClient *client)
{
	m_client = client;
	client->medialib.broadcastEntryChanged (Xmms::bind (&XMediainfoCache::handle_mlib_entry_changed, this));
}

bool
XMediainfoCache::handle_medialib_info (const Xmms::PropDict &info)
{
	int32_t id = info.get<int32_t> ("id");
	QHash<QString, QVariant> hash = XClient::convert_propdict (info);

	m_info.insert (id, hash);
	emit entryChanged (id);

	/*
	if (hash.contains ("picture_front")) {
		QString ha = hash["picture_front"].toString ();
		if (!m_pixmaps.contains (ha)) {
			m_pixmaps[ha] = QPixmap ();
			m_client->bindata.retrieve (ha.toStdString (),
										boost::bind (&XMediainfoCache::handle_bindata, this, _1, ha));
		}
		m_icon_map[ha].append (id);
	}
	*/

	return true;
}

bool
XMediainfoCache::handle_bindata (const Xmms::bin &data, const QString &id)
{
	QPixmap i;
	i.loadFromData (data.c_str (), data.size());

	if (i.isNull ()) {
		return true;
	}

	QPixmapCache::insert (id, i);

	QList<uint32_t> ids = m_icon_map[id];
	for (int i = 0; i < ids.size (); i++) {
		emit entryChanged (ids.at (i));
	}

	return true;
}

QIcon
XMediainfoCache::get_icon (uint32_t id)
{
	return QIcon (get_pixmap (id));
}

QPixmap
XMediainfoCache::get_pixmap (uint32_t id)
{
	if (m_info[id].contains ("picture_front")) {
		QString hash = m_info[id]["picture_front"].toString ();
		QPixmap p;

		if (!QPixmapCache::find (hash, p)) {
			m_client->bindata.retrieve (hash.toStdString (),
										boost::bind (&XMediainfoCache::handle_bindata, this, _1, hash));
			QPixmapCache::insert (hash, QPixmap ());
			m_icon_map[hash].append (id);
		}

		return p;
	}
	return QPixmap ();
}

QHash<QString, QVariant>
XMediainfoCache::get_info (uint32_t id)
{
	if (!m_info.contains (id)) {
		m_client->medialib.getInfo (id, Xmms::bind (&XMediainfoCache::handle_medialib_info, this));
		m_info[id] = QHash<QString, QVariant> ();
	}

	return m_info[id];
}

bool
XMediainfoCache::handle_mlib_entry_changed (const uint32_t &id)
{
	m_client->medialib.getInfo (id, Xmms::bind (&XMediainfoCache::handle_medialib_info, this));
	return true;
}

