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

#include "playerbutton.h"

CollectionManager::CollectionManager (QWidget *parent, XClient *client) : QDialog (parent)
{
	QGridLayout *grid = new QGridLayout (this);
	QSplitter *split = new QSplitter;

	setWindowTitle ("Esperanza - Collection Manager");

	grid->addWidget (split, 0, 0);
	grid->setMargin (2);

	QFrame *dummy = new QFrame (this);;
	dummy->setFrameShape (QFrame::StyledPanel);

	QVBoxLayout *vbox = new QVBoxLayout (dummy);
	vbox->setMargin (1);
	CollectionList *collist = new CollectionList (dummy, Xmms::Collection::COLLECTIONS, client);
	vbox->addWidget (collist, 1);

	QHBoxLayout *hbox = new QHBoxLayout;
	hbox->addStretch (1);
	hbox->addWidget (new PlayerButton (dummy, ":images/plus.png"));
	hbox->addWidget (new PlayerButton (dummy, ":images/minus.png"));
	hbox->addWidget (new PlayerButton (dummy, ":images/settings.png"));

	vbox->addLayout (hbox);

	CollectionList *playlist = new CollectionList (dummy, Xmms::Collection::PLAYLISTS, client);
	vbox->addWidget (playlist, 1);

	hbox = new QHBoxLayout;
	hbox->addStretch (1);
	hbox->addWidget (new PlayerButton (dummy, ":images/plus.png"));
	hbox->addWidget (new PlayerButton (dummy, ":images/minus.png"));
	hbox->addWidget (new PlayerButton (dummy, ":images/settings.png"));
	vbox->addLayout (hbox);

	split->addWidget (dummy);

	dummy = new QFrame;
	QGridLayout *g = new QGridLayout (dummy);
	g->setMargin (0);
	dummy->setFrameShape (QFrame::StyledPanel);

	CollectionView *view = new CollectionView (dummy, client);
	g->addWidget (view, 0, 0);

	split->addWidget (dummy);

	QList<int> l;
	l.append (200);
	l.append (split->size ().width () - 200);
	split->setSizes (l);

	resize (500, 400);
}

