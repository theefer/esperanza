#include "medialibview.h"
#include "medialibsearchmodel.h"

MedialibView::MedialibView (QWidget *parent, XClient *client) : QTreeView (parent)
{
	m_model = new MedialibSearchModel (this, client);
	setModel (m_model);

	setIndentation (0);
	setAlternatingRowColors (true);
	setItemsExpandable (false);
	setRootIsDecorated (true);

	QPalette p (palette ());
	p.setColor (QPalette::AlternateBase, QColor (230, 230, 230));
	setPalette (p);

    setSelectionMode (QAbstractItemView::ExtendedSelection);
    setSelectionBehavior (QAbstractItemView::SelectRows);

    m_selections = new QItemSelectionModel (m_model);
	setSelectionModel (m_selections);

	setIconSize (QSize (75, 75));

}

QList<uint32_t>
MedialibView::get_selection ()
{
	QList<uint32_t> ret;

	QModelIndexList lst = m_selections->selectedIndexes ();
	for (int i = 0; i < lst.size (); i++) {
		QModelIndex idx = lst.at (i);
		if (idx.column () != 0)
			continue;

		ret.append (idx.data (MedialibSearchModel::MedialibIdRole).toUInt ());
	}

	return ret;
}

QList<uint32_t>
MedialibView::get_all ()
{
	return m_model->get_all_id ();
}
