#ifndef __INFOWINDOW_H__
#define __INFOWINDOW_H__

#include "xclient.h"

#include <QWidget>
#include <QTabWidget>
#include <QLabel>
#include <QUrl>
#include <QTextBrowser>
#include <QTextDocument>

class InfoBrowser : public QTextBrowser
{
	Q_OBJECT
	public:
		InfoBrowser (QWidget *parent);
		void setSource (const QUrl &name);
};

class InfoWindow : public QWidget
{
	Q_OBJECT

	public:
		InfoWindow (QWidget *, XClient *);

	private slots:
		void set_current_id (uint32_t id);
		void entry_changed (uint32_t);
		void change_current (int);
		void link_clicked (const QUrl &);

	private:
		QTextDocument *m_doc;
		XClient *m_client;
		QTabWidget *m_tab;
		QLabel *m_label;

		uint32_t m_current_id;
};

#endif
