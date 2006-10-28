#ifndef __GROWL_H__
#define __GROWL_H__

#include <QObject>
#include <QStringList>
#include <QPixmap>

class GrowlNotifier : public QObject
{
	Q_OBJECT
	public:
		GrowlNotifier (QObject *parent, const QString &, const QStringList &);
		void do_registration ();
		void do_notification (const QString &, const QString &, const QString &, const QPixmap &img = QPixmap ());

	private:
		QByteArray m_appname;
		QStringList m_notifications;
};

#endif
