#include <QWidget>

#include "abstract_page.h"
#include "ui_welcome.h"

#ifndef _WELCOME_H_
#define _WELCOME_H_

class Welcome : public AbstractPage, public Ui::Welcome {
	Q_OBJECT

public:
	Welcome(QWidget * parent = NULL)
	: AbstractPage(parent)
	{
		setupUi(this);
	}
};
#endif
