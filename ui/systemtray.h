#ifndef _SYSTEMTRAY_H
#define _SYSTEMTRAY_H

#include "xclient.h"

#include <QSystemTrayIcon>

#include "growl.h"

class SystemTray : public QSystemTrayIcon
{
	Q_OBJECT
	public:
		SystemTray (QObject *, XClient *);

		void do_notification (const QString &title, const QString &message, const QPixmap &img = QPixmap (), MessageIcon icon = Information, int milliseconds = 10000);

	private slots:
		void systray_trigger (QSystemTrayIcon::ActivationReason reason);

	private:
		GrowlNotifier *m_growl;
		QString m_last_notif_str;
		XClient *m_client;
};

#endif
