#include <QLabel>
#include <QPixmap>
#include <QSettings>

#include "playerbutton.h"

PlayerButton::PlayerButton (QWidget *parent, const QString &pic) : QLabel (parent)
{
	setMargin (3);

	setPx (pic);
	setFrameStyle (QFrame::StyledPanel | QFrame::Plain);

	setAutoFillBackground (true);
}

void
PlayerButton::enterEvent (QEvent *ev)
{
	QSettings s;

	QPalette p (palette ());
	p.setColor (QPalette::Window, p.color (QPalette::Highlight));
	p.setColor (QPalette::WindowText, p.color (QPalette::HighlightedText));
	setPalette (p);

	if (!m_pixmap_hoover.isNull ())
		setPixmap (m_pixmap_hoover);

	update ();
}

void
PlayerButton::leaveEvent (QEvent *ev)
{
	QPalette p (parentWidget ()->palette ());
	setPalette (p);

	if (!m_pixmap_hoover.isNull ())
		setPixmap (m_pixmap);

	update ();
}

void
PlayerButton::setPx (const QString &pic)
{
	QPalette p (parentWidget ()->palette ());

	QImage i (pic);
	if (i.isNull ()) {
		setText (pic);
	} else {
		i.setColor (0, p.color (QPalette::Text).rgb ());
		m_pixmap = QPixmap::fromImage (i);

		i.setColor (0, p.color (QPalette::HighlightedText).rgb ());
		m_pixmap_hoover = QPixmap::fromImage (i);

		setPixmap (m_pixmap);
	}
}

void
PlayerButton::mousePressEvent (QMouseEvent *ev)
{
	emit clicked (ev);
}

