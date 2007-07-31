/** 
 *  This file is a part of Esperanza, an XMMS2 Client.
 *
 *  Copyright (C) 2007 XMMS2 Team
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


#ifndef __XMMS2DPREFDIALOG_H__
#define __XMMS2DPREFDIALOG_H__

#include "xclient.h"
#include "xmms2dprefdialog.h"
#include <QTreeView>
#include <QMutex>

#include "xmms2dprefmodel.h"
#include "ui_xmms2dprefdialog.h"

class Xmms2dPrefDialog : public QDialog, public Ui::Xmms2dPrefDialog
{
	Q_OBJECT
	public:
		Xmms2dPrefDialog (QWidget *parent, XClient *client_);
		~Xmms2dPrefDialog ();

	public slots:
		void clickedBtnBox(QAbstractButton*);

	private slots:
		void got_connection (XClient *);
		void userChangedPref (QModelIndex, QModelIndex);

	protected:
		void pressedOk ();
		void pressedApply ();
		void pressedCancel ();
		QWidget* createScrollableWidget (QWidget *parent);
		bool handle_valueList (const Xmms::Dict &d);
		bool handle_valueChanged (const Xmms::Dict &d);
		void addPreference( const std::string&, const Xmms::Dict::Variant&);
		void updatePreference( const std::string&, const Xmms::Dict::Variant&);
		QModelIndex findIndex(QString name, QModelIndex parent = QModelIndex ());
		QString getKeyString(QModelIndex value);

	private:
		Xmms2dPrefModel *model;
		XClient *client;
		QList<QModelIndex> changedPrefs;
		QMutex dataChanging;
};
#endif
