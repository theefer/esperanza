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

	private slots:
		void got_connection (XClient *);
		void plus_pressed (QMouseEvent *);
		void minus_pressed (QMouseEvent *);
		void play_pressed (QMouseEvent *);
		void playstop_pressed (QMouseEvent *);
		void fwd_pressed (QMouseEvent *);
		void back_pressed (QMouseEvent *);
		void snett_pressed (QMouseEvent *);

		void add_remote_file ();
		void add_local_file ();
		void add_local_dir ();

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

		bool handle_index_status (const Xmms::Stats::ReaderStatus &);
		bool handle_unindexed (const uint32_t &);
		void handle_disconnect ();

		void new_info (const QHash<QString, QVariant>&);
		void set_colors ();

		uint32_t m_current_id;

		ProgressFrame *m_pf;
		VolumeBar *m_volbar;

		QProgressDialog *m_unindexed;
		
		SystemTray *m_systray;

};

#endif

