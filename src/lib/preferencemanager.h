/** 
 *  This file is a part of Esperanza, an XMMS2 Client.
 *
 *  Copyright (C) 2005-2007 XMMS2 Team
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

#ifndef __PREFERENCE_MANAGER_H__
#define __PREFERENCE_MANAGER_H__

#include <QObject>
#include <QString>
#include <QStringList>
#include <QVariant>
#include <QDebug>

/**
 * This class defines a registered config value.
 * You can't change the value properties after it's
 * registred and you should only use the access functions
 * here.
 */
class PreferenceValue
{
	public:
		/** Describe the type of this value */
		enum ValueType {
			/** true or false */
			Bool = 0,
			/** number, just int at the moment */
			Num,
			/** a single string */
			Str,
			/** a selection of values, think combobox */
			Selection,
			/** Multiple selection of value */
			MultiSelection,
			/** keyboard shortcut */
			Key
		};

		enum ValueSection {
			None = 0,
			Core,
			Look,
			Feel,
			Shortcuts,
			All
		};

		PreferenceValue (const QString &key,
						 const QString &help,
						 const ValueType &type,
						 const QVariant &defval = QVariant (),
						 const QVariant &data = QVariant (),
						 const ValueSection &sec = None)
		{
			m_key = key;
			m_help = help;
			m_type = type;
			m_defval = defval;
			m_data = data;
			m_section = sec;
		};

		QString key () const {
			return m_key;
		};

		QString help () const {
			return m_help;
		};

		ValueType type () const {
			return m_type;
		};

		QVariant defval () const {
			return m_defval;
		};

		QVariant data () const {
			return m_data;
		};

		ValueSection section () const {
			return m_section;
		};

	private:
		QString m_key;
		QString m_help;
		ValueType m_type;
		QVariant m_data;
		QVariant m_defval;
		ValueSection m_section;
};


/**
 * The preferencemanager knows about all registered config value
 * a registred configvalue is something the user can change.
 * If you only want to have non-user-exposed config values use
 * QSettings directly.
 *
 * call registerVal to register your key. supply a help text and a default.
 * data is for multiple choice or a upper limit of the number.
 * section will be used to do tabbed configdialogs.
 *
 * the preferencedialog should use values() to get a QList of all values
 * and then build the UI
 */
class PreferenceManager : public QObject
{
	Q_OBJECT
	public:
		static PreferenceManager* instance ();

		QList<PreferenceValue *> values (const PreferenceValue::ValueSection &sec = PreferenceValue::All) const;
		QStringList keys () const;

		void registerVal (const QString &key,
						  const QString &help,
						  const PreferenceValue::ValueType &type,
						  const QVariant &defval,
						  const QVariant &data = QVariant (),
						  const PreferenceValue::ValueSection &section = PreferenceValue::None);

	private:
		PreferenceManager ();

		QMap<QString, PreferenceValue *> m_values;
		QMap<PreferenceValue::ValueSection, PreferenceValue *> m_sections;

		static PreferenceManager* instance_;
};

#endif
