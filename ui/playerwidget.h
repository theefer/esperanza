#ifndef __PLAYERWIDGET_H__
#define __PLAYERWIDGET_H__

#include <xmmsclient/xmmsclient++.h>

#include <QMainWindow>
#include <QMouseEvent>
#include <QAction>

#include "playlistview.h"
#include "progressframe.h"
#include "playerbutton.h"
#include "volumebar.h"
#include "systemtray.h"
#include "infowindow.h"
#include "minimode.h"
#include "lastfm.h"

class PlayerWidget : public QMainWindow
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
		void closeEvent (QCloseEvent *);

		void hide_mini () const
		{
			m_mini->hide ();
		};

	public slots:
		void play_pressed ();
		void lastfm_pressed ();
		void playstop_pressed ();
		void fwd_pressed ();
		void back_pressed ();
		void shuffle_pressed ();
		void snett_pressed (QMouseEvent *);
		void jump_pressed ();

	private slots:
		void plus_pressed (QMouseEvent *);
		void info_pressed (QMouseEvent *);
		void minus_pressed (QMouseEvent *);
		void got_connection (XClient *);
		void add_remote_file ();
		void add_local_file ();
		void add_local_dir ();
		void min_pressed ();

		void remove_selected ();
		void remove_all ();

		void show_medialib ();
		void entry_changed (uint32_t);

		void open_pref ();
		void open_about ();
		void open_short_help ();

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

		void handle_disconnect ();

		void new_info (const QHash<QString, QVariant>&);
		void set_colors ();

		uint32_t m_current_id;

		ProgressFrame *m_pf;
		VolumeBar *m_volbar;

		SystemTray *m_systray;
		InfoWindow *m_info;
		MiniMode *m_mini;

		LastFmDialog *m_lastfm;

};

#endif

