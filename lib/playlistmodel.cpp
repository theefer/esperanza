#include <xmmsclient/xmmsclient++.h>
#include <QAbstractTableModel>
#include <QHash>
#include <QVariant>
#include <QIcon>

#include "playlistmodel.h"

PlaylistModel::PlaylistModel (QObject *parent, XClient *client) : QAbstractItemModel (parent)
{
	m_columns.append ("#");
	m_columns.append ("Artist");
	m_columns.append ("Title");

	m_colfallback.append ("");
	m_colfallback.append ("");
	m_colfallback.append ("url");

	connect (client, SIGNAL(gotConnection (XClient *)), this, SLOT (got_connection (XClient *))); 
	connect (client->cache (), SIGNAL(entryChanged (uint32_t)), this, SLOT (entry_changed (uint32_t)));
}

void
PlaylistModel::got_connection (XClient *client)
{
	client->playlist.list (Xmms::bind (&PlaylistModel::handle_list, this));
	client->playlist.currentPos (Xmms::bind (&PlaylistModel::handle_update_pos, this));

	client->playlist.broadcastChanged (Xmms::bind (&PlaylistModel::handle_change, this));
	client->playlist.broadcastCurrentPos (Xmms::bind (&PlaylistModel::handle_update_pos, this));

	m_client = client;
}

bool
PlaylistModel::handle_update_pos (const uint32_t &pos)
{
	m_current_pos = pos;
	emit dataChanged(index (pos, 0), index (pos, m_columns.size ()));
	return true;
}

QList<QString>
PlaylistModel::columns () const
{
	return m_columns;
}

void
PlaylistModel::setColumns (const QList<QString> &new_columns)
{
	m_columns = new_columns;
	reset ();
}

void
PlaylistModel::setColumnFallback (const QList<QString> &new_columns)
{
	m_colfallback = new_columns;
	reset ();
}

bool
PlaylistModel::handle_change (const Xmms::Dict &chg)
{
	int32_t change = chg.get<int32_t> ("type");
	int32_t pos = 0, npos = 0;
	uint32_t id = 0;
	try {
		pos = chg.get<int32_t> ("position");
		id = chg.get<uint32_t> ("id");
	} catch (Xmms::no_such_key_error) {
	}
	
	QModelIndex idx = QModelIndex ();

	switch (change) {
		case XMMS_PLAYLIST_CHANGED_ADD:
			beginInsertRows (idx, pos, pos);
			m_plist.append (id);
			endInsertRows ();
			break;
		case XMMS_PLAYLIST_CHANGED_INSERT:
			beginInsertRows (idx, pos, pos);
			m_plist.insert (pos, id);
			endInsertRows ();
			break;
		case XMMS_PLAYLIST_CHANGED_MOVE:
			beginRemoveRows (idx, pos, pos);
			m_plist.removeAt (pos);
			endRemoveRows ();

			npos = chg.get<uint32_t> ("newposition");
			beginInsertRows (idx, npos, npos);
			m_plist.insert (npos, id);
			endInsertRows ();
			break;
		case XMMS_PLAYLIST_CHANGED_REMOVE:
			beginRemoveRows (idx, pos, pos);
			m_plist.removeAt (pos);
			endRemoveRows ();
			break;
		case XMMS_PLAYLIST_CHANGED_SHUFFLE:
		case XMMS_PLAYLIST_CHANGED_SORT:
		case XMMS_PLAYLIST_CHANGED_CLEAR:
			m_client->playlist.list (Xmms::bind (&PlaylistModel::handle_list, this));
			break;
	}

	return true;
}

bool
PlaylistModel::handle_list (const Xmms::List< unsigned int > &list)
{
	m_plist.clear ();

	for (list.first (); list.isValid (); ++list) {
		m_plist.append (*list);
	}

	reset ();
	return true;
}

QList<uint32_t>
PlaylistModel::getPosById (uint32_t id)
{
	QList<uint32_t> ret;
	int32_t pos = m_plist.indexOf (id);

	while (pos != -1) {
		ret.append (pos);
		pos = m_plist.indexOf (id, pos + 1);
	}

	return ret;
}

