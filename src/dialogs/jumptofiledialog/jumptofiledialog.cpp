/** 
 *  This file is a part of Esperanza, an XMMS2 Client.
 *
 *  Copyright (C) 2005-2007 XMMS2 Team
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version 2
 *  of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 */


#include "jumptofiledialog.h"

#include <QDialog>
#include <QTreeView>
#include <QGridLayout>
#include <QSortFilterProxyModel>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QModelIndex>
#include <QKeyEvent>

#include "playlistmodel.h"

JumpToFileFilterModel::JumpToFileFilterModel (QObject *parent) : QSortFilterProxyModel (parent)
{
}

bool
JumpToFileFilterModel::filterAcceptsRow (int row, const QModelIndex &idx) const
{
	QModelIndex index0 = sourceModel ()->index (row, 0, idx);
	QModelIndex index1 = sourceModel ()->index (row, 1, idx);

	QString s;
	s.append (index0.data ().toString ());
	s.append (index1.data ().toString ());

	return s.contains (filterRegExp ());
}

JumpToFileDialog::JumpToFileDialog (QWidget *parent, PlaylistModel *model) :
	QDialog (parent)
{
	setWindowFlags (Qt::Sheet);
	setWindowTitle ("Esperanza - Jump to file");

	m_model = new JumpToFileFilterModel (this);
	m_model->setSourceModel (model);
	m_model->setFilterCaseSensitivity (Qt::CaseInsensitive);
	m_model->setDynamicSortFilter (true);

	QGridLayout *grid = new QGridLayout (this);
	setLayout (grid);

	m_search = new QLineEdit (this);
	connect (m_search, SIGNAL (textEdited (const QString &)),
			 this, SLOT (set_filter (const QString &)));
	grid->addWidget (m_search, 0, 0);
	grid->setColumnStretch (0, 1);

	setFocusProxy (m_search);

	m_view = new JumpToFileView (this);
	grid->addWidget (m_view, 1, 0);
	grid->setRowStretch (1, 1);

	m_view->setModel (m_model);
	m_view->setIndentation (0);
	m_view->setAlternatingRowColors (true);
	m_view->setItemsExpandable (false);
	m_view->setRootIsDecorated (false);
	m_view->setTabKeyNavigation (false);
	m_view->setTextElideMode (Qt::ElideNone);
    m_view->setSelectionMode (QAbstractItemView::SingleSelection);
    m_view->setSelectionBehavior (QAbstractItemView::SelectRows);

    m_selections = new QItemSelectionModel (m_model);
	m_view->setSelectionModel (m_selections);
	m_view->setCurrentIndex (m_model->index (0, 0));
	
	QDialogButtonBox *bbox = new QDialogButtonBox (Qt::Horizontal, this);
	bbox->addButton (QDialogButtonBox::Ok)->setDefault (true);
	bbox->addButton (QDialogButtonBox::Close);

	connect (bbox, SIGNAL (accepted ()), this, SLOT (accept ()));
	connect (bbox, SIGNAL (rejected ()), this, SLOT (reject ()));
	
	grid->addWidget (bbox, 2, 0);

	resize (400, 450);
}

void
JumpToFileDialog::keyPressEvent (QKeyEvent *ev)
{
	switch (ev->key ()) {
		case Qt::Key_Up:
			m_view->setFocus (Qt::OtherFocusReason);
			m_view->keyPressEvent (ev);
			break;
		case Qt::Key_Down:
			m_view->setFocus (Qt::OtherFocusReason);
			m_view->keyPressEvent (ev);
			break;
		default:
			QDialog::keyPressEvent (ev);
			break;
	}
}

void
JumpToFileDialog::set_filter (const QString &f)
{
	QString s = f;
	s.replace (" ", "*");
	m_model->setFilterWildcard (s);
}

QModelIndex
JumpToFileDialog::sel_item ()
{
	return m_model->mapToSource (m_selections->currentIndex ());
}

