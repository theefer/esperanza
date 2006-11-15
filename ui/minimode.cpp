#include "xclient.h"
#include "minimode.h"
#include "playerbutton.h"
#include "progressframe.h"
#include "playerwidget.h"
#include "volumebar.h"

#include <QWidget>
#include <QGridLayout>
#include <QMoveEvent>
#include <QKeyEvent>
#include <QVariant>

MiniMode::MiniMode (QWidget *parent, XClient *client) : QFrame (NULL)
{
	setWindowFlags (Qt::FramelessWindowHint);
	setWindowTitle (tr ("Esperanza"));
	setAttribute (Qt::WA_QuitOnClose, false);
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

	m_volume = new VolumeButton (this, m_client);
	g->addWidget (m_volume, 0, 4);
	if (!s.value ("ui/volumepopup").toBool ())
		m_volume->hide ();

	g->addWidget (sett, 0, 5);
	g->addWidget (m_progress, 0, 6);
	g->addWidget (minmax, 0, 7);

	g->setColumnStretch (6, 1);
	g->setMargin (1);

	connect (m_client->settings (), SIGNAL (settingsChanged ()),
			 this, SLOT (changed_settings ()));

	move (s.value ("minimode/pos", parent->pos ()).toPoint ());
}

void
MiniMode::keyPressEvent (QKeyEvent *ev)
{
	PlayerWidget *pw = dynamic_cast<PlayerWidget *> (m_parent);
	pw->keyPressEvent (ev);
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

	if (!s.value ("ui/volumepopup").toBool ())
		m_volume->hide ();
	else
		m_volume->show ();
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
