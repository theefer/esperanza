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

#include <Foundation/Foundation.h>
#include <Cocoa/Cocoa.h>

#include <QHash>
#include <QVariant>
#include <QStringList>
#include <QPixmap>
#include <QBuffer>
#include <QByteArray>

void
mac_growl_post_notification (const QString &type,
							 const QHash<QString, QVariant> &post)
{
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	NSMutableDictionary *note = [NSMutableDictionary dictionary];
	NSString *noteName = [NSString stringWithUTF8String:type.toUtf8 ()];

	QStringList l = post.keys ();

	for (int i = 0; i < l.size (); i++) {
		NSString *key = [NSString stringWithUTF8String:l.at (i).toUtf8 ()];

		QVariant qval = post[l.at (i)];

		if (qval.type () == QVariant::String) {
			NSString *val;
			val = [NSString stringWithUTF8String:qval.toString ().toUtf8 ()];
			[note setObject:val forKey:key];
		} else if (qval.type () == QVariant::Pixmap) {
			QByteArray ba;
			QBuffer buffer (&ba);
			buffer.open(QIODevice::WriteOnly);
			QPixmap i = qval.value<QPixmap> ();

			NSData *img;

			if (!i.isNull ()) {
				i.save (&buffer, "JPG");
				img = [[[NSData alloc] initWithBytes:ba.data () length:ba.size ()] autorelease];
			} else {
				NSImage *i;
				i = [[NSWorkspace sharedWorkspace] iconForFileType:[NSString stringWithUTF8String:"mp3"]];
				img = [i TIFFRepresentation];
			}

			[note setObject:img forKey:key];
		} else if (qval.type () == QVariant::StringList) {
			NSMutableArray *list = [NSMutableArray array];
			QStringList sl = qval.toStringList ();

			[list removeAllObjects];
			for (int j = 0; j < sl.size (); j ++) {
				NSString *val = [NSString stringWithUTF8String:sl.at (j).toUtf8 ()];
				[list addObject:val];
			}
			[note setObject:list forKey:key];
		} else {
			qDebug ("Couldn't handle %s", qPrintable (l.at (i)));
		}
	}

	[[NSDistributedNotificationCenter defaultCenter] postNotificationName:noteName 
		object:nil
		userInfo:note
		deliverImmediately:NO];

	[pool release];
}

