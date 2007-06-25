#ifndef _FTWDIALOG_H_
#define _FTWDIALOG_H_

#include <QDialog>

#include "abstract_page.h"
#include "ui_ftwdialog.h"

class FTWManager;

class FTWDialog : public QDialog, public Ui::FTWDialog {
public:
	FTWDialog(FTWManager *ftwman_) {
		setupUi(this);
		ftwman = ftwman_;
	}

	FTWManager* manager() { return ftwman; };

protected:
	FTWManager *ftwman;
};
#endif
