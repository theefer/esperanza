#include <QApplication>
#include <QWidget>
#include <QMessageBox>

#include "abstract_page.h"
#include "ui_finish.h"

#ifndef _FINISH_H_
#define _FINISH_H_

class Finish : public AbstractPage, public Ui::Finish {
	Q_OBJECT

public:
	Finish(QWidget * parent = NULL)
	: AbstractPage(parent)
	{
		setupUi(this);
	}
};
#endif
