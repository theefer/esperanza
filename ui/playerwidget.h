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


#ifndef __PLAYERWIDGET_H__
#define __PLAYERWIDGET_H__

#include <xmmsclient/xmmsclient++.h>

#include <QMainWindow>
#include <QMouseEvent>
#include <QAction>

#include "fancyplaylistview.h"
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
		void coll_pressed ();
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
		void add_url ();
		void min_pressed ();
		void mlib_pressed ();

		void remove_selected ();
		void remove_all ();

		void entry_changed (uint32_t);

		void open_pref ();
		void open_about ();
		void open_short_help ();

		void changed_settings ();

	private:
		XClient *m_client;
		Xmms::Playback::Status m_status;
		FancyPlaylistView *m_playlist;
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

