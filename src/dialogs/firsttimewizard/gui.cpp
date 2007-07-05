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

#include <QSystemTrayIcon>
#include <QSettings>

#include "gui.h"

GuiPage::GuiPage(QWidget *parent)
: Page(parent)
{
	Ui::content::setupUi(this->Content);
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
	if(bTmp) {
		bTmp = !((bool)doQuitOnClose->checkState() == Qt::Checked);
		s.setValue("ui/hideOnClose", QVariant(bTmp));

		iTmp = (singleOrDoubleClick->checkState() == Qt::Checked) ?
						QSystemTrayIcon::DoubleClick : QSystemTrayIcon::Trigger;
		s.setValue("ui/activateTray", QVariant(iTmp));
	}
}

void GuiPage::showEvent(QShowEvent *ev)
{
	parent->setWindowTitle("Some Interfacesettings");
	this->header->setText("<b>Here you can set some esperanza interface settings.</b>");
}
