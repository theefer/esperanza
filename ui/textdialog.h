#ifndef __TEXTDIALOG_H__
#define __TEXTDIALOG_H__

#include <QDialog>
#include <QTextBrowser>
#include <QFile>

class TextDialog : public QDialog
{
	Q_OBJECT
	public:
		TextDialog (QWidget *);
		void read_file (const QString &);
	private:
		QTextBrowser *m_textbrowser;
};

#endif
