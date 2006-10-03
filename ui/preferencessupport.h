#ifndef __PREFERENCESSUPPORT_H__
#define __PREFERENCESSUPPORT_H__

#include <QMainWindow>
#include <QTableWidget>
#include <QHash>
#include <QMap>
#include <QVariant>
#include <QLabel>
#include <QDialog>
#include <QLabel>

class ColorButton : public QLabel
{
	Q_OBJECT
	public:
		ColorButton (QWidget *, const QColor &);
		QColor current_color () const;
		void mouseDoubleClickEvent (QMouseEvent *);

	private:
		QColor m_color;
};

class KeyInputDialog : public QDialog
{
	Q_OBJECT
	public:
		KeyInputDialog (QWidget *, const QString & = QString ());
		void keyPressEvent (QKeyEvent *);
		QString get_key () const {
			return m_key;
		};

	private:
		QLabel *m_l;
		QString m_key;
};

class KeyWidget : public QLabel
{
	public:
		KeyWidget (QWidget *, const QString & = QString ());
		void mouseDoubleClickEvent (QMouseEvent *);
		QString get_key () const {
			return m_key;
		};

	private:
		QString m_key;
};


#endif
