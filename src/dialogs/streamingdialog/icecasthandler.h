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

#ifndef __ICECAST_HANDLER_H__
#define __ICECAST_HANDLER_H__

#include <QXmlDefaultHandler>

class IcecastChannel
{
    public:
        IcecastChannel () {};
        
        IcecastChannel (const IcecastChannel &chan) {
            m_name = chan.name ();
            m_url = chan.url ();
            m_genre = chan.genre ();
            m_br = chan.bitrate ();
        };
        
        void setName (const QString &name) {
            m_name = name;
        };
        
        void setUrl (const QString &url) {
            m_url = url;
        };
        
        void setBitrate (const QString &br) {
            m_br = br;
        };
        
        void setGenre (const QString &genre) {
            m_genre = genre;
        };
        
        QString name () const {
            return m_name;
        };
        
        QString url () const {
            return m_url;
        };
        
        QString bitrate () const {
            return m_br;
        };
        
        QString genre () const {
            return m_genre;
        };
        
        void reset () {
            m_name.clear ();
            m_url.clear ();
            m_br.clear ();
            m_genre.clear ();
        };
        
    private:
        QString m_name;
        QString m_url;
        QString m_br;
        QString m_genre;
};

class IcecastHandler : public QXmlDefaultHandler
{
	public:
		IcecastHandler ();
		
		enum states {
            EMPTY,
            ENTRY,
            NAME,
            URL,
            BITRATE,
            GENRE
        };

        QList<IcecastChannel> channel_list () const {
            return m_channels;
        };

		bool characters (const QString &s);
		bool endDocument ();
		bool endElement (const QString &, const QString &, const QString &);
		bool startElement (const QString &, const QString &, const QString &, const QXmlAttributes &);

	private:
        QList<IcecastChannel> m_channels;
        IcecastChannel m_current;
        
        states m_state;
        states m_entry_state;
};

#endif
