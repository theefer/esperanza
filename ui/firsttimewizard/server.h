#include "page.h"

#include "ui_server.h"

#ifndef _SERVER_H_
#define _SERVER_H_

class ServerPage : public Page, public Ui::server {
public:
	ServerPage(QWidget *parent = NULL);
	void saveSettings();

public slots:
	void showEvent(QShowEvent *ev); 
};
#endif
