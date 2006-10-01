#ifndef __GROWL_H__
#define __GROWL_H__

#include <QObject>
#include <QUdpSocket>
#include <QStringList>
#include <QHostAddress>

class GrowlNotifier : public QObject
{
	Q_OBJECT
	public:
		GrowlNotifier (QObject *parent, const QString &, const QStringList &, const QHostAddress &addr = QHostAddress ());
		void send_data (const QByteArray &);
		void do_registration ();
		void do_notification (const QString &, const QString &, const QString &);

		void do_notification_net (const QString &, const QString &, const QString &);
		void do_registration_net ();

#ifdef apa
		void do_notification_cf (const QString &, const QString &, const QString &);
		void do_registration_cf ();
#endif
	private:
		QUdpSocket m_socket;
		QByteArray m_appname;
		QStringList m_notifications;
		QHostAddress m_host;
};

#endif
