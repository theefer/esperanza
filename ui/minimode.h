#ifndef __MINIMODE_H__
#define __MINIMODE_H__

#include "xclient.h"
#include "progressframe.h"
#include "playerbutton.h"
#include <QWidget>

class MiniMode : public QWidget
{
	Q_OBJECT
	public:
		MiniMode (QWidget *, XClient *);
		void update_playbutton (const QPixmap &p)
		{
			m_playbutt->setPixmap (p);
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

	private slots:
		void min_pressed ();

	private:
		XClient *m_client;
		ProgressFrame *m_progress;
		QWidget *m_parent;

		PlayerButton *m_playbutt;
};

#endif
