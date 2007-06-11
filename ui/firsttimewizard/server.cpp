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
}

void ServerPage::showEvent(QShowEvent *ev)
{
	parent->setWindowTitle("Server Connection Path Setting");
	this->header->setText("<b>Here you can set your server connection path.</b>");
}
