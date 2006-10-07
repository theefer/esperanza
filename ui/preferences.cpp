#include "xclient.h"
#include "preferences.h"

#include <QMainWindow>
#include <QSettings>
#include <QGridLayout>
#include <QWidget>
#include <QSpinBox>
#include <QCheckBox>
#include <QHeaderView>
#include <QPushButton>
#include <QColor>
#include <QLabel>
#include <QColorDialog>
#include <QMessageBox>
#include <QApplication>
#include <QLineEdit>
#include <QKeyEvent>

QList < QMap < QString, QVariant > >
PreferenceDialog::build_prefvalues ()
{
	QList < QMap < QString, QVariant > > ret;

	PREF_VALUE("core/autostart", "Autostart xmms2d if not running", T_BOOL, true);
	PREF_VALUE("serverdialog/show", "Show serverbrowser on startup", T_BOOL, true);
	PREF_VALUE("playlist/jumptocurrent", "Jump to current entry on song change", T_BOOL, true);
	PREF_VALUE("playlist/compactmode", "Use compact (boring) playlist mode", T_BOOL, false);
	PREF_VALUE("ui/showstop", "Show a stop button as well", T_BOOL, false);
	PREF_VALUE("ui/reverseplaytime", "Use reversed elapsed time", T_BOOL, true);
//	PREF_VALUE("playlist/fontsize", "Playlist fontsize in pixels", T_NUM, 10);
	PREF_VALUE("ui/fontsize", "General fontsize in pixels", T_NUM, 10);
	PREF_VALUE("ui/highlight", "General highlight color", T_COLOR, QColor (80, 80, 80));
	PREF_VALUE("ui/highlightedtext", "Highlighted text color", T_COLOR, QColor (Qt::black));
	PREF_VALUE("ui/titlebartop", "Titlebar color gradient top", T_COLOR, QColor (200, 200, 200));
	PREF_VALUE("ui/titlebarbottom", "Titlebar color gradient bottom", T_COLOR, QColor (60, 60, 60));
	PREF_VALUE("ui/titlelighter", "Paint the progress in a lighter color", T_BOOL, false);
	PREF_VALUE("ui/titlebartext", "Titlebar text color", T_COLOR, QColor (Qt::black));
	PREF_VALUE("ui/currententry", "Color of the current playlist entry", T_COLOR, QColor (Qt::red));
	PREF_VALUE("core/usegrowl", "Use Growl notification", T_BOOL, false);
	PREF_VALUE("core/growladdress", "Growl notification address", T_STR, "127.0.0.1");

	return ret;
}

PreferenceDialog::PreferenceDialog (QWidget *parent, XClient *client) : QMainWindow (parent)
{
	setWindowFlags (Qt::Dialog);
	setAttribute (Qt::WA_DeleteOnClose);
	setWindowTitle ("Esperanza - Preferences");
	QSettings s;

	m_client = client;

	m_base = new QWidget (this);
	setCentralWidget (m_base);
	resize (400, 500);

	QGridLayout *g = new QGridLayout (m_base);

	m_table = new QTableWidget (m_base);
	m_table->setColumnCount (2);

	QStringList l (tr ("Name"));
	l.append (tr ("Value"));

	m_table->setHorizontalHeaderLabels (l);
	m_table->verticalHeader ()->hide ();
	m_table->horizontalHeader ()->setResizeMode (0, QHeaderView::Stretch);
	m_table->horizontalHeader ()->setClickable (false);
	m_table->horizontalHeader ()->resizeSection (1, 100);
	m_table->setSelectionMode (QAbstractItemView::NoSelection);

	g->addWidget (m_table, 0, 0, 1, 1);
	g->setColumnStretch (0, 1);
	g->setRowStretch (0, 1);

	QWidget *dummy = new QWidget (m_base);
	QHBoxLayout *hbox = new QHBoxLayout (dummy);
	hbox->addStretch (1);
	QPushButton *ok = new QPushButton (tr ("Save"), dummy);
	connect (ok, SIGNAL (clicked ()), this, SLOT (on_save ()));
	QPushButton *cancel = new QPushButton (tr ("Discard"), dummy);
	connect (cancel, SIGNAL (clicked ()), this, SLOT (on_cancel ()));
	QPushButton *reset = new QPushButton (tr ("Reset"), dummy);
	connect (reset, SIGNAL (clicked ()), this, SLOT (on_reset ()));

	hbox->addWidget (ok);
	hbox->addWidget (cancel);
	hbox->addWidget (reset);
	g->addWidget (dummy, 1, 0, 1, 1);

}

