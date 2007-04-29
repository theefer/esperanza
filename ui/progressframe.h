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
        ProgressFrame (QWidget *parent, XClient *client);

        void clear() { stop(); setText( "" ); m_timerText = ""; }

        int maximum() { return m_maxValue; }
        void setMaximum( int value ) { m_maxValue = value; }
        int value() { return m_value; }

        QString text() { return m_text; }
        void setText( const QString& text );

        // Temporary message that should only be displayed for a few seconds
        void pushText( const QString& text );
    
        QPixmap icon() { return m_pixmap; }
        void setIcon( QPixmap pixmap ) { m_pixmap = pixmap; }

        void start() { setValue( 0 ); m_timer->start(); }
        void stop() { setMaximum( 0 ); setValue( 0 ); m_timer->stop(); }
        void resume() { m_timer->start(); }
        void pause() { m_timer->stop(); }
        bool isActive() { return m_timer->isActive(); }

        void useInternalTimer();
        void setTimerText( QString text );

        void setBackgroundEnabled( bool drawBackground ) { m_drawBackground = drawBackground; }
        void setReverse( bool reverse ) { m_reverse = reverse; }
        void setEnabled( bool enabled );

        QHash<QString, QString> itemData() { return m_itemData; }
        void setItemData( QHash<QString, QString> data ) { m_itemData = data; }

        int itemType() { return m_itemType; }
        void setItemType( int type ) { m_itemType = type; }

        void setGreen( bool green ) { m_green = green; }

		void mouseMoveEvent (QMouseEvent *);
		void mousePressEvent (QMouseEvent *);

		void setMove (bool b) {
			m_move = b;
		};

    public slots:
        void setValue( int value );

    private:
        void paintEvent( QPaintEvent* event );

        void disableInternalTimer();
        void disconnectWatch();

        QPixmap m_pixmap;

        QString m_baseText;
        QString m_text;
        QString m_timerText; 
        
        int m_maxValue;
        int m_value;

        int m_itemType;
        QHash<QString, QString> m_itemData;

        bool m_drawBackground;
        bool m_drawTime;
        bool m_reverse;
        bool m_green;

        QTimer* m_timer;
        bool m_timerEnabled;
        
        QTimer pushTimer;

		bool m_move;
		uint32_t m_diffx;
		uint32_t m_diffy;

		XClient *m_client;

    private slots:
        void updateTimer() { setValue( value() + 1 ); }
        void popText();
};

#endif
