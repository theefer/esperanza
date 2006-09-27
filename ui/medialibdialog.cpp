#include "medialibdialog.h"
#include "medialibsearchmodel.h"

#include "playerbutton.h"
#include "progressindicator.h"

#include <QLabel>
#include <QSettings>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QSettings>
#include <QResizeEvent>
#include <QMainWindow>

MedialibDialog::MedialibDialog (QWidget *parent, XClient *client) : QMainWindow (parent)
{
	QSettings s;

	setWindowFlags (Qt::Dialog);
	setAttribute (Qt::WA_DeleteOnClose);

	QWidget *base = new QWidget (this);
	setCentralWidget (base);
	
	QGridLayout *g = new QGridLayout (base);

	m_client = client;

	m_qb = new QComboBox (base);
	m_qb->addItem (tr ("All"), MedialibSearchModel::SEARCH_ALL);
	m_qb->addItem (tr ("Artist"), MedialibSearchModel::SEARCH_ARTIST);
	m_qb->addItem (tr ("Album"), MedialibSearchModel::SEARCH_ALBUM);
	m_qb->addItem (tr ("Title"), MedialibSearchModel::SEARCH_TITLE);
	m_qb->addItem (tr ("Year"), MedialibSearchModel::SEARCH_YEAR);

	m_qb->setCurrentIndex (s.value ("medialib/searchdef", 0).toInt ());
	
	g->addWidget (m_qb, 0, 0, 1, 1);

	QLabel *l = new QLabel (tr ("="), base);
	g->addWidget (l, 0, 1, 1, 1);

	m_le = new QLineEdit (base);
	m_le->setFocus (Qt::OtherFocusReason);
	m_le->setFocusPolicy (Qt::StrongFocus);
	connect (m_le, SIGNAL (returnPressed ()), this, SLOT (do_search ()));

	g->addWidget (m_le, 0, 2, 1, 1);
	g->setColumnStretch (2, 1);

	m_indicator = new ProgressIndicator (base);
	g->addWidget (m_indicator, 0, 3, 1, 1);

	m_list = new MedialibView (base, client);
	g->addWidget (m_list, 1, 0, 1, 4);

	QWidget *dummy = new QWidget (base);
	QHBoxLayout *hbox = new QHBoxLayout (dummy);

	g->setRowStretch (1, 1);

	hbox->addStretch (1);

	PlayerButton *plus = new PlayerButton (dummy, ":images/plus.png");
	connect (plus, SIGNAL (clicked (QMouseEvent *)),
			 this, SLOT (plus_pressed (QMouseEvent *)));

	PlayerButton *stop = new PlayerButton (dummy, ":images/stop.png");
	connect (stop, SIGNAL (clicked (QMouseEvent *)),
			 this, SLOT (close ()));


	hbox->addWidget (plus);
	hbox->addWidget (stop);
	g->addWidget (dummy, 2, 0, 1, 4);
	g->setMargin (1);

	resize (s.value ("medialibdialog/size", QSize (500, 350)).toSize ());
	if (s.contains ("medialibdialog/position"))
		move (s.value ("medialibdialog/position").toPoint ());

	connect (m_list, SIGNAL (searchDone ()), this, SLOT (search_done ()));
}

void
MedialibDialog::plus_pressed (QMouseEvent *ev)
{
	QList<uint32_t> ids = m_list->get_selection ();
	
	if (ids.size () == 0) {
		ids = m_list->get_all ();
	}

	for (int i = 0; i < ids.size (); i++) {
		m_client->playlist.addId (ids.at (i), &XClient::log);
	}
}

void
MedialibDialog::search_done ()
{
	m_le->setEnabled (true);
	m_indicator->setStatus (false);
	m_le->setFocus (Qt::OtherFocusReason);
}

void
MedialibDialog::do_search ()
{
	QSettings s;

	s.setValue ("medialib/searchdef", m_qb->currentIndex ());

	m_list->do_search (m_qb->itemData (m_qb->currentIndex ()).toUInt (),
					   m_le->displayText ());
	m_le->setEnabled (false);
	m_indicator->setStatus (true);
}

void
MedialibDialog::resizeEvent (QResizeEvent *ev)
{
	QSettings s;
	s.setValue ("medialibdialog/size", ev->size ());
}

void
MedialibDialog::moveEvent (QMoveEvent *ev)
{
	QSettings s;
	s.setValue ("medialibdialog/position", pos ());
}

