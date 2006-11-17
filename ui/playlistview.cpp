#include "playlistview.h"
#include "playlistmodel.h"
#include "xclient.h"
#include "playerwidget.h"

#include <QHeaderView>
#include <QPainter>
#include <QSettings>

PlaylistDelegate::PlaylistDelegate (QObject *parent, PlaylistModel *model) : QItemDelegate (parent)
{
	m_model = model;
}

void
PlaylistDelegate::paint (QPainter *painter,
						 const QStyleOptionViewItem &option,
						 const QModelIndex &index) const
{
	QSettings s;

	QStyleOptionViewItem o (option);
	if (index.data (PlaylistModel::CurrentEntryRole).toBool ()) {
		QFont f (o.font);
		f.setBold (true);
		o.font = f;
	}
	if (index.internalId() != -1) {
		o.state |= QStyle::State_Selected;
		if (s.value ("ui/contextareabright").toBool ()) {
			QPalette p (o.palette);
			p.setColor (QPalette::Highlight, p.highlight ().color ().light ());
			o.palette = p;
		}
	} 

	QItemDelegate::paint (painter, o, index);

}

PlaylistView::PlaylistView (QWidget *parent, XClient *client) : QTreeView (parent)
{
	m_client = client;
	m_parent = parent;
	m_removed = false;

	m_model = new PlaylistModel (this, m_client);
	setModel (m_model);

	setItemDelegate (new PlaylistDelegate (this, m_model));

	setIndentation (0);
	setAlternatingRowColors (true);
	setItemsExpandable (false);
	setRootIsDecorated (false);
	setTabKeyNavigation (false);
	setTextElideMode (Qt::ElideNone);

	setVerticalScrollBarPolicy (Qt::ScrollBarAlwaysOn);

	setDragEnabled (true);
	setAcceptDrops (true);
	setDropIndicatorShown (true);
//	setDragDropMode (QAbstractItemView::DragDrop);

	QHeaderView *head = header ();
	QSettings s;

	head->resizeSection (0, s.value ("playlist/section0", 180).toInt ());
	head->setResizeMode (0, QHeaderView::Interactive);
	connect (head, SIGNAL (sectionResized (int, int, int)), this, SLOT (head_size (int, int, int)));

    setSelectionMode (QAbstractItemView::ExtendedSelection);
    setSelectionBehavior (QAbstractItemView::SelectRows);

    m_selections = new QItemSelectionModel (m_model);
	setSelectionModel (m_selections);

	connect (m_selections, SIGNAL (currentRowChanged (const QModelIndex &, const QModelIndex &)),
			 this, SLOT (item_selected (const QModelIndex &, const QModelIndex &)));

	connect (this, SIGNAL (doubleClicked (const QModelIndex &)),
			 this, SLOT (jump_pos (const QModelIndex &)));

	connect (client, SIGNAL(gotConnection (XClient *)),
			 this, SLOT (got_connection (XClient *))); 

	connect (m_client->settings (), SIGNAL (settingsChanged ()),
			 this, SLOT (changed_settings ()));

	connect (m_model, SIGNAL (rowsInserted (const QModelIndex &, int, int)),
			 this, SLOT (rows_inserted ()));

	connect (m_model, SIGNAL (entryMoved (const QModelIndex &, const QModelIndex &)),
			 this, SLOT (moved (const QModelIndex &, const QModelIndex &)));

	setIconSize (QSize (75, 75));
}

void
PlaylistView::mousePressEvent (QMouseEvent *ev)
{
	if (ev->buttons () & Qt::RightButton || ev->buttons () & Qt::MidButton) {
		ev->ignore ();
		return;
	}

	if (!indexAt (ev->pos ()).isValid ()) {
		ev->ignore ();
		return;
	}

	QTreeView::mousePressEvent (ev);
}

void
PlaylistView::rows_inserted ()
{
	QModelIndex idx = m_model->index (0, 0);
	if (!idx.isValid ())
		return;
	if (!m_model->cached_size (idx.column ()).isValid () && idx.internalId () == -1) {
		m_model->set_cached_size (idx.column (), sizeHintForIndex (idx));
	}
	idx = m_model->index (0, 1);
	if (!m_model->cached_size (idx.column ()).isValid () && idx.internalId () == -1) {
		m_model->set_cached_size (idx.column (), sizeHintForIndex (idx));
	}
}

