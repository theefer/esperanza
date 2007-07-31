#include <QAbstractListModel>
#ifndef LISTMODEL_H
#define LISTMODEL_H

class Xmms2dPrefItem {
public:
	Xmms2dPrefItem (Xmms2dPrefItem* parent = NULL);
	~Xmms2dPrefItem ();

	void appendChild(Xmms2dPrefItem *child);
	Xmms2dPrefItem *child(int row);

	QVariant data (int column) const;
	bool setData (int column, const QVariant &data, int role = Qt::DisplayRole);
	int row() { if (m_parent) return m_parent->indexOfChild(this); else return 0; };
	int childCount() const { return rowCount (); }
	int rowCount () const { return m_children.size (); };
	int columnCount () const { return m_data.size (); };
	Xmms2dPrefItem *parent() { return m_parent; };
	bool insertRow (int row);
	int findItemRecursive (QVariant data, Xmms2dPrefItem **item);
	int indexOfChild(Xmms2dPrefItem *child);
	void sort(int column = 0, Qt::SortOrder order = Qt::AscendingOrder);

private:
	QList<QVariant> m_data;
	QList<Xmms2dPrefItem *> m_children;
	Xmms2dPrefItem* m_parent;
};


class Xmms2dPrefModel : public QAbstractListModel {
	Q_OBJECT
public:
	Xmms2dPrefModel ();
	~Xmms2dPrefModel ();

	Qt::ItemFlags flags(const QModelIndex &index) const;
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
	bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::DisplayRole);
	QVariant headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
	bool insertRow (int row, const QModelIndex &parent = QModelIndex ());
	int rowCount ( const QModelIndex & parent = QModelIndex() ) const;
	int columnCount ( const QModelIndex & parent = QModelIndex() ) const;
	QModelIndex index (int row, int column, const QModelIndex & parent = QModelIndex() ) const;
	QModelIndex findIndex (QVariant data, const QModelIndex & parent = QModelIndex()) const;
	QModelIndex parent(const QModelIndex &index) const;
	bool hasChildren ( const QModelIndex & parent = QModelIndex() ) const;
	void sort ( int column = 0, Qt::SortOrder order = Qt::AscendingOrder);
private:
	Xmms2dPrefItem* rootItem;
};

#endif
