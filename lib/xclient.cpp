#include <xmmsclient/xmmsclient++.h>

#include <QObject>
#include <QHash>
#include <QVariant>
#include <QErrorMessage>
#include <QSettings>

#include "xclient.h"
#include "xmmsqt4.h"

bool XClient::log ()
{
	return false;
}

XClient::XClient (QObject *parent, const std::string &name) : QObject (parent), Xmms::Client (name)
{
	m_cache = new XMediainfoCache (this, this);
}

bool
XClient::connect (const std::string &ipcpath)
{
	bool tried_once = false;

try_again:

	try {
		Xmms::Client::connect (ipcpath);
	}
	catch (Xmms::connection_error& e) {
		if (ipcpath == "" && !tried_once) {
			QSettings s;
			if (s.value ("core/autostart", true).toBool ()) {
				if (!system ("xmms2-launcher")) {
					tried_once = true;
					goto try_again;
				}
			}
		}

		QErrorMessage *err = new QErrorMessage ();
		err->showMessage ("Couldn't connect to XMMS2, please try again.");
		err->exec ();
		delete err;
		return false;
	}

	setMainloop (new XmmsQT4 (getConnection ()));

	emit gotConnection (this);

	return true;
}

void
XClient::propDictToQHash (const std::string &key,
						  const Xmms::Dict::Variant &value,
						  const std::string &source,
						  QHash<QString, QVariant> &hash)
{
	if (value.type () == typeid (int32_t)) {
		hash.insert (QString::fromLatin1 (key.c_str ()),
		             QVariant (boost::get< int32_t > (value)));
	} else if (value.type () == typeid (uint32_t)) {
		hash.insert (QString::fromLatin1 (key.c_str ()),
		             QVariant (boost::get< uint32_t > (value)));
	} else {
		QString val = QString::fromUtf8 (boost::get< std::string > (value).c_str ());
		if (key == "url") {
			val = val.mid (val.lastIndexOf ("/") + 1);
		}

		hash.insert (QString::fromLatin1 (key.c_str ()),
		             QVariant (val));
	}
}

QHash<QString, QVariant>
XClient::convert_propdict (const Xmms::PropDict &dict)
{
	QHash<QString, QVariant> hash;
	dict.each (boost::bind (&XClient::propDictToQHash,
							_1, _2, _3, boost::ref (hash)));

	return hash;
}


