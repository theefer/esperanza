/**
 *  This file is a part of Esperanza, an XMMS2 Client.
 *
 *  Copyright (C) 2005-2006 XMMS2 Team
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version 2
 *  of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 */


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
#include "minimode.h"
#include "serverdialog.h"
#include "preferences.h"

/*
Q_IMPORT_PLUGIN(lastfm)
Q_IMPORT_PLUGIN(aboutdialog)
Q_IMPORT_PLUGIN(medialibdialog)
Q_IMPORT_PLUGIN(streamingdialog)
*/

int
main (int argc, char **argv)
{
    QCoreApplication::setOrganizationName ("xmms2");
	QCoreApplication::setOrganizationDomain ("xmms.org");
	QCoreApplication::setApplicationName ("Esperanza");

	Q_INIT_RESOURCE(esperanza);

    QSettings s;

    if (s.value("core/ignoredesktopsettings", false).toBool ()) {
        QApplication::setDesktopSettingsAware (false);
    }

	QApplication app(argc, argv);

#if QT_VERSION >= 0x040200 && QT_VERSION < 0x040300
	if (app.style ()->objectName () == "motif") {
	    qWarning ("Qt 4.2 is running and the Motif style is selected. We'll switch to plastique so you won't hate us.");
	    app.setStyle(new QPlastiqueStyle());
	}
#endif

	QApplication::setWindowIcon (QIcon (":images/esperanza.png"));

	QString locale = QLocale::system ().name ();

	PreferenceDialog::save_defaults ();

	QTranslator trans;
	if (!s.value ("core/skiplocales").toBool ()) {
		trans.load (QString ("esperanza_") + locale, ":translations");
		app.installTranslator (&trans);
	}

	XClient client (NULL, "Esperanza");

	QString path;

	new PlayerWidget (NULL, &client);

	char *p = NULL;

browser:
	if (!getenv ("XMMS_PATH")) {
		ServerDialog sd (NULL);
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

	if (!client.connect (p)) {
		if (!getenv ("XMMS_PATH")) {
			goto browser;
		} else {
			QErrorMessage *err = new QErrorMessage (NULL);
			err->showMessage ("Your XMMS_PATH enviroment sucks. Fix it and restart the Application");
			err->exec ();
			exit (EXIT_FAILURE);
		}
	}
	
	return app.exec ();
}

