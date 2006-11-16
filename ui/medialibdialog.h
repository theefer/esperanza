#ifndef __MEDIALIBDIALOG_H__
#define __MEDIALIBDIALOG_H__

#include "xclient.h"
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QDialog>
#include <QCompleter>

#include "medialibview.h"
#include "progressindicator.h"

class MedialibDialog : public QDialog
{
	Q_OBJECT

	public:
		MedialibDialog (QWidget *, XClient *);

		void resizeEvent (QResizeEvent *);
		void keyPressEvent (QKeyEvent *);
		void search (int, const QString &, bool a = false);

	private slots:
		void do_search ();
		void search_done ();
		void plus_pressed (QMouseEvent *);
		void load_compl_list (int);

	private:
		bool compl_reply (const Xmms::List < Xmms::Dict > &);

		QLineEdit *m_le;
		QComboBox *m_qb;
		QCheckBox *m_cb;
		QCompleter *m_completer;

		MedialibView *m_list;

		XClient *m_client;

		ProgressIndicator *m_indicator;

};

#endif
