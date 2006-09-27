#ifndef __MEDIALIBDIALOG_H__
#define __MEDIALIBDIALOG_H__

#include "xclient.h"
#include <QLineEdit>
#include <QComboBox>
#include <QMainWindow>

#include "medialibview.h"
#include "progressindicator.h"

class MedialibDialog : public QMainWindow
{
	Q_OBJECT

	public:
		MedialibDialog (QWidget *, XClient *);

		void resizeEvent (QResizeEvent *);
		void moveEvent (QMoveEvent *);
		void keyPressEvent (QKeyEvent *);

	private slots:
		void do_search ();
		void search_done ();
		void plus_pressed (QMouseEvent *);

	private:
		QLineEdit *m_le;
		QComboBox *m_qb;

		MedialibView *m_list;

		XClient *m_client;

		ProgressIndicator *m_indicator;

};

#endif
