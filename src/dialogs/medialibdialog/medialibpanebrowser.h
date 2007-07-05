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

#ifndef __MEDIALIBPANEBROWSER_H__
#define __MEDIALIBPANEBROWSER_H__

#include "xclient.h"

class MedialibPaneBrowser;
 
#include "medialibdialog.h"
#include "panebrowserview.h"

#include <QWidget>
#include <QHBoxLayout>

class MedialibPaneBrowser : public QWidget
{
    Q_OBJECT
    public:
        MedialibPaneBrowser (QWidget *, XClient *);
        
    private slots:
        void set_filter (const Xmms::Coll::Equals &);
        
    private:
        XClient *m_client;

        QHBoxLayout *m_box;
        PaneBrowserView *m_artists;
        PaneBrowserView *m_albums;
        MedialibDialog *m_parent;        
};

#endif

