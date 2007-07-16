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


#include "streamingdialog.h"

#include "playerbutton.h"
#include "icecasthandler.h"

#include <QTabWidget>
#include <QGridLayout>
#include <QResizeEvent>
#include <QInputDialog>
#include <QDateTime>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QMenu>

StreamingDialog::StreamingDialog (QWidget *parent, XClient *client) : QDialog (parent)
{
	QSettings s;

	m_client = client;

	setWindowFlags (Qt::Dialog);

	QGridLayout *g = new QGridLayout (this);

	m_tab = new QTabWidget (this);
	m_bookmarks = new StreamingBookmarks (this, client);
	m_tab->addTab (m_bookmarks, tr ("My bookmarks"));
#if 0
	m_tab->addTab (new QWidget, tr ("Last.Fm radio"));
#endif
	m_tab->addTab (new StreamingIcecast (this, client), tr ("Icecast directory"));

    m_tab->setCurrentIndex (s.value ("streamingdialog/tab", 0).toInt ());

	connect (m_tab, SIGNAL (currentChanged (int)), this, SLOT (set_current (int)));

	g->addWidget (m_tab, 0, 0, 4, 4);
	g->setRowStretch (0, 1);

	QWidget *dummy = new QWidget (this);
	QHBoxLayout *hbox = new QHBoxLayout (dummy);

	g->addWidget (dummy, 4, 0, 1, 4);

	PlayerButton *add = new PlayerButton (dummy, ":images/plus.png");
	connect (add, SIGNAL (clicked (QMouseEvent *)), this, SLOT (add_pressed (QMouseEvent *)));
	PlayerButton *close = new PlayerButton (dummy, ":images/stop.png");
	connect (close, SIGNAL (clicked (QMouseEvent *)), this, SLOT (close ()));

	hbox->addStretch (1);
	hbox->addWidget (add);
	hbox->addWidget (close);

	resize (s.value ("streamingdialog/size", QSize (500, 350)).toSize ());
}

void
StreamingDialog::set_current (int i)
{
    QSettings s;
    s.setValue ("streamingdialog/tab", i);
}

void
StreamingDialog::add_pressed (QMouseEvent *ev)
{
    if (m_tab->currentIndex () == 0) {
	    bool ok;
	    QString url = QInputDialog::getText (this, tr ("Bookmark this URL"),
										    tr ("URL:"), QLineEdit::Normal,
										    "http://", &ok);
	    if (ok) {
            m_client->playlist ()->addUrl (XClient::qToStd (url), "_esperanza_bookmarks") ();
	    }
	} else if (m_tab->currentIndex () == 2) {
		StreamingIcecast *ice = dynamic_cast<StreamingIcecast*> (m_tab->currentWidget ());
		ice->context_menu (ev->globalPos ());
	}
}

void
StreamingDialog::resizeEvent (QResizeEvent *ev)
{
	QSettings s;
	s.setValue ("streamingdialog/size", ev->size ());
}

/***********************
* Icecast directory below
************************/

