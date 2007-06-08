#include <QDialog>

#include "abstract_page.h"
#include "ui_ftwdialog.h"

#ifndef _FTWDIALOG_H_
#define _FTWDIALOG_H_
class FTWDialog : public QDialog, public Ui::FTWDialog {
public:
	FTWDialog() {
		setupUi(this);
	}
};
#endif
