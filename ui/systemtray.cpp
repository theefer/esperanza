#include "xclient.h"
#include "systemtray.h"

#include <QSystemTrayIcon>
#include <QSettings>

#include "playerwidget.h"

SystemTray::SystemTray (QObject *parent, XClient *client) : QSystemTrayIcon (parent)
{
	m_client = client;

	connect (this, SIGNAL (activated (QSystemTrayIcon::ActivationReason)),
			 this, SLOT (systray_trigger (QSystemTrayIcon::ActivationReason)));
	
	setIcon (QIcon (":images/systray.png"));

	QMenu *systray_menu = new QMenu();
	systray_menu->addAction (tr ("Exit"), qApp, SLOT (closeAllWindows ()));
	setContextMenu (systray_menu);

#ifdef Q_WS_MACX
	m_growl = new GrowlNotifier (this, "Esperanza", QStringList ("New song"));
	m_growl->do_registration ();
#endif

}

void
SystemTray::do_notification (const QString &title, const QString &message,
							 const QPixmap &img, MessageIcon icon,
							 int milliseconds)
{
	QSettings s;
	if (s.value ("core/donotification").toBool () && m_last_notif_str != message) {
		m_last_notif_str = message;
		if (m_growl) {
			m_growl->do_notification ("New song", title, message, img);
		} else {
			showMessage (title, message, icon, milliseconds);
		}
	}
}

void 
SystemTray::systray_trigger (QSystemTrayIcon::ActivationReason reason)
{
	PlayerWidget *pw = dynamic_cast<PlayerWidget*> (parent ());

	if (reason == QSystemTrayIcon::Trigger)
	{
		if (pw->isHidden())
			pw->show();
		else
			pw->hide();
	}
}
