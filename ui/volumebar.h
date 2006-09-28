#ifndef __VOLUMEBAR_H__
#define __VOLUMEBAR_H__

#include "xclient.h"

#include <QWidget>
#include <QSlider>

class VolumeBar : public QWidget
{
	Q_OBJECT
	public:
		VolumeBar (QWidget *, XClient *);
		void focusOutEvent (QFocusEvent *);

	private slots:
		void set_volume (int vol);
		void got_connection (XClient *);

	private:
		QSlider *m_slider;
		bool handle_volume (const Xmms::Dict &);
		uint32_t m_channels;
		XClient *m_client;
};

#endif
