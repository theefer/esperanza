#ifndef __PLAYERWIDGET_H__
#define __PLAYERWIDGET_H__

#include <xmmsclient/xmmsclient++.h>

#include <QWidget>
#include <QMouseEvent>
#include <QAction>

#include "playlistview.h"
#include "progressframe.h"
#include "playerbutton.h"

class PlayerWidget : public QWidget
{
	Q_OBJECT
	public:
		PlayerWidget (QWidget *parent, XClient *client);
		void keyPressEvent (QKeyEvent *);

		Xmms::Playback::Status status () const
		{
			return m_status;
		};

		void resizeEvent (QResizeEvent *);
		void moveEvent (QMoveEvent *);

	private slots:
		void got_connection (XClient *);
		void plus_pressed (QMouseEvent *);
		void minus_pressed (QMouseEvent *);
		void play_pressed (QMouseEvent *);
		void playstop_pressed (QMouseEvent *);
		void fwd_pressed (QMouseEvent *);
		void back_pressed (QMouseEvent *);
		void snett_pressed (QMouseEvent *);
		void volume_pressed (QMouseEvent *);

		void add_remote_file ();
		void add_local_file ();
		void add_local_dir ();
		void volume_changed (QAction *);

		void remove_selected ();
		void remove_all ();

		void show_medialib ();
		void entry_changed (uint32_t);

		void open_pref ();

		void changed_settings ();

	private:
		XClient *m_client;
		Xmms::Playback::Status m_status;
		PlaylistView *m_playlist;
		PlayerButton *m_playbutt;
		PlayerButton *m_playstop;

		bool handle_current_id (const unsigned int &);
		bool handle_playtime (const unsigned int &);
		bool handle_status (const Xmms::Playback::Status &);
		bool handle_volume (const Xmms::Dict &);

		void new_info (const QHash<QString, QVariant>&);

		uint32_t m_current_id;
		uint32_t m_volume;
		uint32_t m_channels;

		ProgressFrame *m_pf;
};

#endif

