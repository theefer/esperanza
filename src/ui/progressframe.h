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


/***************************************************************************
 *   Copyright (C) 2005 - 2006 by                                          *
 *      Christian Muehlhaeuser, Last.fm Ltd <chris@last.fm>                *
 *      Erik Jaelevik, Last.fm Ltd <erik@last.fm>                          *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Steet, Fifth Floor, Boston, MA  02111-1307, USA.          *
 ***************************************************************************/

#ifndef PROGRESSFRAME_H
#define PROGRESSFRAME_H

#include "xclient.h"

#include <QtGui>

class ProgressFrame : public QFrame
{
    Q_OBJECT

    public:
        ProgressFrame (QWidget *parent, XClient *client, bool seek = true);

        void clear() { setText( "" ); }

        int maximum() { return m_maxValue; }
        void setMaximum( int value ) { m_maxValue = value; }
        int currentValue() { return m_curValue; }

        QString text() { return m_text; }
        void setText( const QString& text );
    
        void setReverse( bool reverse ) { m_reverse = reverse; }

        int itemType() { return m_itemType; }
        void setItemType( int type ) { m_itemType = type; }

		void mouseMoveEvent (QMouseEvent *);
		void mousePressEvent (QMouseEvent *);

		void setMove (bool b) {
			m_move = b;
		};

    public slots:
        void setValue( int value );
        void playtime( uint32_t tme);
		void got_connection (XClient *);

	private slots:
		void entry_changed (uint32_t id);
    private:
		bool handle_current_id (const unsigned int &);
		bool handle_status (const Xmms::Playback::Status &);
		void new_info (const QHash<QString, QVariant>&);
	
        void paintEvent( QPaintEvent* event );

        QString m_text;
        
        int m_maxValue;
        int m_curValue;

        int m_itemType;
        QHash<QString, QString> m_itemData;

        bool m_reverse;
		bool m_move;
		
		uint32_t m_diffx;
		uint32_t m_diffy;

		XClient *m_client;
		bool m_seek;
		
		uint32_t m_current_id;
};

#endif
