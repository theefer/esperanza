#include <xmmsclient/xmmsclient++.h>
#include <QAbstractTableModel>
#include <QHash>
#include <QVariant>
#include <QIcon>
#include <QMimeData>
#include <QSettings>

#include "playlistmodel.h"

PlaylistModel::PlaylistModel (QObject *parent, XClient *client) : QAbstractItemModel (parent)
{
//	m_columns.append ("#");
	m_columns.append ("Artist");
	m_columns.append ("Title");

//	m_colfallback.append ("");
	m_colfallback.append ("");
	m_colfallback.append ("url");

	m_cached_size.append (QSize ());
	m_cached_size.append (QSize ());
	m_cached_size.append (QSize ());

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

	if (chg.contains ("position")) {
		pos = chg.get<int32_t> ("position");
	}
	if (chg.contains ("id")) {
		id = chg.get<uint32_t> ("id");
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
			npos = chg.get<int32_t> ("newposition");

			beginRemoveRows (idx, pos, pos);
			m_plist.removeAt (pos);
			endRemoveRows ();

			beginInsertRows (idx, npos, npos);
			m_plist.insert (npos, id);
			endInsertRows ();

			if (pos < npos && pos)
				pos --;

			emit entryMoved (index (pos, 0), index (npos, 0));

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
	beginRemoveRows (QModelIndex (), 0, m_plist.size ());
	m_plist.clear ();
	endRemoveRows ();

	int i = 0;
	for (list.first (); list.isValid (); ++list) {
		i ++;
	}
	beginInsertRows (QModelIndex (), 0, i);
	for (list.first (); list.isValid (); ++list) {
		m_plist.append (*list);
	}

	endInsertRows ();

	return true;
}

QModelIndexList
PlaylistModel::get_idxlist_by_id (uint32_t id)
{
	QModelIndexList ret;

	QList<uint32_t> l = getPosById (id);
	for (int i = 0; i < l.count (); i++) {
		ret.append (index (l.at (i), 0));
	}
	return ret;
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

		/* and for the infobar under (child) */
		idx1 = index (0, 0, idx1);
		idx2 = index (0, m_columns.size (), idx1.parent ());
		emit dataChanged (idx1, idx2);
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

	return createIndex (idx.internalId (), idx.column (), -1);
}

QModelIndex
PlaylistModel::index (int row, int column,
					  const QModelIndex &parent) const
{
	if (!parent.isValid ()) {
		if (row > (m_plist.size () - 1))
			return QModelIndex ();
		if (row < 0)
			return QModelIndex ();
		return createIndex (row, column, -1);
	}

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

	if (role == Qt::TextAlignmentRole) {
		return QVariant (Qt::AlignVCenter);
	}

	if (role == Qt::DisplayRole) {
		if (index.column () == m_columns.size () - 1) {
			QString str;
			QSettings s;

			QStringList val = s.value ("ui/contextvalues").toString ().split (",");

			str.append ("\n");
			for (int i = 0; i < val.count (); i ++) {
				if (val.at (i) == "bitrate") {
					str.append (QString ("Bitrate: %0kbps\n").arg (h["bitrate"].toUInt () / 1000));
				} else if (val.at (i) == "duration") {
					unsigned int dur = h["duration"].toUInt ();
					QString m;
					m.sprintf ("%02d:%02d", (dur / 60000), (dur/1000)%60);
					str.append (QString ("Duration: %0\n").arg (m));
				} else if (val.at (i) == "timesplayed") {
					str.append (QString ("Times played: %0\n").arg (h["timesplayed"].toUInt ()));
				} else if (val.at (i) == "album") {
					str.append (QString ("Album: %0\n").arg (h["album"].toString ()));
				} else if (val.at (i) == "artist") {
					str.append (QString ("Artist: %0\n").arg (h["artist"].toString ()));
				} else if (val.at (i) == "title") {
					str.append (QString ("Title: %0\n").arg (h["title"].toString ()));
				} else if (val.at (i) == "genre") {
					str.append (QString ("Genre: %0\n").arg (h["genre"].toString ()));
				} else if (val.at (i) == "tracknr") {
					str.append (QString ("Track Number: %0\n").arg (h["tracknr"].toUInt ()));
				}
			}
			return QVariant (str);
		}
		
	}
	if (role == Qt::DecorationRole) {
		QSettings s;
		int c = 0;

		if (!s.value ("playlist/albumartplace").toBool ())
			c = 1;
		
		if (index.column () == c) {
			QIcon i = m_client->cache ()->get_icon (id);
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

	if (role == Qt::SizeHintRole) {
		if (m_cached_size[index.column ()].isValid ()) {
			return QVariant (m_cached_size[index.column ()]);
		}
		return QVariant ();
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

QStringList
PlaylistModel::mimeTypes () const
{
	QStringList l ("application/x-xmms2poslist");
	l << "application/x-xmms2mlibid";
	return l;
}

QMimeData *
PlaylistModel::mimeData (const QModelIndexList &list) const
{
	QMimeData *ret = new QMimeData ();
	QByteArray ba;
	QDataStream stream (&ba, QIODevice::WriteOnly);

	QList<int> l;
	for (int i = 0; i < list.size (); i ++) {
		QModelIndex idx = list.at (i);
		if (idx.column () != 0)
			continue;
		l.append (idx.row ());
	}

	stream << l;
	ret->setData ("application/x-xmms2poslist", ba);

	return ret;
}

bool
PlaylistModel::dropMimeData (const QMimeData *data,
							 Qt::DropAction action,
							 int row, int column,
							 const QModelIndex & parent)
{
	if (parent.internalId () != -1) {
		qDebug ("don't want this!");
		return false;
	}
	if (data->hasFormat ("application/x-xmms2poslist")) {
		QByteArray ba = data->data ("application/x-xmms2poslist");
		QDataStream stream (&ba, QIODevice::ReadOnly);
		QList<int> l;
		stream >> l;
		qSort (l);
		int target = parent.row ();

		int mod = 0;

		while (l.size ()) {
			int orow = l.takeAt (0) - mod;
			m_client->playlist.move (orow, target, &XClient::log);
			if (orow < target) {
				mod ++;
			} else {
				target ++;
			}
		}
		return true;
	} else if (data->hasFormat ("application/x-xmms2mlibid")) {
		QByteArray ba = data->data ("application/x-xmms2mlibid");
		QDataStream stream (&ba, QIODevice::ReadOnly);
		QList<int> l;
		stream >> l;
		int target = parent.row () + 1;
		while (l.size ()) {
			int id = l.takeAt (0);
			if (target >= m_plist.size ()) {
				m_client->playlist.addId (id, &XClient::log);
			} else {
				m_client->playlist.insertId (target ++, id, &XClient::log);
			}
		}
		return true;
	}
	return false;
}

Qt::DropActions
PlaylistModel::supportedDropActions () const
{
	return Qt::CopyAction | Qt::MoveAction;
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

	if (idx.internalId () == -1) {
		unsigned int id = m_plist[idx.row ()];
		PlaylistModel *fake = const_cast<PlaylistModel*> (this);
		QHash<QString, QVariant> d = fake->m_client->cache ()->get_info (id);

		Qt::ItemFlags f = Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;
		if (d["available"].toBool ()) {
			f |= Qt::ItemIsEnabled;
		}

		return f;
	}

	return Qt::ItemIsEnabled;
}

QList<uint32_t>
PlaylistModel::get_all_id ()
{
	return m_plist;
}
