/**
 *  This file is a part of Esperanza, an XMMS2 Client.
 *
 *  Copyright (C) 2005-2007 XMMS2 Team
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
#include "firsttimewizard/ftwmanager.h"
#include "misc.h"

#include "preferencemanager.h"

Q_IMPORT_PLUGIN(lastfm)
Q_IMPORT_PLUGIN(medialibdialog)
Q_IMPORT_PLUGIN(streamingdialog)
Q_IMPORT_PLUGIN(aboutdialog)
Q_IMPORT_PLUGIN(preferencesdialog)
Q_IMPORT_PLUGIN(xmms2dpreferences)

int
main (int argc, char **argv)
{
	int ret;
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

	/* create the preference manager */
	PreferenceManager::instance ();

	QTranslator trans;
	if (!s.value ("core/skiplocales").toBool ()) {
		trans.load (QString ("esperanza_") + locale, ":translations");
		app.installTranslator (&trans);
	}

	XClient client (NULL, "Esperanza");

	PlayerWidget *pw = new PlayerWidget (NULL, &client);

	if (!s.value("core/firsttimewizardshowen", QVariant(false)).toBool ())
	{
		FTWManager *ftwMan =  new FTWManager (pw, &client);
		ftwMan->show();
		s.setValue ("core/firsttimewizardshowen", QVariant(true));
	}

	if (!client.isConnected ())
		connectXmms2 (&client, pw);

	// SHOWS THE PLAYERWINDOW respectivly minimode window
	client.settings ()->change_settings ();
	ret = app.exec ();
	delete pw;
	return ret;
}

