/** 
 *  This file is a part of Esperanza, an XMMS2 Client.
 *
 *  Copyright (C) 2007 XMMS2 Team
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version 2
 *  of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 */


#ifndef __PREFSELECTIONWIDGET_H__
#define __PREFSELECTIONWIDGET_H__

#include <QWidget>
#include "preferencemanager.h"
#include "ui_prefselectionwidget.h"

class PrefSelectionWidget : public QWidget, public Ui::prefSelectionWidget
{
	Q_OBJECT
	public:
		PrefSelectionWidget (QWidget *parent, PreferenceValue *pref);
		void setLabel (QString s) { }
		QVariant getPrefValue () { return comboBox->itemData (comboBox->currentIndex ()); }

	protected:
		void setMap (QMap<QString, QVariant> map, QVariant def = QVariant ());
};
#endif
