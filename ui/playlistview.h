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
		QModelIndexList getSelection ();

		void keyPressEvent (QKeyEvent *);
		void mousePressEvent (QMouseEvent *);

		PlaylistModel *model () const {
			return m_model;
		};

		void set_removed (bool b) {
			m_removed = b;
		};

	public slots:
		void jump_pos (const QModelIndex &);
		void item_selected (const QModelIndex &, const QModelIndex &);
		void got_connection (XClient *client);
		void changed_settings ();
		void rows_inserted ();
		void head_size (int, int, int);

	signals:
		void selectedID (uint32_t);

	private:
		XClient *m_client;
		PlaylistModel *m_model;
		QItemSelectionModel *m_selections;
		QWidget *m_parent;

		bool handle_update_pos (const uint32_t &);
		bool m_removed;
};


#endif
