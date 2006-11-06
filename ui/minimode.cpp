#include "xclient.h"
#include "minimode.h"
#include "playerbutton.h"
#include "progressframe.h"

#include <QWidget>
#include <QGridLayout>
#include <QMoveEvent>
#include <QVariant>

MiniMode::MiniMode (QWidget *parent, XClient *client) : QWidget (NULL)
{
	setWindowFlags (Qt::FramelessWindowHint);
	setWindowTitle (tr ("Esperanza"));
	m_client = client;
	m_parent = parent;

	QGridLayout *g = new QGridLayout (this);
	setLayout (g);
	m_progress = new ProgressFrame (this);
	m_progress->setMove (true);
	
	m_playbutt = new PlayerButton (this, ":images/play.png");
	connect (m_playbutt, SIGNAL (clicked (QMouseEvent *)),
			 parent, SLOT (play_pressed ()));

	PlayerButton *back = new PlayerButton (this, ":images/back.png");
	connect (back, SIGNAL (clicked (QMouseEvent *)),
			 parent, SLOT (back_pressed ()));

	PlayerButton *fwd = new PlayerButton (this, ":images/forward.png");
	connect (fwd, SIGNAL (clicked (QMouseEvent *)),
			 parent, SLOT (fwd_pressed ()));

	PlayerButton *sett = new PlayerButton (this, ":images/settings.png");
	connect (sett, SIGNAL (clicked (QMouseEvent *)),
			 parent, SLOT (snett_pressed (QMouseEvent *)));

	PlayerButton *minmax = new PlayerButton (this, ":images/minmax.png");
	connect (minmax, SIGNAL (clicked (QMouseEvent *)), this, SLOT (min_pressed ()));

	g->addWidget (back, 0, 0);
	g->addWidget (m_playbutt, 0, 1);
	g->addWidget (fwd, 0, 2);
	g->addWidget (sett, 0, 3);
	g->addWidget (m_progress, 0, 4);
	g->addWidget (minmax, 0, 5);

	g->setColumnStretch (4, 1);
	g->setMargin (1);
	resize (400, 22);

	QSettings s;
	move (s.value ("mini/pos", parent->pos ()).toPoint ());

}

void
MiniMode::moveEvent (QMoveEvent *ev)
{
	QSettings s;
	s.setValue ("mini/pos", ev->pos ());
}

void
MiniMode::min_pressed ()
{
	m_parent->show ();
	hide ();
}
