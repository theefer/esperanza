#include "progressindicator.h"

#include <QLabel>

ProgressIndicator::ProgressIndicator (QWidget *parent) : QLabel (parent)
{
	m_movie = new QMovie (":images/progress.mng", NULL, this);
	setMovie (m_movie);
	hide ();
}

void
ProgressIndicator::setStatus (bool go)
{
	if (go) {
		show ();
		m_movie->start ();
	} else {
		hide ();
		m_movie->stop ();
	}
}

