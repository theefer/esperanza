#include "page.h"

#include "ui_gui.h"

#ifndef _GUI_H_
#define _GUI_H_

class GuiPage : public Page, public Ui::content {
public:
	GuiPage(QWidget *parent = NULL);
	void saveSettings();
};
#endif
