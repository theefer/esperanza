#ifndef __MEDIALIBDIALOG_H__
#define __MEDIALIBDIALOG_H__

#include "xclient.h"
#include <QLineEdit>
#include <QComboBox>
#include <QMainWindow>

#include "medialibview.h"

class MedialibDialog : public QMainWindow
{
	Q_OBJECT

	public:
		MedialibDialog (QWidget *, XClient *);

		void resizeEvent (QResizeEvent *);
		void moveEvent (QMoveEvent *);

	public slots:
		void do_search ();
		void plus_pressed (QMouseEvent *);

	private:
		QLineEdit *m_le;
		QComboBox *m_qb;

		MedialibView *m_list;

		XClient *m_client;

};

#endif
