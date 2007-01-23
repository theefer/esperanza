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

#include "collections/propertyrow.h"

PropertyRow::PropertyRow (QWidget *parent, XClient *client) : QFrame (parent)
{
    m_client = client;
    setFrameStyle (QFrame::StyledPanel);
    
    m_hbox = new QHBoxLayout (this);
    
    /* first combo box */
    m_combo = new QComboBox (this);
    m_combo->addItem (tr ("([Property)"));
    m_combo->addItem (tr ("Artist"));
    m_combo->addItem (tr ("Album"));
    m_combo->addItem (tr ("Title"));
    m_combo->addItem (tr ("Genre"));
    m_combo->addItem (tr ("Year"));
    m_combo->setEditable (true);
    
    connect (m_combo, SIGNAL (activated (int)), this, SLOT (selected (int)));
    m_hbox->addWidget (m_combo);
    
    m_operator = new QComboBox (this);
    m_operator->addItem (tr ("Equals"));
    m_operator->addItem (tr ("Not equals"));
    m_operator->addItem (tr ("Contains"));
    connect (m_operator, SIGNAL (activated (int)), this, SLOT (sel_operator (int)));
    m_hbox->addWidget (m_operator);
    m_operator->hide ();
    
    m_value = new QLineEdit (this);
    m_hbox->addWidget (m_value);
    m_value->hide ();
}

void
PropertyRow::set_values (const QString &prop, int oper, const QString &val)
{
    m_combo->setEditText (prop);
    
    m_operator->setCurrentIndex (oper);
    m_operator->show ();
    
    m_value->setText (val);
    m_value->show ();
}

void
PropertyRow::sel_operator (int i)
{
    m_value->show ();
}

void
PropertyRow::selected (int i)
{
    if (i == 0) {
        m_operator->hide ();
    } else {
        m_operator->show ();        
    }
}