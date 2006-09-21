#ifndef __PLAYERBUTTON_H__
#define __PLAYERBUTTON_H__

#include <QLabel>
#include <QPixmap>

class PlayerButton : public QLabel
{
	Q_OBJECT
	public:
		PlayerButton (QWidget *, const QString &);
		void mousePressEvent (QMouseEvent *);
		void setPx (const QString &);

		void enterEvent (QEvent *);
		void leaveEvent (QEvent *);

	signals:
		void clicked (QMouseEvent *);

	private:
		QPalette m_normal;
		QPalette m_hoover;
};

#endif
