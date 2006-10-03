#include "preferencessupport.h"

#include <QGridLayout>
#include <QPushButton>
#include <QColor>
#include <QLabel>
#include <QColorDialog>
#include <QMessageBox>
#include <QApplication>
#include <QLineEdit>
#include <QKeyEvent>

ColorButton::ColorButton (QWidget *parent, const QColor &val) : QLabel (parent)
{
	QPixmap p (80, 20);
	p.fill (val);
	setPixmap (p);
	m_color = val;
	setAlignment (Qt::AlignHCenter | Qt::AlignVCenter);
}

void
ColorButton::mouseDoubleClickEvent (QMouseEvent *ev)
{
	QColor col = QColorDialog::getColor (m_color);
	if (col.isValid ()) {
		QPixmap p (80, 20);
		p.fill (col);
		m_color = col;
		setPixmap (p);
	}
}

QColor
ColorButton::current_color () const
{
	return m_color;
}

KeyInputDialog::KeyInputDialog (QWidget *parent, const QString &key) : QDialog (parent)
{
	QGridLayout *g = new QGridLayout (this);
	QLabel *l = new QLabel (tr ("Press key to act as shortcut:"), this);
	m_l = new QLabel (this);
	if (!key.isEmpty ())
		m_l->setText (key);
	else
		m_l->setText (tr ("None"));
	g->addWidget (l, 0, 0, 1, 2);
	g->addWidget (m_l, 1, 0, 1, 2);

	QPushButton *ok = new QPushButton (tr ("Ok"), this);
	QPushButton *cancel = new QPushButton (tr ("Cancel"), this);

	g->addWidget (ok, 2, 0);
	g->addWidget (cancel, 2, 1);

	connect (ok, SIGNAL (clicked ()), this, SLOT (accept ()));
	connect (cancel, SIGNAL (clicked ()), this, SLOT (reject ()));
}

void
KeyInputDialog::keyPressEvent (QKeyEvent *ev)
{
	if (ev->key () == Qt::Key_Return) {
		accept ();
		return;
	} else if (ev->key () == Qt::Key_Escape) {
		reject ();
		return;
	}

	if (!ev->text ().isEmpty () && ev->key () >= 0x30 && ev->key () < 0x7e) {
		m_l->setText (ev->text ());
		m_key = ev->text ();
	} else {
		m_l->setText (tr ("None"));
		m_key = QString ();
	}
}

KeyWidget::KeyWidget (QWidget *parent, const QString &key) : QLabel (parent)
{
	setAlignment (Qt::AlignHCenter | Qt::AlignVCenter);

	m_key = key;

	if (m_key.isEmpty ()) {
		setText (tr ("None"));
	} else {
		setText (m_key);
	}
}

void
KeyWidget::mouseDoubleClickEvent (QMouseEvent *ev)
{
	KeyInputDialog d (this, m_key);
	if (d.exec () == QDialog::Accepted) {
		m_key = d.get_key ();
		if (m_key.isEmpty ()) {
			setText (tr ("None"));
		} else {
			setText (m_key);
		}
	}
}


