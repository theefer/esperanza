#include "preferences.h"

#include <QMainWindow>
#include <QSettings>
#include <QGridLayout>
#include <QWidget>
#include <QSpinBox>
#include <QComboBox>
#include <QHeaderView>
#include <QPushButton>
#include <QColor>
#include <QLabel>
#include <QColorDialog>

#define PREF_VALUE(v,h,t,d) { QMap<QString, QVariant> m; m["value"]=QVariant(v); m["help"]=QVariant(h); m["type"]=QVariant(t);m["default"]=QVariant(d);ret.append(m); }

ColorButton::ColorButton (QWidget *parent, const QColor &val) : QLabel (parent)
{
	QPixmap p (80, 20);
	p.fill (val);
	setPixmap (p);
	m_color = val;
}

void
ColorButton::mouseDoubleClickEvent (QMouseEvent *ev)
{
	QColor col = QColorDialog::getColor (m_color);
	if (col.isValid ()) {
		QPixmap p (80, 20);
		p.fill (col);
		m_color = col;
		setPixmap (p);
	}
}

QColor
ColorButton::current_color () const
{
	return m_color;
}

QList < QMap < QString, QVariant > >
PreferenceDialog::build_prefvalues ()
{
	QList < QMap < QString, QVariant > > ret;

	PREF_VALUE("core/autostart", "Autostart xmms2d if not running", T_BOOL, true);
	PREF_VALUE("serverbrowser/show", "Show serverbrowser on startup", T_BOOL, true);
	PREF_VALUE("playlist/jumptocurrent", "Jump to current entry on song change", T_BOOL, true);
	PREF_VALUE("playlist/compactmode", "Use compact (boring) playlist mode", T_BOOL, false);
//	PREF_VALUE("playlist/fontsize", "Playlist fontsize in pixels", T_NUM, 10);
	PREF_VALUE("core/fontsize", "General fontsize in pixels", T_NUM, 10);
	PREF_VALUE("ui/highlight", "General highlight color", T_COLOR, QColor (80, 80, 80));
	PREF_VALUE("ui/highlightedtext", "Highlighted text color", T_COLOR, QColor (Qt::black));

	return ret;
}

PreferenceDialog::PreferenceDialog (QWidget *parent) : QMainWindow (parent)
{
	setWindowFlags (Qt::Dialog);
	setAttribute (Qt::WA_DeleteOnClose);
	QSettings s;

	QWidget *base = new QWidget (this);
	setCentralWidget (base);
	resize (400, 500);

	QGridLayout *g = new QGridLayout (base);
	
	QList < QMap < QString, QVariant > > pref = build_prefvalues ();

	m_table = new QTableWidget (base);
	m_table->setColumnCount (2);
	m_table->setRowCount (pref.size ());

	QStringList l (tr ("Name"));
	l.append (tr ("Value"));

	m_table->setHorizontalHeaderLabels (l);
	m_table->verticalHeader ()->hide ();
	m_table->horizontalHeader ()->setResizeMode (0, QHeaderView::Stretch);
	m_table->horizontalHeader ()->setClickable (false);
	m_table->horizontalHeader ()->resizeSection (1, 100);

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
					QComboBox *cb = new QComboBox (base);
					cb->addItem (tr ("Yes"), QVariant (true));
					cb->addItem (tr ("No"), QVariant (false));
					if (!s.value (val, def).toBool ())
						cb->setCurrentIndex (1);
					else
						cb->setCurrentIndex (0);
					m_table->setCellWidget (i, 1, cb);
					break;
				}
			case T_NUM:
				{
					QSpinBox *sp = new QSpinBox (base);
					sp->setValue (s.value (val, def).toInt ());
					m_table->setCellWidget (i, 1, sp);
					break;
				}
			case T_COLOR:
				{
					ColorButton *cb = new ColorButton (base, s.value (val, def).value<QColor> ());
					m_table->setCellWidget (i, 1, cb);
					break;
				}
			default:
				qDebug ("error!");
				break;
		}
	}

	g->addWidget (m_table, 0, 0, 1, 1);
	g->setColumnStretch (0, 1);
	g->setRowStretch (0, 1);

	QWidget *dummy = new QWidget (base);
	QHBoxLayout *hbox = new QHBoxLayout (dummy);
	hbox->addStretch (1);
	QPushButton *ok = new QPushButton (tr ("Save"), dummy);
	connect (ok, SIGNAL (clicked ()), this, SLOT (on_save ()));
	QPushButton *cancel = new QPushButton (tr ("Discard"), dummy);
	connect (cancel, SIGNAL (clicked ()), this, SLOT (on_cancel ()));

	hbox->addWidget (ok);
	hbox->addWidget (cancel);
	g->addWidget (dummy, 1, 0, 1, 1);
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
					QComboBox *cb = dynamic_cast<QComboBox *>(m_table->cellWidget (i, 1));
					ret = cb->itemData (cb->currentIndex ()).toBool ();
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
				}
		}
		s.setValue (m["value"].toString (), ret);
	}
}

void
PreferenceDialog::on_cancel ()
{
	close ();
}

