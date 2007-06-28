#include <xmmsclient/xmmsclient++.h>

#include <QErrorMessage>
#include <QList>
#include <QDialog>

#include "xclient.h"
#include "abstract_page.h"
#include "ftwdialog.h"

#ifndef _FTW_MANAGER_H_
#define _FTW_MANAGER_H_

class FTWManager : public QObject {
	Q_OBJECT
public:
	FTWManager(QWidget *parent, XClient *client_);
	bool show();
	XClient* client() { return cli; };

protected slots:
	void lastPageFinished(bool Canceled);

signals:
	void settings_changed();

protected:
	void addPage(AbstractPage* p);

	FTWDialog *ftwDlg;
	XClient *cli;
	AbstractPage* firstPage;
	AbstractPage* lastPage;
	QList<AbstractPage*> pages;
	bool Canceled;
};
#endif
