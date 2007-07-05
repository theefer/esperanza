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



#ifndef __MEDIALIBDIALOG_H__
#define __MEDIALIBDIALOG_H__

#include "xclient.h"
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QDialog>
#include <QCompleter>

class MedialibDialog;

#include "medialibview.h"
#include "progressindicator.h"
#include "medialibpanebrowser.h"

class MedialibDialog : public QDialog
{
	Q_OBJECT

	public:
		MedialibDialog (QWidget *, XClient *);

		void resizeEvent (QResizeEvent *);
		void keyPressEvent (QKeyEvent *);
        void set_list_coll (const Xmms::Coll::Coll &coll)
        {
            m_list->set_collection (coll);
        };
        
	private slots:
		void do_search ();
		void search_done ();
		void plus_pressed (QMouseEvent *);
		void load_compl_list (int);
        void expand_clicked ();

	private:
		bool compl_reply (const Xmms::List < Xmms::Dict > &);

		QLineEdit *m_le;
		QComboBox *m_qb;
		QCheckBox *m_cb;
		QCompleter *m_completer;

		MedialibView *m_list;

		XClient *m_client;

		ProgressIndicator *m_indicator;
        MedialibPaneBrowser *m_browser;

		std::string m_currentsel;
};

#endif

