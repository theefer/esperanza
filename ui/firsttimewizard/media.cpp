#include <xmmsclient/xmmsclient++.h>

#include <QSystemTrayIcon>
#include <QFileDialog>
#include <QSettings>
#include <QMessageBox>

#include "media.h"
#include "xclient.h"

#include "ftwmanager.h"

MediaPage::MediaPage(FTWDialog *parent)
: Page(parent)
{
	Ui::media::setupUi(this->Content);
    connect(Ui::media::importFile, SIGNAL(clicked()), SLOT(importFile()));
    connect(Ui::media::importFolder, SIGNAL(clicked()), SLOT(importFolder()));
	par = parent;
}

/*
 * \brief this function needs to be implemented when a dialog
 * should save some settings which the user has configured.
 */
void MediaPage::saveSettings()
{
	QSettings s;
}

void MediaPage::showEvent(QShowEvent *ev)
{
	parent->setWindowTitle("Import some mediafiles");
	this->header->setText("<b>If you already have some mediafiles,<br />import them here.</b>");
}

void MediaPage::importFile()
{
	QStringList files = QFileDialog::getOpenFileNames (this, "Add files");

	try {
		for (int i = 0; i < files.count(); i++) {
			QString s = "file://" + files.at (i);
			qDebug(qPrintable(QString("trying to import file: '%1'").arg(s)));
			par->manager()->client()->medialib.addEntry(XClient::qToStd (s)) ();
		}
	} catch (...) {
		QMessageBox::critical(this, tr("Error occurred"), tr("There was an error while importing the files."));
		return;
	}
	QMessageBox::information(this, tr("Success"), tr("Successfully imported the files"));
}

void MediaPage::importFolder()
{
	QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                 "", QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);
	if (!dir.isEmpty())
		try {
			qDebug(qPrintable(QString("trying to import directory: '%1'").arg(dir)));
			par->manager()->client()->medialib.pathImport(XClient::qToStd("file://" + dir)) ();
		} catch (...) {
			QMessageBox::critical(this, tr("Error occurred"), tr("There was an error while importing the directory."));
			return;
		}
	QMessageBox::information(this, tr("Success"), tr("Successfully imported the directory."));
}
