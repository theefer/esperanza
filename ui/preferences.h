#ifndef __PREFERENCES_H__
#define __PREFERENCES_H__

#include "xclient.h"

#include <QMainWindow>
#include <QTableWidget>
#include <QHash>
#include <QMap>
#include <QVariant>
#include <QLabel>

class ColorButton : public QLabel
{
	Q_OBJECT
	public:
		ColorButton (QWidget *, const QColor &);
		QColor current_color () const;
		void mouseDoubleClickEvent (QMouseEvent *);

	private:
		QColor m_color;
};

class PreferenceDialog : public QMainWindow
{
	Q_OBJECT
	public:
		enum {
			T_BOOL,
			T_NUM,
			T_COLOR
		};

		PreferenceDialog (QWidget *, XClient *);
		QList < QMap < QString, QVariant > > build_prefvalues ();

	private slots:
		void on_save ();
		void on_cancel ();
		void on_reset ();

	private:
		QTableWidget *m_table;
		XClient *m_client;

};

#endif
