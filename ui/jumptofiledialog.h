#ifndef __JUMPTOFILEDIALOG_H__
#define __JUMPTOFILEDIALOG_H__

#include "playlistmodel.h"

#include <QDialog>
#include <QLineEdit>
#include <QTreeView>
#include <QSortFilterProxyModel>

class JumpToFileFilterModel : public QSortFilterProxyModel
{
	Q_OBJECT
	public:
		JumpToFileFilterModel (QObject *);
		bool filterAcceptsRow (int, const QModelIndex &) const;
};

class JumpToFileDialog : public QDialog
{
	Q_OBJECT
	public:
		JumpToFileDialog (QWidget *, PlaylistModel *);

		QModelIndex first_item ();
	
	private:
		QSortFilterProxyModel *m_model;
		QLineEdit *m_search;
		QTreeView *m_view;
};

#endif
