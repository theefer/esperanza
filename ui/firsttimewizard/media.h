#ifndef _MEDIA_H_
#define _MEDIA_H_

#include "page.h"
#include "ftwdialog.h"

#include "ui_media.h"

class MediaPage : public Page, public Ui::media {
	Q_OBJECT;
public:
	MediaPage(FTWDialog *parent = NULL);
	void saveSettings();

public slots:
	void showEvent(QShowEvent *ev);
	void importFile();
	void importFolder();
protected:
	FTWDialog *par;
};
#endif
