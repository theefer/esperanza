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
		/*
		QPalette p (o.palette);
		QColor col = s.value ("ui/currententry").value<QColor> ();
		p.setColor (QPalette::Text, col);
		p.setColor (QPalette::HighlightedText, col);
		o.palette = p;
		*/
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

	m_model = new PlaylistModel (this, m_client);
	setModel (m_model);

	setItemDelegate (new PlaylistDelegate (this, m_model));

	setIndentation (0);
	setAlternatingRowColors (true);
	setItemsExpandable (false);
	setRootIsDecorated (false);
	setTabKeyNavigation (false);

	setDragEnabled (true);
	setAcceptDrops (true);
	setDropIndicatorShown (true);

	QHeaderView *head = header ();
	head->resizeSection (0, 180);
	head->setResizeMode (0, QHeaderView::Interactive);

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

	setIconSize (QSize (75, 75));
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
		default:
			ev->ignore ();
			return;
	}

	if (nidx.isValid ())
		setCurrentIndex (nidx);
}

void
PlaylistView::changed_settings ()
{
	QSettings s;
	if (!s.value ("playlist/compactmode").toBool ()) {
		if (getSelection ().size () > 1)
			return;
		setExpanded (m_selections->currentIndex (), true);
		m_explist.append (m_selections->currentIndex ());
	} else {
		collapse_all ();
	}
}

void
PlaylistView::collapse_all ()
{
	for (int i = 0; i < m_explist.count (); i++) {
		setExpanded (m_explist.at (i), false);
		m_explist.removeAll (m_explist.at (i));
	}
}

void
PlaylistView::got_connection (XClient *client)
{
	m_client = client;
	client->playlist.broadcastCurrentPos (Xmms::bind (&PlaylistView::handle_update_pos, this));
	client->playlist.currentPos (Xmms::bind (&PlaylistView::handle_update_pos, this));
}

bool
PlaylistView::handle_update_pos (const uint32_t &pos)
{
	QSettings s;
	if (s.value ("playlist/jumptocurrent").toBool ())
		setCurrentIndex (m_model->index (pos, 0));
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
	
	if (getSelection ().count () < 1) {
		setCurrentIndex (old);
		return;
	} else if (getSelection ().count () > 1) {
		collapse_all ();
	} else {
		collapse_all ();

		setExpanded (n, true);
		m_explist.append (n);

		QModelIndex c = n.child (0, 0);
		scrollTo (c);
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

QList<uint32_t>
PlaylistView::getSelection ()
{
	QList<uint32_t> ret;
	QModelIndexList lst = m_selections->selectedIndexes ();
	for (int i = 0; i < lst.size (); i++) {
		QModelIndex idx = lst.at (i);
		if (idx.column () != 0)
			continue;

		ret.append (idx.row ());
	}

	return ret;
}

