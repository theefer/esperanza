#ifndef __PLAYLIST_VIEW_H__
#define __PLAYLIST_VIEW_H__

#include <xmmsclient/xmmsclient++.h>

#include <QTreeView>
#include <QItemDelegate>
#include <QDropEvent>

#include "playlistmodel.h"
#include "xclient.h"

class PlaylistDelegate : public QItemDelegate
{
	public:
		PlaylistDelegate (QObject *parent, PlaylistModel *model);
		void paint (QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

	private:
		PlaylistModel *m_model;
};

class PlaylistView : public QTreeView
{
	Q_OBJECT
	public:
		PlaylistView (QWidget *parent, XClient *client);
		QList<uint32_t> getSelection ();

	public slots:
		void jump_pos (const QModelIndex &);
		void item_selected (const QModelIndex &, const QModelIndex &);
		void got_connection (XClient *client);


	private:
		XClient *m_client;
		PlaylistModel *m_model;
		QItemSelectionModel *m_selections;

		bool handle_update_pos (const uint32_t &);
};


#endif
