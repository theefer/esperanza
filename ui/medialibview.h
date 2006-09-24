#ifndef __MEDIALIBVIEW_H__
#define __MEDIALIBVIEW_H__

#include "xclient.h"
#include "medialibsearchmodel.h"

#include <QTreeView>
#include <QEvent>

class MedialibView : public QTreeView
{
	Q_OBJECT
	public:
		MedialibView (QWidget *, XClient *);

		void do_search (uint32_t p, const QString &s)
		{
			m_model->do_search (p, s);
		};

		QList<uint32_t> get_selection ();
		QList<uint32_t> get_all ();

	private slots:
		void search_done ();
		void add_id (const QModelIndex &);

	signals:
		void searchDone ();

	private:
		XClient *m_client;
		MedialibSearchModel *m_model;
		QItemSelectionModel *m_selections;
};

#endif