void
PlaylistModel::entry_changed (uint32_t id)
{
	QList<uint32_t> pos = getPosById (id);
	for (int i = 0; i < pos.size (); i ++) {
		QModelIndex idx1 = index (pos.at (i), 0);
		QModelIndex idx2 = index (pos.at (i), m_columns.size ());
		emit dataChanged(idx1, idx2);
	}
}

int
PlaylistModel::columnCount (const QModelIndex &parent) const
{
	return m_columns.size ();
}

int
PlaylistModel::rowCount (const QModelIndex &parent) const
{
	if (!parent.isValid ()) {
		return m_plist.size ();
	}

	return 1;
}

QModelIndex
PlaylistModel::parent (const QModelIndex &idx) const
{
	if (!idx.isValid ())
		return QModelIndex ();

	if (idx.internalId () == -1) {
		return QModelIndex ();
	}

	return createIndex (idx.internalId (), idx.column (), 0);
}

QModelIndex
PlaylistModel::index (int row, int column,
					  const QModelIndex &parent) const
{
	if (!parent.isValid ())
		return createIndex (row, column, -1);

	return createIndex (row, column, parent.row ());
}

QVariant
PlaylistModel::data (const QModelIndex &index, int role) const
{
	if (!index.isValid ()) {
		return QVariant ();
	}

	if (index.internalId () == -1) {
		return playlist_data (index, role);
	} else {
		return decoration_data (index, role);
	}

	return QVariant ();
}

QVariant
PlaylistModel::decoration_data (const QModelIndex &index, int role) const
{
	unsigned int r = index.parent ().row ();
	unsigned int id = m_plist[r];
	QHash<QString, QVariant> h = m_client->cache ()->get_info (id);

	if (role == Qt::DisplayRole) {
		if (index.column () == m_columns.size () - 1) {
			QString s;

			s.append (QString ("Bitrate: %0kbps\n").arg (h["bitrate"].toUInt () / 1000));
			unsigned int dur = h["duration"].toUInt ();
			QString m;
			m.sprintf ("%02d:%02d", (dur / 60000), (dur/1000)%60);
			s.append (QString ("Duration: %0\n").arg (m));
			s.append (QString ("Times played: %0\n").arg (h["timesplayed"].toUInt ()));
			return QVariant (s);
		}
		
	}
	if (role == Qt::DecorationRole) {
		if (index.column () == 1) {
			QIcon i = m_client->cache ()->get_icon (id);
			if (i.isNull ())
				return QVariant ();
			else
				return i;
		}
	}
	return QVariant ();
}

QVariant
PlaylistModel::playlist_data (const QModelIndex &index, int role) const
{
	if (index.row () >= m_plist.size ()) {
		return QVariant ();
	}

	if (role == MedialibIdRole) {
		return QVariant (m_plist[index.row ()]);
	}

	if (role == Qt::DisplayRole || role == Qt::ToolTipRole) {
		QString key = m_columns[index.column ()].toLower ();
		QString fallkey = m_colfallback[index.column ()].toLower ();

		if (key == "#") {
			return QVariant (index.row ());
		} else {
			unsigned int id = m_plist[index.row ()];
			PlaylistModel *fake = const_cast<PlaylistModel*> (this);
			QHash<QString, QVariant> d = fake->m_client->cache ()->get_info (id);

			if (d.contains (key)) {
				return QVariant (d[key]);
			} else if (d.contains (fallkey)) {
				return QVariant (d[fallkey]);
			}

			return QVariant ();
		}
	} else if (role == CurrentEntryRole) {
		int i = m_current_pos;

		if (index.row () == i)
			return QVariant (true);
		return QVariant (false);
	}

	return QVariant ();
}

QVariant
PlaylistModel::headerData (int section, Qt::Orientation orientation, int role) const
{
	if (role != Qt::DisplayRole) {
		return QVariant ();
	}

	if (orientation == Qt::Horizontal) {
		if (section <= m_columns.size ())
			return QVariant (m_columns[section]);
	}

	return QVariant ();
}

Qt::ItemFlags
PlaylistModel::flags (const QModelIndex &idx) const
{
	if (!idx.isValid ())
		return Qt::ItemIsEnabled;

	if (idx.internalId () == -1)
		return Qt::ItemIsEnabled | Qt::ItemIsSelectable;

	return Qt::ItemIsEnabled;
}

QList<uint32_t>
PlaylistModel::get_all_id ()
{
	return m_plist;
}
