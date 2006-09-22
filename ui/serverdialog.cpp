#include "serverdialog.h"

#include <QDialog>
#include <QGridLayout>
#include <QMap>
#include <QSettings>
#include <QLabel>
#include <QListWidgetItem>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QErrorMessage>

#include "playerbutton.h"

ServerDialog::ServerDialog (QWidget *parent) : QDialog (parent)
{
	QGridLayout *g = new QGridLayout (this);

	QLabel *l = new QLabel (tr ("Welcome to an XMMS2 client\nPlease select the server to connect to.\nIt's possible to disable this dialog in preferences"), this);
	l->setAlignment (Qt::AlignCenter);
	g->addWidget (l, 0, 0, 1, 2);
	g->setRowStretch (0, 0);

	m_list = new QListWidget (this);
	m_list->setSelectionMode (QAbstractItemView::SingleSelection);

	QMap <QString, QVariant> def;
	def["local"] = QString ("local");

	QSettings s;
	QMap<QString, QVariant> m = s.value ("serverbrowser/list", def).toMap ();

	QString defconn = s.value ("serverbrowser/default", "local").toString ();

	QList<QString> lst = m.keys ();
	for (int i = 0; i < lst.size (); i ++) {
		QListWidgetItem *q = new QListWidgetItem (lst.at (i), m_list);
		q->setData (Qt::ToolTipRole, m[lst.at (i)]);
		if (lst.at (i) == defconn)
			m_list->setCurrentItem (q);
	}

	g->addWidget (m_list, 1, 0, 2, 2);
	g->setRowStretch (1, 1);
	g->setMargin (1);

	QWidget *dummy = new QWidget (this);
	QHBoxLayout *hbox = new QHBoxLayout (dummy);

	PlayerButton *c = new PlayerButton (dummy, tr ("connect"));
	connect (c, SIGNAL(clicked (QMouseEvent *)), this, SLOT (accept ()));
	hbox->addWidget (c);

	hbox->addStretch (1);

	PlayerButton *b = new PlayerButton (dummy, ":images/plus.png");
	connect (b, SIGNAL (clicked (QMouseEvent *)), this, SLOT (add_server ()));
	hbox->addWidget (b);

	PlayerButton *b2 = new PlayerButton (dummy, ":images/minus.png");
	connect (b2, SIGNAL (clicked (QMouseEvent *)), this, SLOT (remove_server ()));
	hbox->addWidget (b2);

	g->addWidget (dummy, 3, 0, 1, 2);

	connect (m_list, SIGNAL (itemDoubleClicked (QListWidgetItem *)), this, SLOT (accept ()));

	resize (250, 300);
}

AddServerDialog::AddServerDialog (QWidget *parent)
{
	QGridLayout *g = new QGridLayout (this);
	QLabel *l = new QLabel (tr ("Adding new server"), this);
	g->addWidget (l, 0, 0, 1, 2);

	l = new QLabel (tr ("Server name"), this);
	g->addWidget (l, 1, 0, 1, 1);
	m_name = new QLineEdit (this);
	g->addWidget (m_name, 1, 1, 1, 1);

	l = new QLabel (tr ("Server path"), this);
	g->addWidget (l, 2, 0, 1, 1);
	m_path = new QLineEdit (this);
	g->addWidget (m_path, 2, 1, 1, 1);

	QPushButton *ok = new QPushButton (tr ("Add"), this);
	connect (ok, SIGNAL (clicked ()), this, SLOT (accept ()));
	QPushButton *cancel = new QPushButton (tr ("Cancel"), this);
	connect (cancel, SIGNAL (clicked ()), this, SLOT (reject ()));
	g->addWidget (ok, 3, 0, 1, 1);
	g->addWidget (cancel, 3, 1, 1, 1);
}

void
ServerDialog::add_server ()
{
	AddServerDialog add (this);
	QListWidgetItem *item;
	if (add.exec () == QDialog::Accepted) {
		for (int i = 0; i < m_list->count (); i++) {
			item = m_list->item (i);
			if (item->text () == add.m_name->text ()) {
				QErrorMessage *err = new QErrorMessage (this);
				err->showMessage (tr ("Dang! That name is already taken!"));
				err->exec ();
				return;
			}
		}
		item = new QListWidgetItem (add.m_name->text (), m_list);
		item->setData (Qt::ToolTipRole, add.m_path->text ());
		m_list->setCurrentItem (item);
	}
}

void
ServerDialog::remove_server ()
{
	QListWidgetItem *item = m_list->currentItem ();
	if (item->text () == "local") {
		QErrorMessage *err = new QErrorMessage (this);
		err->showMessage (tr ("Eeep! Can't remove local item!"));
		err->exec ();
		return;
	}

	delete item;
}

QString
ServerDialog::get_default ()
{
	QSettings s;
	QMap<QString, QVariant> m = s.value ("serverbrowser/list").toMap ();
	QString defconn = s.value ("serverbrowser/default", "local").toString ();

	QList<QString> lst = m.keys ();
	for (int i = 0; i < lst.size (); i ++) {
		if (lst.at (i) == defconn)
			return m[lst.at (i)].toString ();
	}

	return QString ();
}

QString
ServerDialog::get_path ()
{
	QSettings s;
	int ret = exec ();

	QMap<QString, QVariant> m;
	for (int i = 0; i < m_list->count (); i ++) {
		QListWidgetItem *item = m_list->item (i);
		m[item->text ()] = QVariant (item->toolTip ());
	}

	s.setValue ("serverbrowser/list", m);
	QListWidgetItem *d = m_list->currentItem ();
	if (d) {
		s.setValue ("serverbrowser/default", d->text ());
	}

	if (ret == QDialog::Accepted) {
		return d->toolTip ();
	}

	return QString ();
}
