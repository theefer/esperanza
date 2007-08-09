/** 
 *  This file is a part of Esperanza, an XMMS2 Client.
 *
 *  Copyright (C) 2005-2006 XMMS2 Team
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

#include "xmms2dprefdialog.h"

#include <QList>
#include <QListIterator>
#include <QWidget>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QSpinBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QSettings>
#include <QComboBox>
#include <QGroupBox>
#include <QHeaderView>
#include <QTableWidgetItem>
#include <QScrollArea>
#include "debug.h"

Xmms2dPrefDialog::Xmms2dPrefDialog (QWidget *parent, XClient *client_)
: QDialog (parent)
{
	QSettings s;

	setupUi (this);

	model = new Xmms2dPrefModel();

	treeView->setModel (model);
	treeView->setItemsExpandable (true);
	treeView->setRootIsDecorated (true);

	connect(model, SIGNAL (dataChanged (QModelIndex, QModelIndex)), this, SLOT (userChangedPref (QModelIndex, QModelIndex)));

	client = client_;

	connect (client, SIGNAL (gotConnection (XClient *)), this, SLOT (got_connection (XClient *)));
	if(client->isConnected ())
		got_connection(client);

}

Xmms2dPrefDialog::~Xmms2dPrefDialog ()
{

}

void Xmms2dPrefDialog::clickedBtnBox (QAbstractButton* btn)
{
	switch (buttonBox->buttonRole (btn))
	{
		case QDialogButtonBox::AcceptRole:
			pressedOk ();
			break;
		case QDialogButtonBox::ApplyRole:
			pressedApply ();
			break;
		case QDialogButtonBox::RejectRole:
			pressedCancel ();
			break;
		default:
			break;
	}
}

void Xmms2dPrefDialog::got_connection (XClient *client_)
{
	client = client_;
	client->config ()->valueList () (Xmms::bind (&Xmms2dPrefDialog::handle_valueList, this));
	client->config ()->broadcastValueChanged () (Xmms::bind(&Xmms2dPrefDialog::handle_valueChanged, this));
}

bool Xmms2dPrefDialog::handle_valueList (const Xmms::Dict &d)
{
	d.each(Xmms::bind (&Xmms2dPrefDialog::addPreference, this));
	model->sort ();
	return true;
}

bool Xmms2dPrefDialog::handle_valueChanged (const Xmms::Dict &d)
{
	d.each(Xmms::bind (&Xmms2dPrefDialog::updatePreference, this));
	return true;
}

void Xmms2dPrefDialog::addPreference( const std::string& name, const Xmms::Dict::Variant& value)
{
	QString prefName (name.c_str ());
	QString prefValue (boost::get<std::string> (value).c_str ());

	// DBGOUT("adding Preference:" << prefName << ", Value:" << prefValue);
	QStringList treePath = prefName.split (".");
	QString parentName = treePath.takeFirst ();
	QModelIndex parent = model->findIndex(parentName);
	QModelIndex tmp;
	QModelIndex child;
	QString childName;
	int iTmp;

	if(!parent.isValid ()) {
		if(model->insertRow(model->rowCount ())) {
			parent = model->index(model->rowCount () - 1, 0);
			if(parent.isValid ()) {
				dataChanging.lock ();
				model->setData (parent, QVariant (parentName));
				dataChanging.unlock ();
			}
		}
		else
			DBGOUT("ERROR: could not insert a new row.");
	}

	tmp = parent;
	QStringListIterator iter (treePath);
	while (iter.hasNext ()) {
		childName = iter.next ();
		child = model->findIndex(childName, tmp);
		if (!child.isValid ()) {
			// DBGOUT("Adding child to parent:" << tmp.data ().toString ());
			iTmp = model->rowCount (tmp);
			if(model->insertRow (iTmp, tmp)) {
				child = model->index(iTmp, 0, tmp);
				if(child.isValid ()) {
					dataChanging.lock ();
					model->setData (child, QVariant (childName));
					dataChanging.unlock ();
				}
				else
					DBGOUT("ERROR: created child is not valid.");
			}
		}

		if(iter.hasNext ())
			tmp = child;
	}
	
	if (child.isValid ()) {
		child = model->index(child.row (), 1, tmp);
		if(child.isValid ()) {
			dataChanging.lock ();
			model->setData (child, QVariant (prefValue));
			dataChanging.unlock ();
		}
	}
/*
	if( value.type() == typeid( unsigned int ) ) {
		unsigned int temp = boost::get< unsigned int >( value );
		std::cout << temp;
	}
	else if( value.type() == typeid( int ) ) {
		int temp = boost::get< int >( value );
		std::cout << temp;
	}
	else if( value.type() == typeid( std::string ) ) {
		std::string temp = boost::get< std::string >( value );
		std::cout << temp;
	}
*/
}

void Xmms2dPrefDialog::updatePreference( const std::string& name, const Xmms::Dict::Variant& value)
{
	QString prefName(name.c_str ());
	QString prefValue (boost::get<std::string> (value).c_str ());
	QStringList treePath = prefName.split (".");
	QString childName;
	QString parentName = treePath.takeFirst ();
	QModelIndex child;
	QModelIndex parent = model->findIndex(parentName);
	QModelIndex tmp;
	QStringListIterator iter (treePath);

	if(!parent.isValid ()) {
		addPreference (name, value);
		return;
	}
	
	tmp = parent;
	while (iter.hasNext ()) {
		childName = iter.next ();
		child = model->findIndex(childName, tmp);
		if (!child.isValid ())
			return;
		if(iter.hasNext ())
			tmp = child;
	}

	if(parent == tmp) {
		addPreference (name, value);
	}
	else {
		dataChanging.lock ();
		model->setData (child, QVariant(prefValue));
		dataChanging.unlock ();
	}
}

void Xmms2dPrefDialog::pressedOk ()
{
	pressedApply ();
	pressedCancel ();
}

void Xmms2dPrefDialog::pressedApply ()
{
	QListIterator<QModelIndex> iter (changedPrefs);
	QModelIndex valueIndex;
	QString value;
	QString key;

	while (iter.hasNext ()) {
		valueIndex = iter.next ();
		value = model->data (valueIndex).toString ();
		key = getKeyString (valueIndex);

		// DBGOUT ("saving: key: " << key << ", value:" << value);
		client->config ()->valueSet (key.toStdString (), value.toStdString ()) ();
	}
}

void Xmms2dPrefDialog::pressedCancel ()
{
	close ();
}

void Xmms2dPrefDialog::userChangedPref (QModelIndex topLeft, QModelIndex bottomRight)
{
	bool isLocked = true;
	if (dataChanging.tryLock()) {
		dataChanging.unlock();
		isLocked = false;
	}
	if(isLocked)	// locked means, that the values are initialized or changed from xmms2d itself, but not from the user,
		return;		// so we can ignore these changes.

	// should be always the same, caused by current implementation of Xmms2dPrefModel
	if (topLeft == bottomRight) {
		// DBGOUT ("dataChanged: row=" << topLeft.row () << ", column=" << topLeft.column () << ", key=" << getKeyString(topLeft));
		changedPrefs.append (topLeft);
	}
}

QString Xmms2dPrefDialog::getKeyString(QModelIndex value)
{
	QString ret;
	if (!value.isValid ())
		return QString();

	QModelIndex child = model->index (value.row (), 0, value.parent ());
	if (!child.isValid ())
		return QString();

	ret = model->data (child).toString ();
	do
	{
		child = child.parent ();
		ret = model->data (child).toString () + "." + ret;
	}
	while (child.parent ().isValid ());
	return ret;
}
