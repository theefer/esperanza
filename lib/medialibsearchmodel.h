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
		void do_search (uint32_t, const QString &);
		void got_connection (XClient *client);

	private:
		bool handle_search (const Xmms::List<Xmms::Dict> &);
};

#endif
