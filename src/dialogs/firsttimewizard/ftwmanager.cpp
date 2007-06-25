#include "ftwmanager.h"

// pages
#include "welcome.h"
#include "server.h"
#include "media.h"
#include "gui.h"
#include "finish.h"

FTWManager::FTWManager(XClient *client_)
{
	cli = client_;
	firstPage = NULL;
	lastPage = NULL;
	ftwDlg = new FTWDialog(this);
	addPage(new Welcome(ftwDlg));
	addPage(new ServerPage(ftwDlg));
	addPage(new MediaPage(ftwDlg));
	addPage(new GuiPage(ftwDlg));
	addPage(new Finish(ftwDlg));
	Canceled = true;
}

void FTWManager::addPage(AbstractPage *p)
{
	if(!firstPage)
		firstPage = p;
	else
		lastPage->setNext(p);
	p->setPrevious(lastPage);
	lastPage = p;

	connect(p, SIGNAL(lastClosed(bool)), this, SLOT(lastPageFinished(bool)));
	pages.append(p);
	p->hide();
}

bool FTWManager::show()
{
	if(firstPage) {
		firstPage->show();
		ftwDlg->exec();
	}
	return !Canceled;
}

void FTWManager::lastPageFinished(bool Canceled_)
{
	AbstractPage *p;
	Canceled = Canceled_;

	if(!Canceled)
		while (!pages.isEmpty()) {
			p = pages.takeFirst();
			p->saveSettings();
		}
	ftwDlg->close();
}
