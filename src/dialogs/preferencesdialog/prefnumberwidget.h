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


#ifndef __PREFNUMBERWIDGET_H__
#define __PREFNUMBERWIDGET_H__

#include <QWidget>
#include <preferencemanager.h>
#include "ui_prefnumberwidget.h"

class PrefNumberWidget : public QWidget, public Ui::prefNumberWidget
{
	Q_OBJECT
	public:
		PrefNumberWidget (QWidget *parent, PreferenceValue *pref);
		QVariant getPrefValue () { return QVariant (spinBox->value ()); }
};
#endif
