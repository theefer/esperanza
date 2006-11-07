#ifndef __SERVERDIALOG_H__
#define __SERVERDIALOG_H__

#include "xclient.h"
#include "mdns.h"

#include <QDialog>
#include <QListWidget>
#include <QWidget>
#include <QLineEdit>
#include <QKeyEvent>

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
		ServerDialog (QWidget *, MDNSQuery *);
		QString get_path ();
		QString get_default ();

		void keyPressEvent (QKeyEvent *);

	private slots:
		void add_server ();
		void remove_server ();
		void mdns_server_update ();

	private:
		QListWidget *m_list;
		MDNSQuery *m_mdns;
		QList<QListWidgetItem *> m_mdns_servers;
};

#endif
