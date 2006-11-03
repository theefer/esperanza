#ifndef __INFOWINDOW_H__
#define __INFOWINDOW_H__

#include "xclient.h"

#include <QWidget>
#include <QTabWidget>
#include <QLabel>

class InfoWindowTab : public QWidget
{
	Q_OBJECT
	public:
		InfoWindowTab (QWidget *parent, XClient *client) : QWidget (parent) {
			m_client = client;
		};
		virtual void fill (uint32_t) { };

	protected:
		XClient *m_client;

};

class InfoWindow : public QWidget
{
	Q_OBJECT

	public:
		InfoWindow (QWidget *, XClient *);

	private slots:
		void set_current_id (uint32_t id);
		void entry_changed (uint32_t);

	private:
		XClient *m_client;
		QTabWidget *m_tab;
		QLabel *m_label;

		uint32_t m_current_id;
};

#endif
