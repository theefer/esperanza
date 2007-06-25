#include <xmmsclient/xmmsclient++.h>
#include <QSettings>
#include <QString>
#include <QErrorMessage>

#include "misc.h"
#include "serverdialog.h"
#include "xclient.h"

bool connectXmms2(XClient *client, QWidget *parent)
{
	QSettings s;
	QString path;
	char *p = NULL;

browser:
	if (!getenv ("XMMS_PATH")) {
		ServerDialog sd (parent);
		if (!s.value ("serverdialog/show").toBool ()) {
			path = sd.get_default ();
		} else {
			path = sd.get_path ();
		}

		if (path == "local") {
			p = NULL;
		} else if (path.isNull ()) {
			return EXIT_FAILURE;
		} else {
			p = path.toAscii ().data ();
		}
	} else {
		p = getenv ("XMMS_PATH");
	}

	if (!client->connect (p, false, parent)) {
		if (!getenv ("XMMS_PATH")) {
			goto browser;
		} else {
			QErrorMessage *err = new QErrorMessage (parent);
			err->showMessage ("Your XMMS_PATH enviroment sucks. Fix it and restart the Application");
			err->exec ();
			return false;
		}
	}
	return true;
}
