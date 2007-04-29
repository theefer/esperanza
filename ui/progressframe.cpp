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

#include "progressframe.h"
#include <QPainter>


ProgressFrame::ProgressFrame (QWidget *parent, XClient *client) :
    QFrame( parent ),
    m_drawBackground( true ),
    m_drawTime( false ),
    m_reverse( false ),
    m_timer( new QTimer( this ) ),
    m_green( false ),
    m_timerEnabled( false )
{
    m_timer->setInterval( 1000 );
    useInternalTimer();

    pushTimer.setSingleShot( true );
    connect( &pushTimer, SIGNAL( timeout() ), this, SLOT( popText() ) );

    m_maxValue = 0;
    m_value = 0;

    setMinimumHeight( 22 );
    setMinimumWidth( 22 );
    setFrameStyle( QFrame::StyledPanel | QFrame::Sunken );
    setLineWidth( 1 );

	m_client = client;
}


void
ProgressFrame::setText( const QString& text )
{
    m_text = text;
    m_baseText = text;
    update();
}


void
ProgressFrame::pushText( const QString& text )
{
    m_text = text;

    pushTimer.stop();
    pushTimer.start( 3000 );

    update();
}

void
ProgressFrame::popText()
{
    m_text = m_baseText;
    update();
}

void
ProgressFrame::useInternalTimer()
{
    disconnectWatch();

    if ( !m_timerEnabled )
    {
        connect( m_timer, SIGNAL( timeout() ), this, SLOT( updateTimer() ) );
        stop();
        m_drawTime = true;
        m_timerEnabled = true;
        m_timerText = "";
    }
}


void
ProgressFrame::setTimerText( QString text )
{
    m_timerText = text;
    update();
}

void
ProgressFrame::setEnabled( bool enabled )
{
    m_drawTime = enabled;
    update();
}


void
ProgressFrame::disableInternalTimer()
{
    if ( m_timerEnabled )
    {
        stop();
        disconnect( m_timer, SIGNAL( timeout() ), this, SLOT( updateTimer() ) );
        m_timerEnabled = false;
    }
}


void
ProgressFrame::disconnectWatch()
{
	/*
    if ( m_watch != NULL )
    {
        disconnect( m_watch, SIGNAL( valueChanged( int ) ),
                    this,    SLOT( setValue( int ) ) );
        m_watch = NULL;
    }
	*/
}

