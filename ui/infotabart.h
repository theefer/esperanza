#ifndef __INFOTAB_ART_H__
#define __INFOTAB_ART_H__

#include "infowindow.h"

#include <QWidget>
#include <QLabel>
#include <QPixmap>
#include <QPushButton>

class InfoTabArt : public InfoWindowTab
{
	Q_OBJECT
	public:
		InfoTabArt (QWidget *, XClient *);
		void fill (uint32_t);

	private slots:
		void browse ();
		void reset ();
		void apply ();

		void setPixmap (const QPixmap &);

	private:
		QLabel *m_art;
		QPushButton *m_apply;
		QPushButton *m_reset;
		QPushButton *m_browse;
		int m_current_id;
	
};

#endif

