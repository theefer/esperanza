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


#ifndef __MDNS_H__
#define __MDNS_H__

#include <QString>
#include <QHash>
#include <QList>
#include <QSocketNotifier>
#include <QObject>

#ifdef HAVE_DNSSD
#	include <dns_sd.h>
#endif

class MDNSServer
{
	public:
		MDNSServer (const QString &name, const QString &domain) {
			m_name = name;
			m_domain = domain;
			m_complete = false;
		};

		QString name () const {
			return m_name;
		};

		void set_port (quint16 p) {
			m_port = p;
		};
		
		void set_hostname (const QString &s) {
			m_hostname = s;
		};

		void set_ip (const QString &ip) {
			m_ip = ip;
		};

		QString ip () const {
			return m_ip;
		};

		quint16 port () const {
			return m_port;
		};

		void set_complete (bool b) {
			m_complete = b;
		};

		bool complete () const {
			return m_complete;
		};

	private:
		QString m_name;
		QString m_domain;
		quint16 m_port;
		QString m_hostname;
		QString m_ip;
		bool m_complete;

};

class MDNSQuery : public QObject
{
	Q_OBJECT
	public:
		MDNSQuery (QObject *parent);
		void browse_service (const QString &service);

#ifdef HAVE_DNSSD
		void add_fd (DNSServiceRef);
		void destroy_sock (DNSServiceRef);
#endif

		void add_server (MDNSServer *s) {
			m_servers[s->name ()] = s;
		};

		void remove_server (const QString &s) {
			MDNSServer *serv = server (s);
			if (serv) {
				m_servers.remove (s);
				emit serverlistChanged ();
				delete serv;
			}
		};

		QString service () const {
			return m_service;
		};

		MDNSServer *server (const QString &name) const {
			if (m_servers.contains (name))
				return m_servers[name];
			return NULL;
		};

		void serv_complete (MDNSServer *);

		QList<MDNSServer *> serverlist () const {
			QList<MDNSServer *> ret;
			for (int i = 0; i < m_servers.keys ().count (); i++) {
				ret.append (m_servers[m_servers.keys ().at (i)]);
			}
			return ret;
		};

	private slots:
#ifdef HAVE_DNSSD
		void process (int);
#endif

	signals:
		void serverlistChanged ();

	private:
#ifdef HAVE_DNSSD
		QHash<DNSServiceRef, QSocketNotifier *> m_sockets;
		QHash<int, DNSServiceRef> m_sockets_fd;
		DNSServiceRef m_client;
#endif
		QHash<QString, MDNSServer *> m_servers;
		QString m_service;
};

#endif
