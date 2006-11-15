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
#include <QCheckBox>
#include <QCompleter>

MedialibDialog::MedialibDialog (QWidget *parent, XClient *client) : QMainWindow (parent)
{
	QSettings s;

	setWindowFlags (Qt::Dialog);
	setAttribute (Qt::WA_DeleteOnClose);
	setWindowTitle ("Esperanza - Medialib");

	QWidget *base = new QWidget (this);
	setCentralWidget (base);
	
	QGridLayout *g = new QGridLayout (base);

	m_client = client;
	m_completer = NULL;

	m_qb = new QComboBox (base);
	m_qb->addItem (tr ("All"), MedialibSearchModel::SEARCH_ALL);
	m_qb->addItem (tr ("Artist"), MedialibSearchModel::SEARCH_ARTIST);
	m_qb->addItem (tr ("Album"), MedialibSearchModel::SEARCH_ALBUM);
	m_qb->addItem (tr ("Title"), MedialibSearchModel::SEARCH_TITLE);
	m_qb->addItem (tr ("Year"), MedialibSearchModel::SEARCH_YEAR);

	m_qb->setCurrentIndex (s.value ("medialib/searchdef", 0).toInt ());
	connect (m_qb, SIGNAL (currentIndexChanged (int)), this, SLOT (load_compl_list (int)));
	
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

	m_cb = new QCheckBox (tr ("Display non-available entries"), dummy);
	hbox->addWidget (m_cb);
	connect (m_cb, SIGNAL (stateChanged (int)), this, SLOT (do_search ()));

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

	connect (m_list, SIGNAL (searchDone ()), this, SLOT (search_done ()));

	load_compl_list (m_qb->currentIndex ());
}

void
MedialibDialog::load_compl_list (int i)
{
	int t = m_qb->itemData (i).toInt ();

	QString q ("select distinct value from Media where key=");
	if (t == MedialibSearchModel::SEARCH_ARTIST) {
		q.append ("'artist'");
	} else if (t == MedialibSearchModel::SEARCH_ALBUM) {
		q.append ("'album'");
	} else if (t == MedialibSearchModel::SEARCH_YEAR) {
		q.append ("'date'");
	} else if (t == MedialibSearchModel::SEARCH_TITLE) {
		q.append ("'title'");
	} else if (t == MedialibSearchModel::SEARCH_ALL) {
		q.append ("'artist' or key = 'album'");
	}
	q.append (" order by lower (value)");

	m_client->medialib.select (q.toStdString (), Xmms::bind (&MedialibDialog::compl_reply, this));

	m_le->setEnabled (false);
	m_qb->setEnabled (false);
	m_cb->setEnabled (false);
	m_indicator->setStatus (true);
}

bool
MedialibDialog::compl_reply (const Xmms::List <Xmms::Dict> &list)
{
	QStringList compl_list;

	for (list.first (); list.isValid (); ++ list) {
		compl_list.append (QString::fromStdString ((*list).get<std::string> ("value")));
	}

	if (m_completer) {
		delete m_completer;
	}

	m_completer = new QCompleter (compl_list, this);
	m_completer->setCaseSensitivity (Qt::CaseInsensitive);
	m_le->setCompleter (m_completer);

	search_done ();

	return true;
}

void
MedialibDialog::keyPressEvent (QKeyEvent *ev)
{
	if (ev->key () == Qt::Key_Escape)
		close ();
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
	m_cb->setEnabled (true);
	m_qb->setEnabled (true);
}

void
MedialibDialog::do_search ()
{
	QSettings s;

	s.setValue ("medialib/searchdef", m_qb->currentIndex ());

	m_list->do_search (m_qb->itemData (m_qb->currentIndex ()).toUInt (),
					   m_le->displayText (), m_cb->checkState () == Qt::Checked);
	m_le->setEnabled (false);
	m_cb->setEnabled (false);
	m_qb->setEnabled (false);
	m_indicator->setStatus (true);
}

void
MedialibDialog::resizeEvent (QResizeEvent *ev)
{
	QSettings s;
	s.setValue ("medialibdialog/size", ev->size ());
}

