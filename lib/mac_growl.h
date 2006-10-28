#ifndef __MAC_GROWL_H__
#define __MAC_GROWL_H__

#include <QString>
#include <QVariant>

void mac_growl_post_notification (const QString &type, const QHash<QString, QVariant> &post);

#endif
