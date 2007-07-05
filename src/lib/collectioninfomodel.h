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

#ifndef __COLLECTIONINFO_MODEL_H__
#define __COLLECTIONINFO_MODEL_H__

#include "xclient.h"

#include <QStandardItemModel>

class CollectionInfoModel : public QStandardItemModel
{
    Q_OBJECT
    public:
        CollectionInfoModel (QObject *, XClient *, const bool &all_field = false);
    
    public slots:
		void set_collection (const Xmms::Coll::Coll &,
		                     const QList<QString> &,
		                     const QList<QString> &,
		                     const QList<QString> &);
		                     
	private:
        bool info_callback (const Xmms::List<Xmms::Dict> &);
        static std::list<std::string> str_list_to_std (const QList<QString> &);
        XClient *m_client;
        bool m_all_field;
		
};

#endif

