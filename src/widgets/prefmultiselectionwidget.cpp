#include "prefmultiselectionwidget.h"
#include <QSettings>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QCheckBox>
#include <QDebug>

PrefMultiSelectionWidget::PrefMultiSelectionWidget (QWidget *parent, PreferenceValue *pref_)
: QWidget (parent)
{
	QSettings s;
	QString sValue;
	QVBoxLayout *vbox;
	QHBoxLayout *hbox;
	QCheckBox *cb;

	setupUi (this);

	pref = pref_;

	groupBox->setTitle (pref->help ());

	QMapIterator<QString, QVariant> iter (pref->data ().toMap ());
	sValue = s.value (pref->key (), pref->defval ()).toString ();
	vbox = new QVBoxLayout ();

	while (iter.hasNext ()) {
		iter.next ();
		hbox = new QHBoxLayout ();
		cb = new QCheckBox (groupBox);

		cb->setText (iter.key ());
		if (sValue.contains (iter.value ().toString ()))
			cb->setCheckState (Qt::Checked);
		else
			cb->setCheckState (Qt::Unchecked);
		hbox->addWidget (cb);

		if (iter.hasNext ())
		{
			iter.next ();
			cb = new QCheckBox (groupBox);

			cb->setText (iter.key ());
			if (sValue.contains (iter.value ().toString ()))
				cb->setCheckState (Qt::Checked);
			else
				cb->setCheckState (Qt::Unchecked);
			hbox->addWidget (cb);
		}
		vbox->addLayout (hbox);
	 }
	groupBox->setLayout (vbox);
}

QVariant PrefMultiSelectionWidget::getPrefValue ()
{
	bool first = true;
	QMap<QString, QVariant> range;
	QString ret, tmp;
	QCheckBox *cb;
	QLayoutItem *li;

	range = pref->data ().toMap ();

	for (int i = 0; i < groupBox->layout ()->count (); i++)
	{
		li = groupBox->layout ()->itemAt (i);

		for (int y = 0; y < li->layout ()->count (); y++)
		{
			cb = dynamic_cast<QCheckBox *> (li->layout ()->itemAt (y)->widget ());
			
			if (cb->checkState () == Qt::Checked)
			{
				if(!first)
					ret += ",";
				else
					first = false;
				ret += range[cb->text ()].toString ();
			}
		}
	}
	return QVariant (ret);
}
