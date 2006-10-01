
#include "growl.h"
#include "qtmd5.h"

#include <QDataStream>
#include <QByteArray>
#include <QIODevice>

GrowlNotifier::GrowlNotifier (QObject *parent,
							  const QString &appname,
							  const QStringList &notifications,
							  const QHostAddress &host) : 
	QObject (parent),
	m_socket (this)
{
	if (!host.isNull ()) {
		m_host = host;
	}
	m_appname = appname.toUtf8 ();
	m_notifications = notifications;
}

void
GrowlNotifier::do_notification (const QString &notif,
								const QString &title,
								const QString &desc)
{
	if (!m_host.isNull ())
		do_notification_net (notif, title, desc);
}

void
GrowlNotifier::do_registration ()
{
	if (!m_host.isNull ())
		do_registration_net ();
}

/* 
 * This functions will work on any platform since they just
 * send a UDP packet to host
 */
void
GrowlNotifier::send_data (const QByteArray &b)
{
	m_socket.writeDatagram (b, m_host, 9887);
}

void
GrowlNotifier::do_notification_net (const QString &notification,
							        const QString &title,
							        const QString &desc)
{
	QByteArray notif = notification.toUtf8 ();
	QByteArray d = desc.toUtf8 ();
	QByteArray t = title.toUtf8 ();

	QByteArray *data = new QByteArray ();

	QDataStream s (data, QIODevice::WriteOnly | QIODevice::Unbuffered);
	s.setByteOrder (QDataStream::BigEndian);
	s << (quint8) 1; /* version */
	s << (quint8) 1; /* notification */
	s << (quint16) 0x08; /* flags */

	s << (quint16) notif.size ();
	s << (quint16) t.size ();
	s << (quint16) d.size ();
	s << (quint16) m_appname.size ();

	s.writeRawData (notif, notif.size ());
	s.writeRawData (t, t.size ());
	s.writeRawData (d, d.size ());
	s.writeRawData (m_appname, m_appname.size ());

	unsigned char dig[16];

	qtMD5 (*data, dig);
	s.writeRawData ((char *)dig, 16);

	send_data (*data);

	delete data;
}

void
GrowlNotifier::do_registration_net ()
{
	QByteArray *data = new QByteArray ();
	QDataStream s (data, QIODevice::WriteOnly | QIODevice::Unbuffered);

	s.setByteOrder (QDataStream::BigEndian);
	s << (quint8) 1; /* version */
	s << (quint8) 0; /* type */

	s << (quint16) m_appname.size (); /* application name size */

	s << (quint8) m_notifications.size (); /* number of notifications */
	s << (quint8) m_notifications.size (); /* number of default notifications */

	s.writeRawData (m_appname, m_appname.size ());

	for (int i = 0; i < m_notifications.size (); i ++) {
		QByteArray tmp = m_notifications.at (i).toUtf8 ();
		s << (quint16) tmp.size (); /* size of notification name */
		s.writeRawData (tmp, tmp.size ());
	}

	/* active notifications */
	for (int i = 0; i < m_notifications.size (); i ++) {
		s << (quint8) i;
	}

	unsigned char dig[16];

	qtMD5 (*data, dig);
	s.writeRawData ((char *)dig, 16);

	send_data (*data);

	delete data;
}
