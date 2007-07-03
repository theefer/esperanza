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


#ifndef __PREFBOOLWIDGET_H__
#define __PREFBOOLWIDGET_H__

#include <QWidget>
#include <preferencemanager.h>
#include "ui_prefboolwidget.h"

class PrefBoolWidget : public QWidget, public Ui::prefBoolWidget
{
	Q_OBJECT
	public:
		PrefBoolWidget (QWidget *parent, PreferenceValue *pref);
		QVariant getPrefValue () { return QVariant(checkBox->checkState () == Qt::Checked ? true : false); }
};
#endif
