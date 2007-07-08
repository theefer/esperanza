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


#include "mdns.h"

#ifdef HAVE_DNSSD
#include <dns_sd.h>

static void DNSSD_API
qr_reply (DNSServiceRef sdRef,
		  DNSServiceFlags flags,
		  uint32_t ifIndex,
		  DNSServiceErrorType errorCode,
		  const char *fullname,
		  uint16_t rrtype,
		  uint16_t rrclass,
		  uint16_t rdlen,
		  const void *rdata,
		  uint32_t ttl,
		  void *context)
{
	MDNSQuery *mq = static_cast<MDNSQuery*> (context);
	const unsigned char *rd = (unsigned char *) rdata;

	QString s = QString::fromAscii (fullname);
	s = s.left (s.indexOf ("."));

	MDNSServer *serv = mq->server (s);
	if (!serv) {
		qWarning ("This shouldn't happen!");
		return;
	}
	
	QString ip;
	ip.sprintf ("%d.%d.%d.%d", rd[0], rd[1], rd[2], rd[3]);
	serv->set_ip (ip);
	mq->serv_complete (serv);

	mq->destroy_sock (sdRef);
}

static void DNSSD_API
resolve_reply (DNSServiceRef client,
			   DNSServiceFlags flags,
			   uint32_t ifIndex,
			   DNSServiceErrorType errorCode,
			   const char *fullname,
			   const char *hosttarget,
			   uint16_t opaqueport,
			   uint16_t txtLen,
			   const char *txtRecord,
			   void *context)
{
	DNSServiceErrorType err;
	MDNSQuery *mq = static_cast<MDNSQuery*> (context);
	union { quint16 s; unsigned char b[2]; } portu = { opaqueport };
	QString s = QString::fromAscii (fullname);
	s = s.left (s.indexOf ("."));

	MDNSServer *serv = mq->server (s);
	if (!serv) {
		qWarning ("This shouldn't happen!");
		return;
	}

	serv->set_port (((quint16) portu.b[0]) << 8 | portu.b[1]);
	serv->set_hostname (QString::fromAscii (hosttarget));

	DNSServiceRef c2;
	err = DNSServiceQueryRecord (&c2, 0,
								 kDNSServiceInterfaceIndexAny,
								 hosttarget,
								 kDNSServiceType_A,
								 kDNSServiceClass_IN,
								 qr_reply, mq);

	if (err != kDNSServiceErr_NoError) {
		qWarning ("error from query record!");
	}
	
	mq->add_fd (c2);

	mq->destroy_sock (client);
}

static void DNSSD_API
browse_reply (DNSServiceRef client,
			  DNSServiceFlags flags,
			  uint32_t ifIndex,
			  DNSServiceErrorType errorCode,
			  const char *replyName,
			  const char *replyType,
			  const char *replyDomain,
			  void *context)
{   
	DNSServiceErrorType err;
	MDNSQuery *mq = static_cast<MDNSQuery*> (context);
	bool remove = (flags & kDNSServiceFlagsAdd) ? false : true;

	if (!remove) {
		DNSServiceRef c2;
		MDNSServer *server = new MDNSServer (QString::fromAscii (replyName),
											 QString::fromAscii (replyDomain));

		err = DNSServiceResolve (&c2, 0, kDNSServiceInterfaceIndexAny,
								 replyName, mq->service ().toAscii (), "local",
								 (DNSServiceResolveReply)resolve_reply, mq);

		if (err != kDNSServiceErr_NoError) {
			delete server;
			qWarning ("failed to run service resolve!");
			return;
		}

		mq->add_server (server);
		mq->add_fd (c2);

	} else {
		mq->remove_server (QString::fromAscii (replyName));
	}
}


void
MDNSQuery::serv_complete (MDNSServer *serv)
{
	if (serv->complete ())
		return;

	serv->set_complete (true);
	emit serverlistChanged ();
}

void
MDNSQuery::destroy_sock (DNSServiceRef client)
{
	QSocketNotifier *sn = m_sockets[client];
	sn->setEnabled (false);
	delete sn;
}

void
MDNSQuery::add_fd (DNSServiceRef client)
{
	QSocketNotifier *sn = new QSocketNotifier (DNSServiceRefSockFD (client),
											   QSocketNotifier::Read, this);
	connect (sn, SIGNAL (activated (int)), this, SLOT (process (int)));
	sn->setEnabled (true);
	m_sockets.insert (client, sn);
	m_sockets_fd.insert (sn->socket (), client);
}

void
MDNSQuery::process (int fd)
{
	DNSServiceErrorType err;
	DNSServiceRef ref = m_sockets_fd[fd];

	err = DNSServiceProcessResult (ref);
	if (err != kDNSServiceErr_NoError) {
		qWarning ("error in MDNS process!");
	}
}

void
MDNSQuery::browse_service (const QString &service)
{
	DNSServiceErrorType err;

	m_service = service;
	
	err = DNSServiceBrowse (&m_client, 0, kDNSServiceInterfaceIndexAny,
							service.toAscii (), 0,
							browse_reply, static_cast<void *> (this));
	if (err != kDNSServiceErr_NoError) {
		qWarning ("can't setup the browser!");
		return;
	}

	add_fd (m_client);
}

#endif /* HAVE DNSSD */

MDNSQuery::MDNSQuery (QObject *parent) : QObject (parent)
{
}

#ifndef HAVE_DNSSD
void
MDNSQuery::browse_service (const QString &service)
{
	/* noop */
}
#endif

