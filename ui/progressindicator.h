#ifndef __PROGRESSINDICATOR_H__
#define __PROGRESSINDICATOR_H__

#include <QLabel>
#include <QMovie>

class ProgressIndicator : public QLabel
{
	Q_OBJECT
	public:
		ProgressIndicator (QWidget *);
		void setStatus (bool);
		/*
		QSize sizeHint () const;
		*/
	private:
		QMovie *m_movie;
};

#endif

