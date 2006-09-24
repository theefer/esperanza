#ifndef __XCLIENT_H__
#define __XCLIENT_H__

#include <xmmsclient/xmmsclient++.h>

class XClient;

#include <QObject>
#include <QHash>
#include <QVariant>

#include "xmediainfocache.h"

class XSettings : public QObject
{
	Q_OBJECT
	public:
		XSettings (QObject *);
		void change_settings ();

	signals:
		void settingsChanged ();
};

class XClient : public QObject, public Xmms::Client
{
	Q_OBJECT
	public:
		XClient (QObject *, const std::string &);

		static bool log ();
		bool connect (const std::string &ipcpath = "");
		static void propDictToQHash (const std::string &key,
									 const Xmms::Dict::Variant &value,
									 const std::string &source,
									 QHash<QString, QVariant> &hash);

		static QHash<QString, QVariant> convert_propdict (const Xmms::PropDict &);

		XMediainfoCache *cache () const {
			return m_cache;
		};

		XSettings *settings () const {
			return m_settings;
		};

	signals:
		void gotConnection (XClient *);

	private:
		XMediainfoCache *m_cache;
		XSettings *m_settings;

};


#endif
