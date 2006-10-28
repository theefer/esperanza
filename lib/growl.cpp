
#include "growl.h"
#include "qtmd5.h"

#include "mac_growl.h"

#include <QDataStream>
#include <QByteArray>
#include <QIODevice>
#include <QHash>
#include <QVariant>
#include <QPixmap>

GrowlNotifier::GrowlNotifier (QObject *parent,
							  const QString &appname,
							  const QStringList &notifications) :
	QObject (parent)
{
	m_appname = appname.toUtf8 ();
	m_notifications = notifications;
}

void
GrowlNotifier::do_notification (const QString &notif,
								const QString &title,
								const QString &desc,
								const QPixmap &img)
{
	QHash<QString, QVariant> note;

	note["ApplicationName"] = QVariant (QString (m_appname));
	note["NotificationName"] = QVariant (notif);
	note["NotificationTitle"] = QVariant (title);
	note["NotificationDescription"] = QVariant (desc);
	if (!img.isNull ()) {
		note["NotificationIcon"] = QVariant (img);
	}

#ifdef Q_OS_MACX
	mac_growl_post_notification ("GrowlNotification", note);
#endif
}

void
GrowlNotifier::do_registration ()
{
	QHash<QString, QVariant> note;

	note["ApplicationName"] = QVariant (QString (m_appname));
	note["AllNotifications"] = QVariant (m_notifications);
	note["DefaultNotifications"] = QVariant (m_notifications);
//	note["ApplicationIcon"] = QVariant (QImage (":images/test.jpg"));

#ifdef Q_OS_MACX
	mac_growl_post_notification ("GrowlApplicationRegistrationNotification", note);
#endif
}

