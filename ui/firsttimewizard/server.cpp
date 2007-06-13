#include <QSystemTrayIcon>
#include <QSettings>

#include "server.h"

ServerPage::ServerPage(QWidget *parent)
: Page(parent)
{
	Ui::server::setupUi(this->Content);
}

/*
 * \brief this function needs to be implemented when a dialog
 * should save some settings which the user has configured.
 */
void ServerPage::saveSettings()
{
	QSettings s;
	QString sDefName, sTmp;
	QMap<QString, QVariant> m;

	sDefName = "default";
	sTmp == serverConPath->text();
	if(sTmp.isEmpty())
		sTmp = "local";
		
	m[sDefName] = QVariant(sTmp);
	s.setValue ("serverbrowser/default", sDefName);
	s.setValue ("serverbrowser/list", m);
}

void ServerPage::showEvent(QShowEvent *ev)
{
	parent->setWindowTitle("Server Connection Path Setting");
	this->header->setText("<b>Here you can set your server connection path.</b>");
}
