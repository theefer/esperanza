#ifndef __VALUEBAR_H__
#define __VALUEBAR_H__

#include <QWidget>
#include <QPaintEvent>

class ValueBar : public QWidget
{
	Q_OBJECT
	public:
		ValueBar (QWidget *);
		void paintEvent (QPaintEvent *);
		void setValue (int i) {
			m_value = i;
			update ();
		};
	private:
		int m_value;
};

#endif
