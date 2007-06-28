#ifndef _FTWDIALOG_H_
#define _FTWDIALOG_H_

#include <QDialog>

#include "abstract_page.h"
#include "ui_ftwdialog.h"

class FTWManager;

class FTWDialog : public QDialog, public Ui::FTWDialog {
public:
	FTWDialog(QWidget *parent, FTWManager *ftwman_)
	: QDialog(parent)
	{
		setupUi(this);
		ftwman = ftwman_;
		setModal(true);
	}

	FTWManager* manager() { return ftwman; };

protected:
	FTWManager *ftwman;
};
#endif
