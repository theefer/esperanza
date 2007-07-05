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


#ifndef __JUMPTOFILEDIALOG_H__
#define __JUMPTOFILEDIALOG_H__

#include "playlistmodel.h"

#include <QDialog>
#include <QLineEdit>
#include <QTreeView>
#include <QKeyEvent>
#include <QSortFilterProxyModel>

class JumpToFileView;

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

		QModelIndex sel_item ();

		void keyPressEvent (QKeyEvent *);

	private slots:
		void set_filter (const QString &);
	
	private:
		QSortFilterProxyModel *m_model;
		QLineEdit *m_search;
		JumpToFileView *m_view;
		QItemSelectionModel *m_selections;
};

class JumpToFileView : public QTreeView
{
	Q_OBJECT
	public:
		JumpToFileView (JumpToFileDialog *parent) : QTreeView (parent) {
			m_parent = parent;
		};

		void keyPressEvent (QKeyEvent *ev) {
			switch (ev->key ()) {
				case Qt::Key_Up:
				case Qt::Key_Down:
				case Qt::Key_Enter:
				case Qt::Key_Return:
					QTreeView::keyPressEvent (ev);
					break;
				default:
					m_parent->setFocus ();
					m_parent->keyPressEvent (ev);
					break;
			}
		};

	private:
		JumpToFileDialog *m_parent;
};



#endif
