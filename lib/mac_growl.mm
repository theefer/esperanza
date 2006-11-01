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

