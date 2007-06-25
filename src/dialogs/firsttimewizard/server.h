#include "page.h"

#include "ui_server.h"
#include "ftwdialog.h"

#ifndef _SERVER_H_
#define _SERVER_H_

class ServerPage : public Page, public Ui::server {
	Q_OBJECT;
public:
	ServerPage(FTWDialog *parent_ = NULL);
	void saveSettings();

public slots:
	void showEvent(QShowEvent *ev); 
	void tryit();
	void nextPage();

protected:
	FTWDialog *parent;
};
#endif