StreamingIcecast::StreamingIcecast (QWidget *parent, XClient *client) : QWidget (parent)
{
    m_client = client;

    QGridLayout *g = new QGridLayout (this);

    m_progress = new QProgressBar (this);
    m_progress->setTextVisible (true);
	m_progress->setRange (0, 1);
	m_progress->reset ();
    g->addWidget (m_progress, 5, 0);

    m_model = new QStandardItemModel (this);
    m_proxy = new QSortFilterProxyModel (this);
    m_proxy->setSourceModel (m_model);
    m_proxy->setFilterCaseSensitivity (Qt::CaseInsensitive);
    m_proxy->setFilterKeyColumn (0); /* WTF ? */

    m_tree = new QTreeView (this);
    m_tree->setVerticalScrollBarPolicy (Qt::ScrollBarAlwaysOn);
	m_tree->setHorizontalScrollBarPolicy (Qt::ScrollBarAlwaysOff);
	m_tree->setIndentation (0);
	m_tree->setAlternatingRowColors (true);
	m_tree->setItemsExpandable (false);
	m_tree->setRootIsDecorated (false);
    m_tree->setModel (m_proxy);
    m_tree->header ()->setClickable (true);
	m_tree->header ()->setSortIndicatorShown (true);
	m_tree->setContextMenuPolicy (Qt::CustomContextMenu);
	m_order = Qt::AscendingOrder;

    connect (m_tree->header (), SIGNAL (sectionClicked (int)), this, SLOT (sort (int)));
    connect (m_tree, SIGNAL (doubleClicked (const QModelIndex &)), this, SLOT (dbclicked (const QModelIndex &)));
	connect (m_tree, SIGNAL (customContextMenuRequested (const QPoint &)), this, SLOT (custom_context (const QPoint &)));

    g->addWidget (m_tree, 0, 0, 4, 2);
    g->setRowStretch (0, 1);

    m_refresh = new QPushButton (tr ("Refresh"), this);
    connect (m_refresh, SIGNAL (clicked ()), this, SLOT (refresh ()));
    g->addWidget (m_refresh, 5, 1);
    g->setColumnStretch (0, 1);

    m_le = new QLineEdit (this);
    m_le->setFocus (Qt::OtherFocusReason);
    m_le->setFocusPolicy (Qt::StrongFocus);
    QPushButton *reset = new QPushButton (tr ("Reset"), this);

    connect (reset, SIGNAL (clicked ()), m_le, SLOT (clear ()));
    connect (m_le, SIGNAL (textChanged (const QString &)), this, SLOT (do_filter (const QString &)));

    g->addWidget (m_le, 4, 0, 1, 1);
    g->addWidget (reset, 4, 1);

    connect (&m_http, SIGNAL (requestStarted (int)), this, SLOT (req_start ()));
    connect (&m_http, SIGNAL (requestFinished (int, bool)), this, SLOT (req_finished (int, bool)));
    connect (&m_http, SIGNAL (dataReadProgress (int, int)), this, SLOT (req_progress (int, int)));

    QString fname = XClient::esperanza_dir ().absoluteFilePath ("icecast.xml");
    m_file.setFileName (fname);
    m_http.setHost ("dir.xiph.org");

    parse_xml ();
}

void
StreamingIcecast::custom_context (const QPoint &p)
{
	context_menu (m_tree->viewport ()->mapToGlobal (p));
}

void
StreamingIcecast::context_menu (const QPoint &p)
{
	QMenu m;
	m.addAction (tr ("Add to playlist"), this, SLOT (add_to_pls ()));
	m.addAction (tr ("Add to bookmarks"), this, SLOT (add_to_bookmarks ()));
	m.exec (p);
}

void
StreamingIcecast::add_to_pls ()
{
	dbclicked (m_tree->currentIndex ());
}

void
StreamingIcecast::add_to_bookmarks ()
{
	QModelIndex idx = m_tree->currentIndex ();
    m_client->playlist ()->addUrl (XClient::qToStd (idx.data (Qt::UserRole + 1).toString ()), "_esperanza_bookmarks") ();
}

void
StreamingIcecast::dbclicked (const QModelIndex &idx)
{
    m_client->playlist ()->addUrl (XClient::qToStd (idx.data (Qt::UserRole + 1).toString ())) ();
}

void
StreamingIcecast::do_filter (const QString &text)
{
    m_proxy->setFilterRegExp (text);
}

void
StreamingIcecast::sort (int i)
{
    int current = m_tree->header ()->sortIndicatorSection ();
    Qt::SortOrder norder = (i == current && m_order == Qt::AscendingOrder) ? Qt::DescendingOrder : Qt::AscendingOrder;

    m_model->sort (i, norder);
	m_order = norder;

    m_tree->header ()->setSortIndicator (i, norder);
}

void
StreamingIcecast::add_channels (const QList<IcecastChannel> &list)
{
    m_model->clear ();

    m_model->setColumnCount (3);
    QStringList heads;
    heads.append (tr ("Name"));
    heads.append (tr ("Genre"));
    heads.append (tr ("Bitrate"));
    m_tree->header ()->setStretchLastSection (false);
    m_tree->header ()->setResizeMode (0, QHeaderView::Stretch);
    m_tree->header ()->setResizeMode (1, QHeaderView::Stretch);
    m_model->setHorizontalHeaderLabels (heads);
    m_tree->header ()->setSortIndicator (0, Qt::AscendingOrder);

    for (int i = 0; i < list.size (); i ++) {
        QList<QStandardItem *> l;

        QStandardItem *item = new QStandardItem (list.value (i).name ());
        item->setData (list.value (i).url ());
        item->setEditable (false);
        l.append (item);

        item = new QStandardItem (list.value (i).genre ());
        item->setData (list.value (i).url ());
        item->setEditable (false);
        l.append (item);

        item = new QStandardItem (list.value (i).bitrate ());
        item->setData (list.value (i).url ());
        item->setEditable (false);
        l.append (item);

        m_model->appendRow (l);
    }
    m_le->setFocus (Qt::OtherFocusReason);
}

