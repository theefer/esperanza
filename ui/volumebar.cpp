#include "volumebar.h"
#include <QVBoxLayout>

VolumeBar::VolumeBar (QWidget *parent, XClient *client) : QWidget (parent)
{
	setWindowFlags (Qt::FramelessWindowHint);
	setAttribute (Qt::WA_DeleteOnClose);

	m_client = client;

	QVBoxLayout layout (this);
	m_slider = new QSlider (Qt::Vertical, this);
	m_slider->setMaximum (100);
	m_slider->setMinimum (0);
	m_slider->setTickInterval (5);
	m_slider->setTracking (false);
	layout.addWidget (m_slider);
	resize (m_slider->sizeHint ());

	connect (m_client, SIGNAL (gotConnection (XClient *)), this, SLOT (got_connection (XClient *)));
	connect (m_slider, SIGNAL (valueChanged (int)), this, SLOT (set_volume (int)));

	hide ();
}

void
VolumeBar::got_connection (XClient *c)
{
	m_client->playback.volumeGet (Xmms::bind (&VolumeBar::handle_volume, this));
	m_client->playback.broadcastVolumeChanged (Xmms::bind (&VolumeBar::handle_volume, this));
}

bool
VolumeBar::handle_volume (const Xmms::Dict &d)
{
	/* XXX: this function really should do each and sum / split the channels */
	if (d.contains ("master")) {
		if (!m_slider->isSliderDown ())
			m_slider->setValue (d.get<uint32_t> ("master"));
		m_channels = 1;
	} else if (d.contains ("left") && d.contains ("right")) {
		if (!m_slider->isSliderDown ())
			m_slider->setValue ((d.get<uint32_t> ("left") + d.get<uint32_t> ("right")) / 2);
		m_channels = 2;
	}
	return true;
}

void
VolumeBar::set_volume (int vol)
{
	if (m_channels == 1) {
		m_client->playback.volumeSet ("master", vol, &XClient::log);
	} else {
		m_client->playback.volumeSet ("left", vol, &XClient::log);
		m_client->playback.volumeSet ("right", vol, &XClient::log);
	}
}


void
VolumeBar::focusOutEvent (QFocusEvent *ev)
{
	hide ();
}

