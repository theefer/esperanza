#include "volumebar.h"
#include "playerbutton.h"

#include <QVBoxLayout>
#include <QSettings>

VolumeButton::VolumeButton (QWidget *parent, XClient *client) : QWidget (parent)
{
	m_client = client;

	setMaximumSize (100, 20);
	setToolTip ("Volumebar");

	/* Create the slider */
	m_slider = new QSlider (Qt::Horizontal, this);
	m_slider->hide ();
	m_slider->setMaximum (100);
	m_slider->setMinimum (0);
	m_slider->setTickInterval (5);
	m_slider->setTracking (false);

	m_volbar = new VolumeBar (this);

	connect (m_client, SIGNAL (gotConnection (XClient *)), this, SLOT (got_connection (XClient *)));
	connect (m_slider, SIGNAL (valueChanged (int)), this, SLOT (set_volume (int)));

	/* we add both and let change_settings decide */
	m_hbox = new QHBoxLayout (this);
	m_hbox->setMargin (0);
	m_button = new PlayerButton (this, ":images/volume.png");
	m_button->hide ();

	connect (m_button, SIGNAL (clicked (QMouseEvent *)),
			 this, SLOT (volume_pressed (QMouseEvent *)));

	m_hbox->addWidget (m_button);
	m_hbox->addWidget (m_slider);

	connect (m_client->settings (), SIGNAL (settingsChanged ()),
			 this, SLOT (changed_settings ()));

	changed_settings ();
}

void
VolumeButton::changed_settings ()
{
	QSettings s;

	if (s.value ("ui/volumepopup").toBool ()) {
		m_slider->hide ();
		m_button->show ();
	} else {
		m_button->hide ();
		m_slider->show ();
	}
}

void
VolumeButton::volume_pressed (QMouseEvent *ev)
{
	qDebug ("pressed!");
	m_volbar->move (ev->globalPos ());
	m_volbar->show ();
}

void
VolumeButton::got_connection (XClient *c)
{
	m_client->playback.volumeGet (Xmms::bind (&VolumeButton::handle_volume, this));
	m_client->playback.broadcastVolumeChanged (Xmms::bind (&VolumeButton::handle_volume, this));
}

bool
VolumeButton::handle_volume (const Xmms::Dict &d)
{
	/* XXX: this function really should do each and sum / split the channels */
	if (d.contains ("master")) {
		if (!m_slider->isSliderDown ()) {
			m_slider->setValue (d.get<uint32_t> ("master"));
			m_volbar->setValue (d.get<uint32_t> ("master"));
		}
		m_channels = 1;
	} else if (d.contains ("left") && d.contains ("right")) {
		if (!m_slider->isSliderDown ()) {
			m_slider->setValue ((d.get<uint32_t> ("left") + d.get<uint32_t> ("right")) / 2);
			m_volbar->setValue ((d.get<uint32_t> ("left") + d.get<uint32_t> ("right")) / 2);
		}
		m_channels = 2;
	}
	return true;
}

void
VolumeButton::set_volume (int vol)
{
	if (m_channels == 1) {
		m_client->playback.volumeSet ("master", vol, &XClient::log);
	} else {
		m_client->playback.volumeSet ("left", vol, &XClient::log);
		m_client->playback.volumeSet ("right", vol, &XClient::log);
	}
}

VolumeBar::VolumeBar (QWidget *parent) : QWidget (NULL)
{
	setWindowFlags (Qt::FramelessWindowHint);
	setAttribute (Qt::WA_DeleteOnClose);

	m_box = new QVBoxLayout (this);

	m_slider = new QSlider (Qt::Vertical, this);
	m_slider->setMaximum (100);
	m_slider->setMinimum (0);
	m_slider->setTickInterval (5);
	m_slider->setTracking (false);
	connect (m_slider, SIGNAL (valueChanged (int)), parent, SLOT (set_volume (int)));
	resize (m_slider->sizeHint ());

	m_box->addWidget (m_slider);

	setFocusPolicy (Qt::StrongFocus);

	hide ();
}

void
VolumeBar::focusOutEvent (QFocusEvent *ev)
{
	hide ();
}

