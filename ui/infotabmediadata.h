#ifndef __INFOTAB_MEDIADATA_H__
#define __INFOTAB_MEDIADATA_H__

#include "infowindow.h"

#include <QWidget>
#include <QTableWidget>
#include <QStringList>

class InfoTabMediadata : public InfoWindowTab
{
	Q_OBJECT
	public:
		InfoTabMediadata (QWidget *, XClient *);
		void fill (uint32_t);

	private slots:
		void head_size (int, int, int);

	private:
		QTableWidget *m_table;
		QStringList m_values;
	
};

#endif

