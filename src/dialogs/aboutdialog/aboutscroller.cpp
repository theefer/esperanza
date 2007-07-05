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

#include "aboutscroller.h"
#include "version.h"

#include <QLabel>
#include <QImage>
#include <QPainter>
#include <QRect>
#include <QTimer>
#include <QFont>
#include <QFontMetrics>

/* TODO: could be nicer */
const char *text = 
"Esperanza " ESPERANZA_VERSION_STR "\n"
"\n"
"***\n"
"\n"
"Author:\n"
"Tobias Rundström\n"
"tru@xmms.org\n"
"\n"
"***\n"
"\n"
"Contributors:\n"
"Thilo Cestonaro\n"
"c.thilo@gmx.de\n"
"\n"
"Gianluca Romanin\n"
"j_zar@openjay.org\n"
"\n"
"Anders Gustafsson\n"
"andersg@xmms.org\n"
"\n"
"Dan Chokola\n"
"dan@chokola.com\n"
"\n"
"Peter Simonsson\n"
"peter.simonsson@gmail.com\n"
"\n"
"***\n"
"\n"
"Translations:\n"
"Sebastien Cevey\n"
"seb@cine7.net\n"
"\n"
"Georg Schild\n"
"dangertools@gmx.at\n"
"\n"
"Auke Schrijnen\n"
"auke@schrijnen.nl\n"
"\n"
"\n"
"\n"
"\n"
"And a great thank you to:\n"
"Everyone @ #xmms2\n"
"\n"
"\n"
"\n"
"\n"
"\\o/\n";

QPixmap
AboutScroller::preparePixmap ()
{
	QString s = QString::fromUtf8 (text);
	
    QStringList lines = s.split ("\n");
    
    QFont f ("Helvetica", 20, 2);
	QFontMetrics fm (f);
    
    QRect r;
    
    int total_h = m_orgpix.height () + 10;
    int bound_w = m_orgpix.width ();
    
    for (int i = 0; i < lines.size (); i ++)
    {
        r = fm.boundingRect (lines.at (i));
        total_h += r.height () + 10;
        if (r.width () > bound_w) {
            bound_w = r.width ();
        }
    }
    bound_w += 5;
    
    QPixmap p (bound_w, total_h);
    p.fill (Qt::transparent);
    
    QPainter paint (&p);
    paint.setRenderHint (QPainter::TextAntialiasing, true);
    paint.setRenderHint (QPainter::Antialiasing, true);
    paint.setFont (f);
    paint.setBrush (Qt::black);
    paint.drawPixmap ((p.width () - m_orgpix.width ()) / 2, 0, m_orgpix);

    int h = m_orgpix.height () + 10;
    for (int i = 0; i < lines.size (); i ++) {
        r = fm.boundingRect (lines.at (i));
        h += r.height ();
        
        paint.drawText ((p.width () - r.width ()) / 2, h, lines.at (i));
        h += 10;
    }

    return p;
}

AboutScroller::AboutScroller (QWidget *parent) : QWidget (parent), m_orgpix (":images/esperanza.png")
{
	m_orgpix = preparePixmap();
	
	m_y = m_orgpix.height () + height ();	
	
    m_time = new QTimeLine (m_orgpix.height () * 50, this);
    m_time->setLoopCount (1);
    m_time->setUpdateInterval (5);
    m_time->setDirection (QTimeLine::Forward);
    m_time->setFrameRange (0, height () + m_orgpix.height ());
    m_time->setCurveShape (QTimeLine::LinearCurve);
    connect (m_time, SIGNAL (frameChanged (int)), this, SLOT (advance_y (int)));
    connect (m_time, SIGNAL (finished ()), this, SLOT (done_y ()));
    m_time->start();

	setSizePolicy (QSizePolicy::Fixed, QSizePolicy::Fixed);
	setMinimumSize (m_orgpix.width () * 2, m_orgpix.width ());
	setMaximumSize (m_orgpix.width () * 2, m_orgpix.width ());
    
}

void
AboutScroller::done_y ()
{
    m_y = m_orgpix.height () + height ();
    repaint ();
}

void
AboutScroller::advance_y (int x)
{
	m_y = height () - x;
	repaint ();
}

void
AboutScroller::update_pixmap ()
{
	m_p = QPixmap (size ());
	m_p.fill (Qt::transparent);

	QPainter p (&m_p);
	p.setRenderHint (QPainter::Antialiasing, true);
	p.drawPixmap ((width () - m_orgpix.width ()) / 2, m_y, m_orgpix);

	p.setCompositionMode(QPainter::CompositionMode_DestinationOut);
	m_grad = QLinearGradient (0, height (), 0, 0);
	m_grad.setColorAt (0.0, QColor (255, 255, 255, 255));
	m_grad.setColorAt (0.3, QColor (255, 255, 255, 0));
	m_grad.setColorAt (0.7, QColor (255, 255, 255, 0));
	m_grad.setColorAt (1.0, QColor (255, 255, 255, 255));
	p.setBrush (QBrush (m_grad));
	p.drawRect (rect ());
	p.end ();
}

void
AboutScroller::paintEvent (QPaintEvent *ev)
{
	Q_UNUSED (ev);

	update_pixmap ();
	
    QPainter p (this);
    p.setRenderHint (QPainter::Antialiasing, true);
	p.drawPixmap (rect (), m_p);
    p.end ();
}
