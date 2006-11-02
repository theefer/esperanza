#ifndef _SYSTEMTRAY_H
#define _SYSTEMTRAY_H

#include "xclient.h"

#include <QSystemTrayIcon>
#include <QAction>

#include "growl.h"

class SystemTray : public QSystemTrayIcon
{
	Q_OBJECT
	public:
		SystemTray (QObject *, XClient *);

		void do_notification (const QString &title, const QString &message, const QPixmap &img = QPixmap (), MessageIcon icon = Information, int milliseconds = 10000);

	private slots:
		void systray_trigger (QSystemTrayIcon::ActivationReason reason);
		void toggle_hide ();
		void build_menu ();

	private:
		QAction *m_display_action;
		QAction *m_hide_action;
		QAction *m_play_action;
		GrowlNotifier *m_growl;
		QString m_last_notif_str;
		XClient *m_client;
};

#endif