void
ProgressFrame::paintEvent( QPaintEvent * event )
{
	QSettings s;
    QRect rect = event->rect();
    rect.adjust( 8, 0, -8, 0 );

    // progress bar colour
	QColor top = s.value ("ui/titlebartop").value<QColor> ();
	QColor bottom = s.value ("ui/titlebarbottom").value<QColor> ();

	QPalette p = palette ();

    QColor cactivetop = p.highlight ().color ();
    QColor cactivemiddle = cactivetop.dark ();
    QColor cactivebottom = cactivemiddle.dark ();

	QColor ctop, cmiddle, cbottom;
	if (s.value ("ui/titlelighter").toBool ()) {
		ctop = cactivetop.dark ();
		cmiddle = cactivemiddle.dark ();
		cbottom = cactivebottom.dark ();
	} else {
		ctop = cactivetop.light ();
		cmiddle = cactivemiddle.light ();
		cbottom = cactivebottom.light ();
	}

	/*
    QColor ctop = QColor( 140, 140, 140, 255 );
    QColor cmiddle = QColor( 60, 60, 60, 255 );
    QColor cbottom = QColor( 40, 40, 40, 255 );
	*/

	/*
    QColor ctop = QColor( 243, 247, 252, 255 );
    QColor cmiddle = QColor( 192, 212, 238, 255 );
    QColor cbottom = QColor( 187, 207, 235, 255 );
	*/

    QLinearGradient gradientActive( 0, 0, 0, height() );
    gradientActive.setColorAt( 0, cactivetop );
    gradientActive.setColorAt( 0.5, cactivemiddle );
//    gradientActive.setColorAt( 0.51, cactivebottom );
    gradientActive.setColorAt( 1, cactivebottom );

    QLinearGradient gradient( 0, 0, 0, height() );
    gradient.setColorAt( 0, ctop );
    gradient.setColorAt( 0.5, cmiddle );
//    gradient.setColorAt( 0.51, cbottom );
    gradient.setColorAt( 1, cbottom );

    QPainter painter( this );
    drawFrame( &painter );

    int w = 0;
    if ( m_drawTime )
    {
        if ( m_maxValue > 0 )
        {
            w = (int)( ( (float)m_value / (float)m_maxValue ) * ( width() - 2 ) );
            if ( w > width() - 2 )
                w = width() - 2;
        }

        // active progressbar
        QRect r( 1, 2, w, height() - 3 );
        painter.fillRect( r, gradientActive );
    }

    // background
    QRect r( w + 1, 2, width() - 2 - w, height() - 3 );
    painter.fillRect( r, gradient );

    // icon if one is set
    if ( !icon().isNull() )
    {
        QRect r = rect;
        r.adjust( 0, ( rect.height() - icon().height() ) / 2, 0, 0 );
        r.setSize( icon().size() );
        painter.drawPixmap( r, icon() );

        rect.adjust( icon().width() + 6, 1, 0, 0 );
    }

    painter.setPen (p.highlightedText ().color ());
    painter.setBrush (p.highlightedText ().color ());

    QString timeString;
    if ( m_drawTime && maximum() != 0 )
    {
        bool endReached = ( value() >= maximum() ) && ( maximum() != -1 );
        int time = m_reverse ? maximum() - value() : value();

        if ( endReached )
        {
            if ( !m_timerText.isEmpty() )
            {
                timeString = m_timerText;
                painter.drawText( rect, Qt::AlignRight | Qt::AlignVCenter, m_timerText );
            }
        }
        else
        {
            QString hrs  = QString::number( ( time / 3600 ) > 0 ? ( time / 3600 ) : 0 );
            QString mins = QString::number( ( ( time % 3600 ) / 60 ) > 0 ? ( ( time % 3600 ) / 60 ) : 0 );
            QString secs = QString::number( ( ( time % 3600 ) % 60 ) > 0 ? ( ( time % 3600 ) % 60 ) : 0 );
            timeString = QString( "%1%2%3:%4" )
                            .arg( m_reverse ? "- " : "" )
                            .arg( hrs == "0" ? "" : hrs + ":" )
                            .arg( hrs == "0" ? mins : ( mins.length() < 2 ? "0" + mins : mins ) )
                            .arg( secs.length() < 2 ? "0" + secs : secs );

            painter.drawText( rect, Qt::AlignRight | Qt::AlignVCenter, timeString );
        }
    }
    else
    {
        if ( !m_timerText.isEmpty() )
        {
            timeString = m_timerText;
            painter.drawText( rect, Qt::AlignRight | Qt::AlignVCenter, m_timerText );
        }
    }

    rect.adjust( 0, 0, -( painter.fontMetrics().boundingRect( timeString ).width() + 8 ), 0 );
    painter.drawText( rect, Qt::AlignLeft | Qt::AlignVCenter, text() );
}

void
ProgressFrame::mousePressEvent (QMouseEvent *event)
{
	QPoint p = mapTo (window (), event->pos ());
	m_diffx = p.x ();
	m_diffy = p.y ();

	float pro = (float) m_diffx / (float) width ();
	uint32_t m = (uint32_t) (pro * m_maxValue * 1000);

	m_client->playback.seekMs (m) ();
}

void
ProgressFrame::mouseMoveEvent (QMouseEvent *event)
{
	if (m_move) {
		QWidget *w = window ();
		w->move (event->globalPos ().x () - m_diffx,
				 event->globalPos ().y () - m_diffy);
	}
}

void
ProgressFrame::setValue( int value )
{
	if (m_value != value) {
		m_value = value;
		update();
	}
}

