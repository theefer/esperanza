#include "prefselectionwidget.h"
#include <QSettings>

PrefSelectionWidget::PrefSelectionWidget (QWidget *parent, PreferenceValue *pref)
: QWidget (parent)
{
	QSettings s;
	setupUi (this);
	label->setText (pref->help ()); 
	setMap (pref->data ().toMap (), s.value (pref->key (), pref->defval ()).toString ());
}

void PrefSelectionWidget::setMap (QMap<QString, QVariant> map, QVariant current)
{
	QMapIterator<QString, QVariant> iter(map);

	while (iter.hasNext ()) {
		iter.next ();
		comboBox->addItem (iter.key (), QVariant (iter.value ()));
		if (current.isValid () && current == iter.value ())
			comboBox->setCurrentIndex (comboBox->count () - 1);
	}
}
