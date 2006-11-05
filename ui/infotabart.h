#ifndef __INFOTAB_ART_H__
#define __INFOTAB_ART_H__

#include "infowindow.h"

#include <QWidget>
#include <QLabel>
#include <QPixmap>
#include <QPushButton>
#include <QProgressDialog>

class InfoTabArt : public InfoWindowTab
{
	Q_OBJECT
	public:
		InfoTabArt (QWidget *, XClient *);
		void fill (uint32_t);

	private slots:
		void browse ();
		void apply ();
		void remove ();
		void remove_art (bool);
		void art_finished ();

		void setPixmap (const QPixmap &);

	private:
		QString build_album_query (const QHash<QString, QVariant> &);
		bool medialib_reply (const Xmms::List< Xmms::Dict > &list, const std::string &hash);
		bool medialib_reply_remove (const Xmms::List< Xmms::Dict > &list);
		void add_art (bool);
		bool art_uploaded (const std::string &, bool);

		QLabel *m_art;
		QPushButton *m_apply;
		QPushButton *m_browse;
		QPushButton *m_remove;
		int m_current_id;

		QProgressDialog *m_progress;
	
};

#endif

