#include "abstract_page.h"

#include "ui_page.h"

#ifndef _PAGE_H_
#define _PAGE_H_
class Page : public AbstractPage, public Ui::DefaultPage {
	Q_OBJECT
public:
	Page(QWidget *parent = NULL);
};
#endif
