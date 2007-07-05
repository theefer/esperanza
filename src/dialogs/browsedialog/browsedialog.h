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


#ifndef __BROWSEDIALOG_H__
#define __BROWSEDIALOG_H__

#include "xclient.h"
#include "browsemodel.h"

#include <QDialog>
#include <QTreeView>
#include <QListView>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QItemSelectionModel>

class BrowseDialog : public QDialog
{
	Q_OBJECT
	public:
		BrowseDialog (QWidget *parent, XClient *);

	public slots:
		void setPath (const QModelIndex &index);
		void setPath (const QString &path);
		void navigateToPrevious ();
		void dirChanged (const QString &path);
		void accept ();
		QStringList getFiles ();

	private:
		QTreeView *m_list;
		QComboBox *m_lookInCombo;
		QLineEdit *m_lookInEdit;
		QLineEdit *m_fileNameEdit;
		QComboBox *m_fileTypeCombo;
		BrowseModel *m_model;
		QItemSelectionModel *m_selections;

		XClient *m_client;
};
#endif 
