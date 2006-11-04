#include "infotabart.h"

#include <QGridLayout>
#include <QHash>
#include <QVariant>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QSettings>
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>

InfoTabArt::InfoTabArt (QWidget *parent, XClient *client) :
	InfoWindowTab (parent, client)
{
	QGridLayout *g = new QGridLayout (this);
	g->setMargin (1);
	setLayout (g);

	m_art = new QLabel (this);
	m_art->setAlignment (Qt::AlignCenter);
	m_art->setText (tr ("No Image"));
	g->addWidget (m_art, 0, 0);
	g->setRowStretch (0, 1);

	QFrame *line = new QFrame (this);
	line->setFrameStyle (QFrame::HLine | QFrame::Raised);
	g->addWidget (line, 1, 0);

	QDialogButtonBox *bbox = new QDialogButtonBox (this);

	m_browse = new QPushButton (tr ("Select"));
	m_browse->setDefault (true);
	connect (m_browse, SIGNAL (clicked ()), this, SLOT (browse ()));

	m_reset = new QPushButton (tr ("Reset"));
	connect (m_reset, SIGNAL (clicked ()), this, SLOT (reset ()));
	m_reset->setEnabled (false);

	m_apply = new QPushButton (tr ("Apply"));
	connect (m_apply, SIGNAL (clicked ()), this, SLOT (apply ()));
	m_apply->setEnabled (false);

	bbox->addButton (m_browse, QDialogButtonBox::ActionRole);
	bbox->addButton (m_reset, QDialogButtonBox::DestructiveRole);
	bbox->addButton (m_apply, QDialogButtonBox::ApplyRole);
	g->addWidget (bbox, 2, 0);
}

void
InfoTabArt::browse ()
{
	QString file = QFileDialog::getOpenFileName (this,
												 tr ("Select albumart"),
												 QDir::homePath (),
												 tr ("Images (*.png *.jpg)"));
	if (!file.isNull ()) {
		QPixmap p (file);
		setPixmap (p);
		m_browse->setDefault (false);
		m_apply->setEnabled (true);
		m_reset->setEnabled (true);
		m_apply->setDefault (true);
	}

}

void
InfoTabArt::reset ()
{
	QPixmap p = m_client->cache ()->get_pixmap (m_current_id);
	setPixmap (p);
	m_browse->setDefault (true);
	m_apply->setEnabled (false);
	m_reset->setEnabled (false);
	m_apply->setDefault (false);
}

void
InfoTabArt::apply ()
{
	int ret = QMessageBox::question (this, tr ("Esperanza - Question"),
									 tr ("Do you want to change art for the whole album?\nPress yes to apply to the whole album and no to apply only to this song."),
									 QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel,
									 QMessageBox::Yes);
	if (ret == QMessageBox::Yes) {
	} else if (ret == QMessageBox::No) {
	} else if (ret == QMessageBox::Cancel) {
	}
}

void
InfoTabArt::fill (uint32_t id)
{
	m_current_id = id;
	QPixmap p = m_client->cache ()->get_pixmap (id);
	setPixmap (p);
}

void
InfoTabArt::setPixmap (const QPixmap &p)
{
	if (p.isNull ()) {
		m_art->setPixmap (QPixmap ());
		m_art->setText (tr ("No Image"));
	} else {
		if (p.width () > 220) {
			QPixmap small = p.scaledToWidth (220, Qt::SmoothTransformation);
			m_art->setPixmap (small);
		} else {
			m_art->setPixmap (p);
		}
	}
}

