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

#ifndef __PANEBROWSERVIEW_H__
#define __PANEBROWSERVIEW_H__

#include "xclient.h"

#include "collectioninfomodel.h"

#include <QWidget>
#include <QTreeView>

class PaneBrowserView : public QTreeView
{
    Q_OBJECT
    public:
        PaneBrowserView (QWidget *, XClient *, const QString &);
    
    public slots:
        void item_filter (const QString &);
        void item_clicked (const QModelIndex &);
        
    signals:
        void filter (const QString &);
        
    private:
        XClient *m_client;
        CollectionInfoModel *m_model;
        QItemSelectionModel *m_selection;
        QString m_view;
        
};

#endif