#include <xmmsclient/xmmsclient++.h>

#include <QTreeView>
#include <QErrorMessage>
#include <QHeaderView>
#include <QApplication>
#include <QImageReader>
#include <QSettings>
#include <QColor>

#include "xclient.h"
#include "playerwidget.h"
#include "serverdialog.h"
#include "preferences.h"

int
main (int argc, char **argv)
{
	QApplication app(argc, argv);
	QApplication::setDesktopSettingsAware (false);

	QCoreApplication::setOrganizationName("XMMS2");
	QCoreApplication::setOrganizationDomain("xmms.org");
	QCoreApplication::setApplicationName("Esperanza");

	PreferenceDialog::save_defaults ();

	QSettings s;

	XClient client (NULL, "Esperanza");

	QString path;

	PlayerWidget *pw = new PlayerWidget (NULL, &client);

browser:
	if (!getenv ("XMMS_PATH")) {
		ServerDialog sd (NULL);
		if (!s.value ("serverdialog/show").toBool ()) {
			path = sd.get_default ();
		} else {
			path = sd.get_path ();
		}
		if (path == "local") {
			path = "";
		} else if (path.isNull ()) {
			return EXIT_FAILURE;
		}
	} else {
		path = QString::fromAscii (getenv ("XMMS_PATH"));
	}

	if (!client.connect (path.toStdString ()))
		goto browser;

	pw->show ();

	return app.exec ();
}

