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

#include "progressframe.h"
#include <QPainter>

ProgressFrame::ProgressFrame (QWidget *parent, XClient *client, bool seek) :
    QFrame( parent ),
    m_reverse( false )
{
	m_client = client;
    m_maxValue = 0;
    m_curValue = 0;
	m_seek = seek;

    setMinimumHeight( 22 );
    setMinimumWidth( 22 );
    setFrameStyle( QFrame::StyledPanel | QFrame::Sunken );
	setSizePolicy( QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding );
    setLineWidth( 1 );
	
	connect (client->cache (), SIGNAL (playtime (uint32_t)), this, SLOT (playtime (uint32_t)));
	connect (client->cache (), SIGNAL (entryChanged (uint32_t)), this, SLOT (entry_changed (uint32_t)));
	connect (client, SIGNAL (gotConnection (XClient *)), this, SLOT (got_connection (XClient *)));
}

void
ProgressFrame::got_connection (XClient *client)
{
	m_client = client;

	client->playback ()->getStatus () (Xmms::bind (&ProgressFrame::handle_status, this));
	client->playback ()->broadcastStatus () (Xmms::bind (&ProgressFrame::handle_status, this));

	client->playback ()->broadcastCurrentID () (Xmms::bind (&ProgressFrame::handle_current_id, this));
	client->playback ()->currentID () (Xmms::bind (&ProgressFrame::handle_current_id, this));
}

bool
ProgressFrame::handle_status (const Xmms::Playback::Status &st)
{
	if (st == Xmms::Playback::STOPPED) {
		setValue (0);
	}
	return true;
}

void
ProgressFrame::entry_changed (uint32_t id)
{
	if (id == m_current_id) {
		new_info (m_client->cache ()->get_info (id));
	}
}

void
ProgressFrame::new_info (const QHash<QString,QVariant> &h)
{
	QString s;
	if (!h.contains ("title")) {
		s = h["url"].toString ();
	} else {
		s = QString ("%1 - %2")
			.arg(h["artist"].toString ())
			.arg(h["title"].toString ());
	}

	setText (s);
	
	if (h.contains ("duration")) {
		uint32_t dur = h["duration"].toUInt ();
		setMaximum (dur / 1000);
		setValue (0);
	}
}

bool
ProgressFrame::handle_current_id (const unsigned int &id)
{
	m_current_id = id;
	new_info (m_client->cache ()->get_info (id));
	return true;
}


void
ProgressFrame::setText( const QString& text )
{
    m_text = text;
    update();
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
	if (!s.value ("ui/titlelighter").toBool ()) {
		ctop = cactivetop.dark ();
		cmiddle = cactivemiddle.dark ();
		cbottom = cactivebottom.dark ();
	} else {
		ctop = cactivetop.light ();
		cmiddle = cactivemiddle.light ();
		cbottom = cactivebottom.light ();
	}

    QLinearGradient gradientActive( 0, 0, 0, height() );
    gradientActive.setColorAt( 0, cactivetop );
    gradientActive.setColorAt( 0.5, cactivemiddle );
    gradientActive.setColorAt( 1, cactivebottom );

    QLinearGradient gradient( 0, 0, 0, height() );
    gradient.setColorAt( 0, ctop );
    gradient.setColorAt( 0.5, cmiddle );
    gradient.setColorAt( 1, cbottom );

    QPainter painter( this );
    drawFrame( &painter );

    int w = 0;
    if ( m_maxValue > 0 )
	{
		w = (int)( ( (float)currentValue () / (float)m_maxValue ) * ( width() - 2 ) );
		if ( w > width() - 2 )
			w = width() - 2;
	}
	// active progressbar
	QRect r( 1, 2, w, height() - 3 );
	painter.fillRect( r, gradientActive );
 
    // background
    r = QRect ( w + 1, 2, width() - 2 - w, height() - 3 );
    painter.fillRect( r, gradient );

    painter.setPen (p.highlightedText ().color ());
    painter.setBrush (p.highlightedText ().color ());

    QString timeString;
    if ( maximum() != 0 )
    {
        int time = m_reverse ? maximum() - currentValue() : currentValue();

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

    rect.adjust( 0, 0, -( painter.fontMetrics().boundingRect( timeString ).width() + 8 ), 0 );
    painter.drawText( rect, Qt::AlignLeft | Qt::AlignVCenter, text() );
}

void
ProgressFrame::mousePressEvent (QMouseEvent *event)
{
	QPoint p = mapTo (window (), event->pos ());
	m_diffx = p.x ();
	m_diffy = p.y ();

	/* some windows showing this don't want to seek */
	if (m_seek) {
		float pro = (float) m_diffx / (float) width ();
		uint32_t m = (uint32_t) (pro * m_maxValue * 1000);

		m_client->playback ()->seekMs (m) ();
	}
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
	if (m_curValue != value) {
		m_curValue = value;
		update();
	}
}

void
ProgressFrame::playtime( uint32_t tme)
{
	setValue (tme / 1000);
}
