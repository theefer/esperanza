#ifndef __PREFERENCES_H__
#define __PREFERENCES_H__

#include "xclient.h"

#include <QMainWindow>
#include <QTableWidget>
#include <QHash>
#include <QMap>
#include <QVariant>
#include <QLabel>
#include <QDialog>
#include <QLabel>

#include "preferencessupport.h"

#define PREF_VALUE(v,h,t,d) { QMap<QString, QVariant> m; m["value"]=QVariant(v); m["help"]=QVariant(h); m["type"]=QVariant(t);m["default"]=QVariant(d);ret.append(m); }

class PreferenceDialog : public QMainWindow
{
	Q_OBJECT
	public:
		enum {
			T_BOOL,
			T_NUM,
			T_COLOR,
			T_STR,
			T_KEY
		};

		PreferenceDialog (QWidget *, XClient *);
		virtual QList < QMap < QString, QVariant > > build_prefvalues ();
		void fill_list ();
		void showEvent (QShowEvent *);

	private slots:
		void on_save ();
		void on_cancel ();
		void on_reset ();

	private:
		QTableWidget *m_table;
		XClient *m_client;
		QWidget *m_base;
};

#endif
