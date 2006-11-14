#include "jumptofiledialog.h"

#include <QDialog>
#include <QTreeView>
#include <QGridLayout>
#include <QSortFilterProxyModel>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QModelIndex>

#include "playlistmodel.h"

JumpToFileFilterModel::JumpToFileFilterModel (QObject *parent) : QSortFilterProxyModel (parent)
{
}

bool
JumpToFileFilterModel::filterAcceptsRow (int row, const QModelIndex &idx) const
{
	QModelIndex index0 = sourceModel ()->index (row, 0, idx);
	QModelIndex index1 = sourceModel ()->index (row, 1, idx);

	return (sourceModel ()->data (index0).toString ().contains (filterRegExp ()) ||
			sourceModel ()->data (index1).toString ().contains (filterRegExp ()));
}

JumpToFileDialog::JumpToFileDialog (QWidget *parent, PlaylistModel *model) :
	QDialog (parent)
{
	setWindowFlags (Qt::Sheet);
	setWindowTitle ("Esperanza - Jump to file");

	m_model = new JumpToFileFilterModel (this);
	m_model->setSourceModel (model);
	m_model->setFilterCaseSensitivity (Qt::CaseInsensitive);

	QGridLayout *grid = new QGridLayout (this);
	setLayout (grid);

	m_search = new QLineEdit (this);
	connect (m_search, SIGNAL (textEdited (const QString &)),
			 m_model, SLOT (setFilterWildcard (const QString &)));
	grid->addWidget (m_search, 0, 0);
	grid->setColumnStretch (0, 1);

	QTreeView *m_view = new QTreeView (this);
	grid->addWidget (m_view, 1, 0);
	grid->setRowStretch (1, 1);

	m_view->setModel (m_model);
	m_view->setIndentation (0);
	m_view->setAlternatingRowColors (true);
	m_view->setItemsExpandable (false);
	m_view->setRootIsDecorated (false);
	m_view->setTabKeyNavigation (false);
	m_view->setTextElideMode (Qt::ElideNone);
	
	QDialogButtonBox *bbox = new QDialogButtonBox (Qt::Horizontal, this);
	bbox->addButton (QDialogButtonBox::Ok)->setDefault (true);
	bbox->addButton (QDialogButtonBox::Close);

	connect (bbox, SIGNAL (accepted ()), this, SLOT (accept ()));
	connect (bbox, SIGNAL (rejected ()), this, SLOT (reject ()));
	
	grid->addWidget (bbox, 2, 0);

	resize (400, 450);
}

QModelIndex
JumpToFileDialog::first_item ()
{
	QModelIndex i = m_model->index (0, 0);
	if (i.isValid ())
		return m_model->mapToSource (i);
	return QModelIndex ();
}

