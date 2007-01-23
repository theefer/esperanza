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

#ifndef __PROPERTYEDITOR_H__
#define __PROPERTYEDITOR_H__

#include "xclient.h"

#include <QScrollArea>
#include <QVBoxLayout>
#include "collections/propertyrow.h"

class PropertyEditor : public QScrollArea
{
    Q_OBJECT
    public:
        PropertyEditor (QWidget *parent, XClient *client);
        void add_row (PropertyRow *);

    private:
        QVBoxLayout *m_vbox;
        QList<PropertyRow *> m_properties;
};

#endif