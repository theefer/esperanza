#include "xclient.h"
#include "infowindow.h"

#include <QWidget>
#include <QTabWidget>
#include <QGridLayout>


#include "infotabmediadata.h"
#include "infotabart.h"

InfoWindow::InfoWindow (QWidget *parent, XClient *client) : QWidget (parent)
{
	setWindowFlags (Qt::Drawer);
	setWindowTitle (tr ("Esperanza - Info"));

	m_client = client;

	QGridLayout *g = new QGridLayout (this);

	m_label = new QLabel (this);
	m_label->setText (tr ("Nothing selected"));
	m_label->setAlignment (Qt::AlignCenter);
	g->addWidget (m_label, 0, 0);

	m_tab = new QTabWidget (this);
	m_tab->addTab (new InfoTabMediadata (this, m_client), tr ("Metadata"));
	m_tab->addTab (new InfoTabArt (this, m_client), tr ("Art"));
	connect (m_tab, SIGNAL (currentChanged (int)), this, SLOT (change_current (int)));

//	m_tab->addTab (new QWidget (), tr ("Wikipedia"));

	g->addWidget (m_tab, 1, 0);

	setLayout (g);
	resize (300, 200);

//	setMargin (0);
	g->setMargin (0);

	connect (m_client->cache (), SIGNAL (entryChanged (uint32_t)),
			 this, SLOT (entry_changed (uint32_t)));
}

void
InfoWindow::change_current (int c)
{
	set_current_id (m_current_id);
}

void
InfoWindow::entry_changed (uint32_t id)
{
	if (id == m_current_id) {
		set_current_id (id);
	}
}

void
InfoWindow::set_current_id (uint32_t id)
{
	QHash<QString, QVariant> minfo;
	m_current_id = id;

	if (id != 0) {
		minfo = m_client->cache ()->get_info (id);
		QString s = QString ("%2 - %3")
			.arg(minfo["artist"].toString ())
			.arg(minfo["title"].toString ());
		m_label->setText (s);
	} else {
		m_label->setText (tr ("Invalid selection"));
	}

	InfoWindowTab *t = dynamic_cast<InfoWindowTab*> (m_tab->currentWidget ());
	if (t) {
		t->fill (id);
	}
}

