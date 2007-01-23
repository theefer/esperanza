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

#include "collections/propertyeditor.h"

#include <QWidget>
#include <QDialog>
#include <QGridLayout>
#include <QSplitter>
#include <QListWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QFrame>

PropertyEditor::PropertyEditor (QWidget *parent, XClient *client) : QScrollArea (parent)
{
	setHorizontalScrollBarPolicy (Qt::ScrollBarAlwaysOff);
	m_vbox = new QVBoxLayout (this);
}

void
PropertyEditor::add_row (PropertyRow *row)
{
    m_properties.append (row);
    m_vbox->addWidget (row);
}

