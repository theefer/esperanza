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



#include "medialibdialog.h"
#include "collectionmodel.h"

#include "playerbutton.h"
#include "progressindicator.h"
#include "medialibsearchmodel.h"

#include <QLabel>
#include <QSettings>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QSettings>
#include <QResizeEvent>
#include <QCheckBox>
#include <QCompleter>

MedialibDialog::MedialibDialog (QWidget *parent, XClient *client) : QDialog (parent)
{
	QSettings s;

	setAttribute (Qt::WA_DeleteOnClose);
	setWindowTitle (tr ("Esperanza - Medialib Search"));

	QGridLayout *g = new QGridLayout (this);

	m_client = client;
	m_completer = NULL;

	m_qb = new QComboBox (this);
	m_qb->addItem (tr ("All"), MedialibSearchModel::ALL);
	m_qb->addItem (tr ("Artist"), MedialibSearchModel::ARTIST);
	m_qb->addItem (tr ("Album"), MedialibSearchModel::ALBUM);
	m_qb->addItem (tr ("Title"), MedialibSearchModel::TITLE);
	m_qb->addItem (tr ("Year"), MedialibSearchModel::YEAR);


	g->addWidget (m_qb, 0, 0, 1, 1);

	QLabel *l = new QLabel (tr ("="), this);
	g->addWidget (l, 0, 1, 1, 1);

	m_le = new QLineEdit (this);
	m_le->setFocus (Qt::OtherFocusReason);
	m_le->setFocusPolicy (Qt::StrongFocus);
	connect (m_le, SIGNAL (returnPressed ()), this, SLOT (do_search ()));

	g->addWidget (m_le, 0, 2, 1, 1);
	g->setColumnStretch (2, 1);

	m_indicator = new ProgressIndicator (this);
	g->addWidget (m_indicator, 0, 3, 1, 1);

    PlayerButton *expand = new PlayerButton (this, ":images/minmax.png");
    g->addWidget (expand, 0, 4, 1, 1);
    connect (expand, SIGNAL (clicked (QMouseEvent *)),
             this, SLOT (expand_clicked ()));

    m_browser = new MedialibPaneBrowser (this, m_client);
    if (!s.value ("medialib/paneopen", false).toBool ()) {
        m_browser->hide ();
    } else {
        m_le->setEnabled (false);
        m_qb->setEnabled (false);
    }
    g->addWidget (m_browser, 1, 0, 1, 5);

	m_list = new MedialibView (this, client);
	g->addWidget (m_list, 2, 0, 1, 5);

	QWidget *dummy = new QWidget (this);
	QHBoxLayout *hbox = new QHBoxLayout (dummy);

	g->setRowStretch (2, 1);

	m_cb = new QCheckBox (tr ("Display unavailable entries"), dummy);
	hbox->addWidget (m_cb);
	connect (m_cb, SIGNAL (stateChanged (int)), this, SLOT (do_search ()));

	hbox->addStretch (1);

	PlayerButton *plus = new PlayerButton (dummy, ":images/plus.png");
	connect (plus, SIGNAL (clicked (QMouseEvent *)),
			 this, SLOT (plus_pressed (QMouseEvent *)));

	PlayerButton *stop = new PlayerButton (dummy, ":images/stop.png");
	connect (stop, SIGNAL (clicked (QMouseEvent *)),
			 this, SLOT (close ()));


	hbox->addWidget (plus);
	hbox->addWidget (stop);
	g->addWidget (dummy, 3, 0, 1, 5);
	g->setMargin (1);

	resize (s.value ("medialibdialog/size", QSize (500, 350)).toSize ());

	connect (m_list, SIGNAL (searchDone ()), this, SLOT (search_done ()));

	m_qb->setCurrentIndex (s.value ("medialib/searchdef", 0).toInt ());

	load_compl_list (m_qb->currentIndex ());
    connect (m_qb, SIGNAL (currentIndexChanged (int)), this, SLOT (load_compl_list (int)));
}

