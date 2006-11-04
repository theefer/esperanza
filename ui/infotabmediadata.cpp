#include "infotabmediadata.h"

#include <QGridLayout>
#include <QHash>
#include <QVariant>
#include <QHeaderView>
#include <QSettings>

InfoTabMediadata::InfoTabMediadata (QWidget *parent, XClient *client) :
	InfoWindowTab (parent, client)
{
	QGridLayout *g = new QGridLayout (this);
	m_table = new QTableWidget (this);
	g->setMargin (1);

	m_table->setColumnCount (2);
	QStringList l (tr ("Name"));
	l.append (tr ("Value"));

	m_table->setHorizontalHeaderLabels (l);
	m_table->verticalHeader ()->hide ();
	m_table->setVerticalScrollBarPolicy (Qt::ScrollBarAlwaysOn);

	m_table->horizontalHeader ()->setResizeMode (0, QHeaderView::Interactive);
	m_table->horizontalHeader ()->setClickable (false);

	QSettings s;
	m_table->horizontalHeader ()->resizeSection (0, s.value ("infotabmedia/section0", 70).toInt ());
	m_table->setSelectionMode (QAbstractItemView::NoSelection);
	m_table->horizontalHeader ()->setStretchLastSection (true);
	connect (m_table->horizontalHeader (),
			 SIGNAL (sectionResized (int, int, int)),
			 this, SLOT (head_size (int, int, int)));

	g->addWidget (m_table, 0, 0);
	setLayout (g);

	m_values.append ("title");
	m_values.append ("album");
	m_values.append ("artist");
	m_values.append ("bitrate");
	m_values.append ("comment");
	m_values.append ("date");
	m_values.append ("genre");
	m_values.append ("isvbr");
	m_values.append ("samplerate");
	m_values.append ("size");
	m_values.append ("tracknr");
	m_values.append ("timesplayed");
}

void
InfoTabMediadata::fill (uint32_t id)
{
	m_table->clearContents ();
	QHash<QString, QVariant> minfo;

	if (id != 0) {
		minfo = m_client->cache ()->get_info (id);

		QStringList keys = minfo.keys ();
		QStringList keys2;
		for (int j = 0; j < keys.count (); j++)
			if (m_values.contains (keys.at (j)))
				keys2.append (keys.at (j));

		keys = keys2;

		m_table->setRowCount (keys.count ());

		for (int i = 0; i < keys.count (); i ++) {
			QString k = keys.at (i);

			QString val = minfo[k].toString ();
			QTableWidgetItem *item = new QTableWidgetItem (k);
			item->setToolTip (val);
			m_table->setItem (i, 0, item);
			item = new QTableWidgetItem (minfo[k].toString ());
			item->setToolTip (val);
			m_table->setItem (i, 1, item);
		}

	} else {
		m_table->setRowCount (0);
	}
}

void
InfoTabMediadata::head_size (int c, int o, int n)
{
	if (c == 0) {
		QSettings s;
		s.setValue ("infotabmedia/section0", n);
	}
}

