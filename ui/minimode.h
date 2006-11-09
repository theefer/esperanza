#ifndef __MINIMODE_H__
#define __MINIMODE_H__

#include "xclient.h"
#include "progressframe.h"
#include "playerbutton.h"
#include "volumebar.h"

#include <QWidget>
#include <QFrame>

class MiniMode : public QFrame
{
	Q_OBJECT
	public:
		MiniMode (QWidget *, XClient *);
		void update_playbutton (const QString &s)
		{
			m_playbutt->setPx (s);
		};

		void setText (const QString &s)
		{
			m_progress->setText (s);
		};

		void setMaximum (uint32_t max)
		{
			m_progress->setMaximum (max);
		};

		void setValue (uint32_t val)
		{
			m_progress->setValue (val);
		};

		void moveEvent (QMoveEvent *);
		void showEvent (QShowEvent *);

	private slots:
		void min_pressed ();
		void changed_settings ();

	private:
		XClient *m_client;
		ProgressFrame *m_progress;
		QWidget *m_parent;

		PlayerButton *m_playbutt;
		PlayerButton *m_stop;
		VolumeButton *m_volume;
};

#endif
