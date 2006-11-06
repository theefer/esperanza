#include "xclient.h"
#include "minimode.h"
#include "playerbutton.h"
#include "progressframe.h"

#include <QWidget>
#include <QGridLayout>
#include <QMoveEvent>
#include <QVariant>

MiniMode::MiniMode (QWidget *parent, XClient *client) : QFrame (NULL)
{
	setWindowFlags (Qt::FramelessWindowHint);
	setWindowTitle (tr ("Esperanza"));
	m_client = client;
	m_parent = parent;

	QSettings s;

	setFrameStyle (QFrame::Plain | QFrame::StyledPanel);
	setLineWidth (2);

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

	m_stop = new PlayerButton (this, ":images/playstop.png");
	connect (m_stop, SIGNAL (clicked (QMouseEvent *)),
			 parent, SLOT (playstop_pressed ()));

	PlayerButton *minmax = new PlayerButton (this, ":images/minmax.png");
	connect (minmax, SIGNAL (clicked (QMouseEvent *)), this, SLOT (min_pressed ()));

	g->addWidget (back, 0, 0);
	g->addWidget (m_playbutt, 0, 1);
	g->addWidget (m_stop, 0, 2);

	if (!s.value ("ui/showstop").toBool ())
		m_stop->hide ();

	g->addWidget (fwd, 0, 3);
	g->addWidget (sett, 0, 4);
	g->addWidget (m_progress, 0, 5);
	g->addWidget (minmax, 0, 6);

	g->setColumnStretch (5, 1);
	g->setMargin (1);

	connect (m_client->settings (), SIGNAL (settingsChanged ()),
			 this, SLOT (changed_settings ()));

	move (s.value ("minimode/pos", parent->pos ()).toPoint ());
}

void
MiniMode::showEvent (QShowEvent *sh)
{
	resize (m_parent->size ().width (), 22);
}

void
MiniMode::changed_settings ()
{
	QSettings s;
	if (!s.value ("ui/showstop").toBool ())
		m_stop->hide ();
	else
		m_stop->show ();
}

void
MiniMode::moveEvent (QMoveEvent *ev)
{
	QSettings s;
	s.setValue ("minimode/pos", ev->pos ());
}

void
MiniMode::min_pressed ()
{
	m_parent->show ();
	hide ();
}
