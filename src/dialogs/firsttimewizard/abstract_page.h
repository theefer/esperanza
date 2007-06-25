#include <QWidget>

#ifndef _ABSTRACT_PAGE_H_
#define _ABSTRACT_PAGE_H_

class AbstractPage : public QWidget {
	Q_OBJECT
public:
	AbstractPage(QWidget *parent_ = NULL)
	: QWidget(parent_)
	{
		parent = parent_;
		prev = NULL;
		next = NULL;
	};
	virtual void setNext(AbstractPage *next_ = NULL) { if(next_) next = next_; }
	virtual void setPrevious(AbstractPage *previous_ = NULL) { if(previous_) prev = previous_; }

public slots:
	virtual void cancel() { emit lastClosed(true); }
	virtual void nextPage()
	{
		if(next) { next->show(); hide(); }
		else {
			hide();
			emit lastClosed();
		}
	}
	virtual void previousPage() { if(prev) { prev->show(); hide(); } }
	virtual void saveSettings() { };
	virtual void showEvent(QShowEvent *ev) { parent->setWindowTitle(""); }

signals:
	void lastClosed(bool Canceled = false);

protected:
	AbstractPage *prev;
	AbstractPage *next;
	QWidget* parent;
};
#endif
