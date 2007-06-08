#include <QList>

#include "xclient.h"
#include "abstract_page.h"
#include "ftwdialog.h"

#ifndef _FTW_MANAGER_H_
#define _FTW_MANAGER_H_

class FTWManager : public QObject {
	Q_OBJECT
public:
	FTWManager(XClient *client_);
	void show();

protected slots:
	void lastPageFinished(bool Canceled);

signals:
	void settings_changed();

protected:
	void addPage(AbstractPage* p);

	FTWDialog *ftwDlg;
	XClient *client;
	AbstractPage* firstPage;
	AbstractPage* lastPage;
	QList<AbstractPage*> pages;
};
#endif
