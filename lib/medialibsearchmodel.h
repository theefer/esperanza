#ifndef __MEDIALIBSEARCHMODEL_H__
#define __MEDIALIBSEARCHMODEL_H__

#include "xclient.h"
#include "playlistmodel.h"

#include <QStandardItemModel>

class MedialibSearchModel : public PlaylistModel
{
	Q_OBJECT
	public:
		enum {
			SEARCH_ALL,
			SEARCH_ARTIST,
			SEARCH_ALBUM,
			SEARCH_TITLE,
			SEARCH_YEAR
		};

		MedialibSearchModel (QObject *parent, XClient *client);
		void do_search (uint32_t, const QString &, bool unav=false);
		void got_connection (XClient *client);

		QMimeData *mimeData (const QModelIndexList &list) const;

		Qt::DropActions supportedDropActions () const {
			return Qt::IgnoreAction;
		};
		QStringList mimeTypes () const {
			return QStringList ("application/x-xmms2mlibid");
		};


	signals:
		void searchDone ();

	private:
		bool handle_search (const Xmms::List<Xmms::Dict> &);
};

#endif
