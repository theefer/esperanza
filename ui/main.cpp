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

int
main (int argc, char **argv)
{
	QApplication app(argc, argv);
	QApplication::setDesktopSettingsAware (false);

	QCoreApplication::setOrganizationName("XMMS2");
	QCoreApplication::setOrganizationDomain("xmms.org");
	QCoreApplication::setApplicationName("simpleqt");

	QFont f = app.font ();
	f.setPixelSize (10);
	QApplication::setFont (f);

	QSettings s;
	/* base palette */
	QPalette p (app.palette ());
	p.setColor (QPalette::Highlight,
				s.value ("ui/highlight", QColor (80, 80, 80)).value<QColor> ());
	p.setColor (QPalette::HighlightedText,
				s.value ("ui/highlightedtext", QColor (Qt::black)).value<QColor> ());
	p.setColor (QPalette::Inactive, QPalette::Text, QColor (Qt::black));
	QApplication::setPalette (p);

	XClient client (NULL, "SimpleQt");
	PlayerWidget *pw = new PlayerWidget (NULL, &client);

	client.connect (getenv ("XMMS_PATH") ? getenv ("XMMS_PATH") : "");

	pw->show ();

	return app.exec ();
}

