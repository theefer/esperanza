#include <QSystemTrayIcon>
#include <QSettings>

#include "gui.h"

GuiPage::GuiPage(QWidget *parent)
: Page(parent)
{
	Ui::content::setupUi(this->Content);
	// QSystemTrayIcon::DoubleClick == DoubleClick
	// QSystemTrayIcon::Trigger == SingleClick
}

/*
 * \brief this function needs to be implemented when a dialog
 * should save some settings which the user has configured.
 */
void GuiPage::saveSettings()
{
	QSettings s;
	bool bTmp;
	int iTmp;

	bTmp = (bool)(IgnoreDesktopSettings->checkState() == Qt::Checked);
	s.setValue("core/ignoredesktopsettings", QVariant(bTmp));

	bTmp = (bool)(showServerBrowser->checkState() == Qt::Checked);
	s.setValue("serverdialog/show", QVariant(bTmp));

	bTmp = (bool)groupTray->isChecked();
	s.setValue("core/systray", QVariant(bTmp));

	bTmp = (bool)singleOrDoubleClick->checkState() == Qt::Checked;
	s.setValue("ui/activateTray", QVariant(bTmp));

	iTmp = ((bool)doQuitOnClose->checkState() == Qt::Checked) ?
					QSystemTrayIcon::DoubleClick : QSystemTrayIcon::Trigger;
	s.setValue("ui/hideOnClose", QVariant(iTmp));
}