void
StreamingIcecast::parse_xml ()
{
    if (!m_file.open (QIODevice::ReadOnly)) {
        return;
    }

    QXmlSimpleReader r;
    QXmlInputSource *source = new QXmlInputSource (&m_file);
    IcecastHandler *handler = new IcecastHandler;
    r.setContentHandler (handler);

    r.parse (source);

    add_channels (handler->channel_list ());

    m_file.close ();

    delete source;
    delete handler;
}

void
StreamingIcecast::refresh ()
{
    if (m_file.isOpen ()) {
        qDebug ("file is open!");
        return;
    }

    if (!m_file.open (QIODevice::Truncate | QIODevice::ReadWrite)) {
        qDebug ("couldn't open icecast.xml!");
    }

    m_http.get ("/yp.xml", &m_file);
}

void
StreamingIcecast::req_progress (int value, int max)
{
    m_progress->setMaximum (max);
    m_progress->setValue (value);
}

void
StreamingIcecast::req_start ()
{
    m_refresh->setText (tr ("Downloading"));
    m_refresh->setEnabled (false);
}

void
StreamingIcecast::req_finished (int id, bool ok)
{
    m_progress->reset ();
    m_refresh->setText (tr ("Refresh"));
    m_refresh->setEnabled (true);

    if (m_file.isOpen ()) {
        m_file.close ();
    } else {
        return;
    }

    if (!ok && m_http.error () != QHttp::NoError) {
        qDebug ("failed the download! %d", m_http.error ());
    } else {
        parse_xml ();
    }
}

/***********************
 * Bookmarks tab below
 ***********************/

StreamingBookmarks::StreamingBookmarks (QWidget *parent, XClient *client) : QTreeView (parent)
{
	setIndentation (0);
	setAlternatingRowColors (true);
	setItemsExpandable (false);
	setRootIsDecorated (false);
	setTabKeyNavigation (false);
	setVerticalScrollBarPolicy (Qt::ScrollBarAlwaysOn);
	setHorizontalScrollBarPolicy (Qt::ScrollBarAlwaysOff);
	setTextElideMode (Qt::ElideRight);

    m_client = client;

    connect (this, SIGNAL (doubleClicked (const QModelIndex &)), this, SLOT (dbclicked (const QModelIndex &)));

    m_client->playlist ()->list () (Xmms::bind (&StreamingBookmarks::handle_list, this));

    m_timer = new QTimer ();
    connect (m_timer, SIGNAL (timeout ()), this, SLOT (update_list ()));
}

void
StreamingBookmarks::update_list ()
{
    QList<quint32> l = m_model->get_all_id ();
    for (int i = 0; i < l.size (); i ++) {
        m_client->medialib ()->rehash (l.value (i));
    }

    m_timer->start (1000 * 60);
}

void
StreamingBookmarks::keyPressEvent (QKeyEvent *ev)
{
    uint32_t id = currentIndex ().data (PlaylistModel::MedialibIdRole).toUInt ();
    uint32_t pos = currentIndex ().row ();
	switch (ev->key ()) {
		case Qt::Key_Backspace:
		case Qt::Key_Delete:
            m_client->playlist ()->removeEntry (pos, "_esperanza_bookmarks") ();
            break;
        case Qt::Key_Return:
            m_client->playlist ()->addId (id) ();
            break;
        default:
            ev->ignore ();
            break;
    }

}

void
StreamingBookmarks::dbclicked (const QModelIndex &idx)
{
    m_client->playlist ()->addId (idx.data (PlaylistModel::MedialibIdRole).toUInt ());
}

bool
StreamingBookmarks::handle_list (const Xmms::List<std::string> &list)
{
    bool r = false;
    for (list.first (); list.isValid (); ++list) {
        if (*list == "_esperanza_bookmarks") {
            r = true;
            break;
        }
	}

	if (!r) {
        m_client->playlist ()->create ("_esperanza_bookmarks") ();
	}

    m_model = new PlaylistModel (this, m_client, "_esperanza_bookmarks");

    QStringList l;
    l.append ("Channel");
    l.append ("Title");
    m_model->setColumns (l);

    l.clear ();

    l.append ("url");
    l.append ("");
    m_model->setColumnFallback (l);

    setModel (m_model);

    header ()->setResizeMode (0, QHeaderView::Stretch);
    header ()->setResizeMode (1, QHeaderView::Stretch);
    header ()->setStretchLastSection (false);

    update_list ();

    return true;
}