void
PreferenceDialog::showEvent (QShowEvent *ev)
{
	if (m_table->rowCount () < 1)
		fill_list ();
	QMainWindow::showEvent (ev);
}

void
PreferenceDialog::fill_list ()
{
	QSettings s;

	QList < QMap < QString, QVariant > > pref = build_prefvalues ();

	m_table->setRowCount (pref.size ());

	for (int i = 0; i < pref.size (); i ++) {
		QMap<QString, QVariant> m = pref.at (i);
		QString val = m["value"].toString ();
		QString help = m["help"].toString ();
		QVariant def = m["default"];
		int type = m["type"].toInt ();

		QTableWidgetItem *item = new QTableWidgetItem (help);
		item->setData (Qt::UserRole, m);
		
		m_table->setItem (i, 0, item);

		switch (type) {
			case T_BOOL:
				{
					QCheckBox *cb = new QCheckBox (this);
					if (s.value (val, def).toBool ())
						cb->setCheckState (Qt::Checked);
					else
						cb->setCheckState (Qt::Unchecked);
					m_table->setCellWidget (i, 1, cb);
					break;
				}
			case T_NUM:
				{
					QSpinBox *sp = new QSpinBox (m_base);
					sp->setValue (s.value (val, def).toInt ());
					m_table->setCellWidget (i, 1, sp);
					break;
				}
			case T_COLOR:
				{
					ColorButton *cb = new ColorButton (m_base, s.value (val, def).value<QColor> ());
					m_table->setCellWidget (i, 1, cb);
					break;
				}
			case T_STR:
				{
					QLineEdit *le = new QLineEdit (s.value (val, def).toString (), m_base);
					m_table->setCellWidget (i, 1, le);
					break;
				}
			case T_KEY:
				{
					KeyWidget *kw = new KeyWidget (m_base, s.value (val, def).toString ());
					m_table->setCellWidget (i, 1, kw);
					break;
				}
			default:
				qDebug ("error!");
				break;
		}
	}
}

void
PreferenceDialog::on_save ()
{
	QSettings s;
	for (int i = 0; i < m_table->rowCount (); i++) {
		QTableWidgetItem *item = m_table->item (i, 0);
		QMap<QString, QVariant> m = item->data (Qt::UserRole).toMap ();
		QVariant ret;
		switch (m["type"].toInt ()) {
			case T_BOOL:
				{
					QCheckBox *cb = dynamic_cast<QCheckBox *>(m_table->cellWidget (i, 1));
					ret = QVariant (cb->checkState () == Qt::Checked ? true : false);
					break;
				}
			case T_NUM:
				{
					QSpinBox *sp = dynamic_cast<QSpinBox *>(m_table->cellWidget (i, 1));
					ret = QVariant (sp->value ());
					break;
				}
			case T_COLOR:
				{
					ColorButton *cb = dynamic_cast<ColorButton *>(m_table->cellWidget (i, 1));
					ret = QVariant (cb->current_color ());
					break;
				}
			case T_STR:
				{
					QLineEdit *le = dynamic_cast<QLineEdit *>(m_table->cellWidget (i, 1));
					ret = QVariant (le->text ());
					break;
				}
			case T_KEY:
				{
					KeyWidget *kw = dynamic_cast<KeyWidget *>(m_table->cellWidget (i, 1));
					qDebug ("saving %s", qPrintable (kw->get_key ()));
					ret = QVariant (kw->get_key ());
					break;
				}
		}
		s.setValue (m["value"].toString (), ret);
	}

	m_client->settings ()->change_settings ();

	close ();
	s.sync ();
}

void
PreferenceDialog::on_cancel ()
{
	close ();
}

void
PreferenceDialog::on_reset ()
{
	QSettings s;

	if (QMessageBox::question (this,
							   tr ("Reset settings?"),
							   tr ("Are your sure you want to reset program to standard values?"),
							   tr ("Yes"), tr ("No")) == 0) {
		QStringList l = s.allKeys ();
		for (int i = 0; i < l.count (); i++) {
			s.remove (l.at (i));
		}
	}

	close ();
}
