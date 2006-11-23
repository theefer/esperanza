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


#include "textdialog.h"
#include "version.h"

#include <QTextBrowser>
#include <QDialog>
#include <QGridLayout>
#include <QPushButton>

TextDialog::TextDialog (QWidget *parent) : QDialog (parent)
{
	QGridLayout *g = new QGridLayout (this);
	m_textbrowser = new QTextBrowser (this);
	m_textbrowser->setReadOnly (true);
#if QT_VERSION >= 0x040200
	m_textbrowser->setOpenExternalLinks (true);
#endif

	QPushButton *close = new QPushButton (tr ("Close"), this);
	connect (close, SIGNAL (clicked ()), this, SLOT (accept ()));

	g->addWidget (m_textbrowser, 0, 0, 3, 3);
	g->addWidget (close, 3, 2, 1, 1);
}


void
TextDialog::read_file (const QString &filename)
{
	QFile file (filename);
	if (file.open (QFile::ReadOnly)) {
		QByteArray cont = file.readAll ();
		QString s = QString::fromAscii (cont);
		s.replace ("@ESP_VERSION@", ESPERANZA_VERSION_STR);
		s.replace ("@ESP_DATE@", ESPERANZA_DATE_STR);
		s.replace ("@QT_VERSION@", QT_VERSION_STR);
		m_textbrowser->setHtml (s);
	}
}

