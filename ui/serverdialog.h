#ifndef __SERVERDIALOG_H__
#define __SERVERDIALOG_H__

#include "xclient.h"

#include <QDialog>
#include <QListWidget>
#include <QWidget>
#include <QLineEdit>

class AddServerDialog : public QDialog
{
	Q_OBJECT
	public:
		AddServerDialog (QWidget *);
		QLineEdit *m_path;
		QLineEdit *m_name;
};

class ServerDialog : public QDialog
{
	Q_OBJECT
	public:
		ServerDialog (QWidget *);
		QString get_path ();
		QString get_default ();

	private slots:
		void add_server ();
		void remove_server ();

	private:
		QListWidget *m_list;
};

#endif
