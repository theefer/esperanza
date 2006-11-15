#include "medialibsearchmodel.h"

MedialibSearchModel::MedialibSearchModel (QObject *parent, XClient *client) :
	PlaylistModel (parent, client)
{
	m_client = client;
	QStringList columns;
	columns.append ("Artist");
	columns.append ("Album");
	columns.append ("Title");
	setColumns (columns);
	columns.append ("url");
	columns.append ("");
	columns.append ("");
	setColumnFallback (columns);

}

void
MedialibSearchModel::do_search (uint32_t p, const QString &s, bool unavailable)
{
	QString a = QString ("%%%0%%").arg (s.toLower ());
	a = QString::fromUtf8 (m_client->medialib.sqlitePrepareString (std::string (a.toUtf8 ())).c_str ());

	QString q ("select distinct m1.id as id from Media m1 left join Media m2 on m1.id = m2.id and m2.key = 'resolved' and m2.value = 1");
	
	if (!unavailable) {
		q.append (" left join Media m3 on m3.id = m1.id and m3.key = 'available' ");
	}

	switch (p) {
		case SEARCH_ALL:
			q.append (QString (" where lower(m1.value) like %0").arg (a));
			break;
		case SEARCH_ALBUM:
			q.append (QString (" where m1.key = 'album' and lower(m1.value) like %0").arg (a));
			break;
		case SEARCH_ARTIST:
			q.append (QString (" where m1.key = 'artist' and lower(m1.value) like %0").arg (a));
			break;
		case SEARCH_TITLE:
			q.append (QString (" where m1.key = 'title' and lower(m1.value) like %0").arg (a));
			break;
		case SEARCH_YEAR:
			q.append (QString (" where m1.key = 'date' and lower(m1.value) like %0").arg (a));
			break;
	}

	if (!unavailable) {
		q.append (" and m3.value = 1");
	}

	q.append (" order by m1.id");


	m_client->medialib.select (std::string (q.toUtf8 ()), Xmms::bind (&MedialibSearchModel::handle_search, this));
}

void
MedialibSearchModel::got_connection (XClient *client)
{
	m_client = client;
}

bool
MedialibSearchModel::handle_search (const Xmms::List< Xmms::Dict > &list)
{
	m_plist.clear ();

	for (list.first (); list.isValid (); ++ list) {
		m_plist.append ((*list).get<int32_t> ("id"));
	}

	emit searchDone ();

	reset ();

	return true;
}
