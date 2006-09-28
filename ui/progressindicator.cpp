#include "progressindicator.h"

#include <QLabel>

ProgressIndicator::ProgressIndicator (QWidget *parent) : QLabel (parent)
{
	m_movie = new QMovie (":images/progress.mng", NULL, this);
	setMovie (m_movie);
//	setSizePolicy (QSizePolicy::Fixed, QSizePolicy::Fixed);

//	connect (m_movie, SIGNAL (frameChanged (int)), this, updateImage ());
	hide ();
}

/*
QSize
ProgressIndicator::sizeHint () const
{
	return m_movie->size ();
}
*/

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

