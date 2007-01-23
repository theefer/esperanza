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

#ifndef __PROPERTYROW_H__
#define __PROPERTYROW_H__

#include "xclient.h"

#include <QHBoxLayout>
#include <QComboBox>
#include <QLineEdit>
#include <QFrame>

class PropertyRow : public QFrame
{
    Q_OBJECT
    public:
        PropertyRow (QWidget *, XClient *);
        void set_values (const QString &, int, const QString &);
        
    private slots:
        void selected (int);
        void sel_operator (int);

    private:
        QHBoxLayout *m_hbox;
        QComboBox *m_combo;
        QComboBox *m_operator;
        QLineEdit *m_value;
        XClient *m_client;
};

#endif
