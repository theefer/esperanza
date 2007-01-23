/** 
 *  This file is a part of Esperanza, an XMMS2 Client.
 *
 *  Copyright (C) 2005-2006 XMMS2 Team
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

#include "collections/manager.h"

#include <QWidget>
#include <QDialog>
#include <QGridLayout>
#include <QSplitter>
#include <QListWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QFrame>

#include "collections/collectionlist.h"
#include "collections/collectionview.h"
#include "collections/propertyeditor.h"

#include "playerbutton.h"
#include "playlistview.h"
#include "playlistmodel.h"

CollectionManager::CollectionManager (QWidget *parent, XClient *client) : QDialog (parent)
{
	QGridLayout *grid = new QGridLayout (this);
	QSplitter *split = new QSplitter;
	
	m_client = client;

	setWindowTitle ("Esperanza - Collection Manager");

	grid->addWidget (split, 0, 0);
	grid->setMargin (2);

	QFrame *dummy = new QFrame (this);;
	dummy->setFrameShape (QFrame::StyledPanel);

	QVBoxLayout *vbox = new QVBoxLayout (dummy);
	vbox->setMargin (1);
	m_collist = new CollectionList (dummy, client);
	vbox->addWidget (m_collist, 1);

	QHBoxLayout *hbox = new QHBoxLayout;
	hbox->addStretch (1);
	
	/* button for adding playlists / collections */
	PlayerButton *plus = new PlayerButton (dummy, ":images/plus.png");
	hbox->addWidget (plus);
	connect (plus, SIGNAL (clicked (QMouseEvent *)), this, SLOT (plus_pressed ()));
	
	PlayerButton *minus = new PlayerButton (dummy, ":images/minus.png");
	hbox->addWidget (minus);
	connect (minus, SIGNAL (clicked (QMouseEvent *)), this, SLOT (minus_pressed ()));

	hbox->addWidget (new PlayerButton (dummy, ":images/settings.png"));

	vbox->addLayout (hbox);
	split->addWidget (dummy);

	dummy = new QFrame;
	QGridLayout *g = new QGridLayout (dummy);
	g->setMargin (0);
	dummy->setFrameShape (QFrame::StyledPanel);

	m_propeditor = new PropertyEditor (this, client);
	g->addWidget (m_propeditor, 0, 0);

	m_stacked = new QStackedWidget (this);
	g->addWidget (m_stacked, 1, 0);

	CollectionView *view = new CollectionView (this, client);
	m_stacked->addWidget (view);

	PlaylistView *plsview = new PlaylistView (this, client);
	m_stacked->addWidget (plsview);

	m_plsmodel = new PlaylistModel (this, client);
	m_plsmodel->got_connection (client);
	plsview->setModel (m_plsmodel);

	split->addWidget (dummy);

	connect (m_collist, SIGNAL (switch_view (const Xmms::Collection::Namespace &, const QString &)),
			 this, SLOT (switch_view_proxy (const Xmms::Collection::Namespace &, const QString &)));

	QList<int> l;
	l.append (200);
	l.append (split->size ().width () - 200);
	split->setSizes (l);

	resize (600, 400);
	
}

void
CollectionManager::minus_pressed ()
{
    CollectionListItem *item = dynamic_cast<CollectionListItem *> (m_collist->currentItem ());
    
    m_client->collection.remove (XClient::qToStd (item->text (0)), item->ns ()) ();
}

void
CollectionManager::plus_pressed ()
{
    QTreeWidgetItem *parent = m_collist->currentItem ();
    if (!parent) return; /* not selected */
    /* get parent */
    while (parent->parent ()) parent = parent->parent ();
    
    QTreeWidgetItem *item = new QTreeWidgetItem (parent, QStringList (tr ("New")));
    item->setFlags (Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    m_collist->setCurrentItem (item);
    m_collist->editItem (item, 0);
}

void
CollectionManager::switch_view_proxy (const Xmms::Collection::Namespace &ns,
									  const QString &str)
{
	if (ns == Xmms::Collection::COLLECTIONS) {
		m_stacked->setCurrentIndex (0); /* switch to collections */
		emit switch_view (ns, str);
	} else {
		m_stacked->setCurrentIndex (1); /* switch to playlists */
		m_plsmodel->set_playlist (str);
	}
}

