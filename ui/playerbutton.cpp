#include <QLabel>
#include <QPixmap>
#include <QSettings>

#include "playerbutton.h"

PlayerButton::PlayerButton (QWidget *parent, const QString &pic) : QLabel (parent)
{
	setMargin (3);
	QPixmap p (pic);
	if (!p.isNull ())
		setPixmap (p);
	else
		setText (pic);
	setFrameStyle (QFrame::StyledPanel | QFrame::Plain);

	setAutoFillBackground (true);
}

void
PlayerButton::enterEvent (QEvent *ev)
{
	QSettings s;

	QPalette p (palette ());
	p.setColor (QPalette::Window,
				s.value ("ui/highlight", QColor (80, 80, 80)).value<QColor> ());
	setPalette (p);
	update ();
}

void
PlayerButton::leaveEvent (QEvent *ev)
{
	QPalette p (parentWidget ()->palette ());
	setPalette (p);
	update ();
}

void
PlayerButton::setPx (const QString &pic)
{
	setPixmap (QPixmap (pic));
}

void
PlayerButton::mousePressEvent (QMouseEvent *ev)
{
	emit clicked (ev);
}