void
PlaylistView::keyPressEvent (QKeyEvent *ev)
{
	QModelIndex idx = m_selections->currentIndex ();
	QModelIndex nidx;

	switch (ev->key ()) {
		case Qt::Key_Up:
			nidx = m_model->index (idx.row () - 1, idx.column (), QModelIndex ());
			break;
		case Qt::Key_Down:
			nidx = m_model->index (idx.row () + 1, idx.column (), QModelIndex ());
			break;
		case Qt::Key_PageDown:
		case Qt::Key_PageUp:
			QTreeView::keyPressEvent (ev);
			return;
		default:
			ev->ignore ();
			return;
	}

	if (nidx.isValid ())
		setCurrentIndex (nidx);
}

void
PlaylistView::moved (const QModelIndex &o, const QModelIndex &n)
{
	if (m_selections->isSelected (o)) {
		/* why the fuc??? */
//		m_selections->select (o, QItemSelectionModel::Deselect | QItemSelectionModel::Rows);
//		m_selections->select (n, QItemSelectionModel::ClearAndSelect  | QItemSelectionModel::Rows);
		setCurrentIndex (n);
	}
}

void
PlaylistView::changed_settings ()
{
	QSettings s;

	collapseAll ();

	if (!s.value ("playlist/compactmode").toBool ()) {
		if (getSelection ().size () > 1)
			return;
		setExpanded (m_selections->currentIndex (), true);
	}
}

void
PlaylistView::got_connection (XClient *client)
{
	m_client = client;
	client->playback.broadcastCurrentID (Xmms::bind (&PlaylistView::handle_update_pos, this));
	client->playback.currentID (Xmms::bind (&PlaylistView::handle_update_pos, this));
}

bool
PlaylistView::handle_update_pos (const uint32_t &id)
{
	QSettings s;
	QModelIndex idx = m_model->current_playlist_pos ();
	if (!idx.isValid ())
		return true;

	if (s.value ("playlist/jumptocurrent").toBool ())
		setCurrentIndex (idx);

	return true;
}

void
PlaylistView::item_selected (const QModelIndex &n, const QModelIndex &old)
{
	QSettings s;

	if (s.value ("playlist/compactmode").toBool ())
		return;

	if (n.internalId () != -1) {
		setCurrentIndex (n.parent ());
		return;
	}

	QModelIndexList l = getSelection ();
	
	if (l.count () < 1) {
		setCurrentIndex (old);
	} else if (l.count () > 1) {
		collapseAll ();
	} else {
		setCurrentIndex (n);

		collapseAll ();

		setExpanded (n, true);

		QModelIndex c = n.child (0, 0);
		scrollTo (c);
	}

	/* emit current id */
	if (l.count () > 1 || l.count () < 1) {
		emit selectedID (0);
	} else {
		emit selectedID (l[0].data (PlaylistModel::MedialibIdRole).toUInt ());
	}
}

static bool
dummy_uint (const uint32_t &)
{
	return false;
}

void
PlaylistView::jump_pos (const QModelIndex &i)
{
	QModelIndex idx = i;

	if (!idx.isValid ())
		idx = currentIndex ();

	uint32_t row = idx.row ();
	if (idx.internalId () != -1)
		row = idx.parent ().row ();

	m_client->playlist.setNext (row, &dummy_uint);
	/* Note. tickle before checking status is a good
	 * idea here. It seems to bork on linux platform
	 * otherwise
	 */
	m_client->playback.tickle (&XClient::log);

	PlayerWidget *pw = dynamic_cast<PlayerWidget *> (m_parent);
	if (pw->status () != Xmms::Playback::PLAYING) {
		m_client->playback.start (&XClient::log);

		if (pw->status () == Xmms::Playback::PAUSED) {
			m_client->playback.tickle (&XClient::log);
		}
	}

}

QModelIndexList
PlaylistView::getSelection ()
{
	QModelIndexList lst = m_selections->selectedIndexes ();
	QModelIndexList ret;

	for (int i = 0; i < lst.size (); i++) {
		QModelIndex idx = lst.at (i);
		if (idx.column () != 0)
			continue;

		ret.append (idx);
	}

	return ret;
}

void
PlaylistView::head_size (int c, int o, int n)
{
	if (c == 0) {
		QSettings s;
		s.setValue ("playlist/section0", n);
	}
}