void
MedialibDialog::expand_clicked ()
{
    QSettings s;

    if (m_browser->isVisible ()) {
        m_browser->hide ();
        m_le->setEnabled (true);
        m_qb->setEnabled (true);
        load_compl_list (m_qb->currentIndex ());
        s.setValue ("medialib/paneopen", QVariant (false));
    } else {
        m_browser->show ();
        m_le->setEnabled (false);
        m_qb->setEnabled (false);
        s.setValue ("medialib/paneopen", QVariant (true));
    }
}

void
MedialibDialog::load_compl_list (int i)
{
    QSettings s;

    if (s.value ("medialib/paneopen").toBool ()) {
        return;
    }

    if (!s.value ("medialib/completion").toBool ()) {
        return;
    }

	int t = m_qb->itemData (i).toInt ();
	Xmms::Coll::Universe univ;
	std::list <std::string> arg;
	std::string str;

	s.setValue ("medialib/searchdef", m_qb->currentIndex ());

	if (t == MedialibSearchModel::ALL ||
		t == MedialibSearchModel::TITLE) {
		    if (m_completer) {
                delete m_completer;
            }
            m_completer = new QCompleter (this);
            m_le->setCompleter (m_completer);
            return;
	}

	if (t == MedialibSearchModel::ARTIST) {
		str = std::string ("artist");
	} else if (t == MedialibSearchModel::ALBUM) {
		str = std::string ("album");
	} else if (t == MedialibSearchModel::YEAR) {
		str = std::string ("year");
	}

	m_currentsel = str;

	arg.push_front (str);

	m_client->collection ()->queryInfos (univ, arg, arg, 0, 0, arg) (Xmms::bind (&MedialibDialog::compl_reply, this));

	m_le->setEnabled (false);
	m_cb->setEnabled (false);
	m_indicator->setStatus (true);
}

bool
MedialibDialog::compl_reply (const Xmms::List <Xmms::Dict> &list)
{
	QStringList compl_list;

	for (list.first (); list.isValid (); ++ list) {
		/* This seems to happen if a album is "" */
		if ((*list).contains (m_currentsel)) {
			QString qs = XClient::stdToQ ((*list).get<std::string> (m_currentsel));
			compl_list.append (qs);
		}
	}

	if (m_completer) {
		delete m_completer;
	}

	m_completer = new QCompleter (compl_list, this);
	m_completer->setCaseSensitivity (Qt::CaseInsensitive);
	m_le->setCompleter (m_completer);

	m_le->setEnabled (true);
	m_indicator->setStatus (false);
	m_le->setFocus (Qt::OtherFocusReason);
	m_cb->setEnabled (true);

	return true;
}

void
MedialibDialog::keyPressEvent (QKeyEvent *ev)
{
	if (ev->key () == Qt::Key_Escape)
		close ();
}

void
MedialibDialog::plus_pressed (QMouseEvent *ev)
{
	QList<uint32_t> ids = m_list->get_selection ();

	if (ids.size () == 0) {
		ids = m_list->get_all ();
	}

	for (int i = 0; i < ids.size (); i++) {
		m_client->playlist ()->addId (ids.at (i)) ();
	}
}

void
MedialibDialog::search_done ()
{
    if (!m_browser->isVisible ()) {
	    m_le->setEnabled (true);
	    m_indicator->setStatus (false);
	    m_le->setFocus (Qt::OtherFocusReason);
	    m_cb->setEnabled (true);
    }
}

void
MedialibDialog::do_search ()
{
	m_list->search (m_qb->itemData (m_qb->currentIndex ()).toUInt (),
				    m_le->displayText (), m_cb->checkState () == Qt::Checked);
	m_le->setEnabled (false);
	m_cb->setEnabled (false);
	m_indicator->setStatus (true);
}

void
MedialibDialog::resizeEvent (QResizeEvent *ev)
{
	QSettings s;
	s.setValue ("medialibdialog/size", ev->size ());
}

