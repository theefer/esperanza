#include "xmms2dprefmodel.h"
#include <QMessageBox>
#include "debug.h"

/******************************************************************************************/
/* The Item                                                                               */
/******************************************************************************************/
Xmms2dPrefItem::Xmms2dPrefItem (Xmms2dPrefItem* parent/* = NULL*/)
{
	// DBGOUT("entered");
	m_parent = parent;
	// DBGOUT("leaving");
}

Xmms2dPrefItem::~Xmms2dPrefItem () {
	// DBGOUT("entered");
	qDeleteAll(m_children);
	// DBGOUT("leaving");
}

void Xmms2dPrefItem::appendChild(Xmms2dPrefItem *child)
{
	// DBGOUT("entered");
	if(child)
		m_children.append(child);
	// DBGOUT("leaving");
}

QVariant Xmms2dPrefItem::data (int column) const
{
	// DBGOUT("entered"); // to excessive
	QVariant ret;
	if (m_data.size () > column) {
		ret = m_data.value (column);
		// DBGOUT("column:" << column << ", data:" << ret.toString ());
	}
	// DBGOUT("leaving"); // to excessive
	return ret;
}

Xmms2dPrefItem* Xmms2dPrefItem::child(int row) {
	// DBGOUT("entered; row:" << row); // to excessive
	Xmms2dPrefItem* ret = NULL;
	if (m_children.size () > row) {
		ret = m_children.at (row);
		// DBGOUT("child requested; row:" << row << ", child:" << ret);
	}
	else {
		// DBGOUT("ERROR: child requested, but with invalid row:" << row);
	}
	// DBGOUT("leaving; ret:" << ret); // to excessive
	return ret;
}

bool Xmms2dPrefItem::setData (int column, const QVariant &data, int role/* = Qt::DisplayRole*/)
{
	// DBGOUT("entered - col:" << column << ", Data:" << data.toString ());
	if (role == Qt::DisplayRole || role == Qt::EditRole)
		m_data.insert(column, data);
	// DBGOUT("leaving");
	return true;
}

bool Xmms2dPrefItem::insertRow (int row)
{
	// DBGOUT("entered; row:" << row); // to excessive
	bool ret = false;
	Xmms2dPrefItem *item = new Xmms2dPrefItem(this);
	if (item) {
		// DBGOUT("inserting item:" << item << " at row:" << row);
		m_children.insert(row, item);
		ret = true;
	}
	// DBGOUT("leaving; ret:" << ret); // to excessive
	return true;
}

int Xmms2dPrefItem::findItemRecursive (QVariant data, Xmms2dPrefItem **item)
{
	// DBGOUT("entered"); // to excessive
	if(item)
		*item = NULL;

	int ret = -1;
	if (m_data.contains (data)) {
		*item = this;
		ret = m_data.indexOf (data);
	}
	else {
		for (int i = 0 ; i < m_children.size (); i++) {
			ret = m_children[i]->findItemRecursive (data, item);
			if(ret > -1 && *item)
				break;
		}
	}
	// DBGOUT("leaving"); // to excessive
	return ret;
}

int Xmms2dPrefItem::indexOfChild(Xmms2dPrefItem *child)
{
	// DBGOUT("entered"); // to excessive
	int ret = -1;
	if (m_children.contains (child))
		ret = m_children.indexOf (child);
	// DBGOUT("leaving"); // to excessive
	return ret;
}

void Xmms2dPrefItem::sort ( int column/* = 0*/, Qt::SortOrder order/* = Qt::AscendingOrder */)
{
	QList<Xmms2dPrefItem*> new_children;
	Xmms2dPrefItem* item;
	Xmms2dPrefItem* tmp;

	while (m_children.size () > 0) {
		item = m_children.takeFirst ();
		for (int i=0; i < m_children.size (); i++) {
			if (
				(item->data (column).toString () > m_children[i]->data (column).toString () && order == Qt::AscendingOrder) ||
				(item->data (column).toString () < m_children[i]->data (column).toString () && order == Qt::DescendingOrder)
			) {
				tmp = m_children[i];
				m_children[i] = item;
				item = tmp;
			}
		}
		new_children.append (item);
	}

	for (int i=0; i < new_children.size (); i++) {
		new_children[i]->sort(column, order);
		m_children.append(new_children[i]);
	}
}

/******************************************************************************************/
/* The Model                                                                              */
/******************************************************************************************/

Xmms2dPrefModel::Xmms2dPrefModel() {
	// DBGOUT("entered");
	rootItem = new Xmms2dPrefItem ();
	rootItem->setData (0, QVariant("Name"));
	rootItem->setData (1, QVariant("Value"));
	// DBGOUT("leaving");
}

Xmms2dPrefModel::~Xmms2dPrefModel()
{
	// DBGOUT("entered");
	delete rootItem;
	// DBGOUT("leaving");
}

Qt::ItemFlags Xmms2dPrefModel::flags ( const QModelIndex & index ) const
{
	// DBGOUT("entered"); // to excessive
	Qt::ItemFlags ret = 0;
	if (index.isValid ()) {
		ret = Qt::ItemIsEnabled | Qt::ItemIsSelectable;
		if (index.column () == 1)
			ret |= Qt::ItemIsEditable;
	}
	// DBGOUT("leaving"); // to excessive
	return ret;
}

QVariant Xmms2dPrefModel::data ( const QModelIndex & index, int role/* = Qt::DisplayRole*/) const
{
	// DBGOUT("entered"); // to excessive
	QVariant ret;
	if (index.isValid () && (role == Qt::DisplayRole || role == Qt::EditRole)) {
		ret = static_cast<Xmms2dPrefItem*>(index.internalPointer ())->data (index.column ());
	}
	// DBGOUT("leaving"); // to excessive
	return ret;
}

bool Xmms2dPrefModel::setData(const QModelIndex &index, const QVariant &value, int role/* = Qt::DisplayRole*/)
{
	// DBGOUT("entered");
	bool ret = false;

	if( index.isValid () ) {
		Xmms2dPrefItem *item = static_cast<Xmms2dPrefItem *>(index.internalPointer ());
		if (item) {
			// DBGOUT("row:" << index.row () << ", column:" << index.column () << ", data:" << value.toString ());
			ret = item->setData (index.column (), value, role);
			emit dataChanged (index, index);
		}
	}
	// DBGOUT("leaving");
	return ret;
}

QVariant Xmms2dPrefModel::headerData ( int section, Qt::Orientation orientation, int role ) const
{
	// DBGOUT("entered"); // to excessive
	Q_UNUSED(orientation);
	QVariant ret;

	if (role == Qt::DisplayRole) 
		ret = rootItem->data (section);

	// DBGOUT("leaving"); // to excessive
	return ret;
}

int Xmms2dPrefModel::rowCount ( const QModelIndex & parent ) const {
	// DBGOUT("entered"); // to excessive
	int ret;
	Xmms2dPrefItem *item;

	if(parent.isValid ())
		item = static_cast<Xmms2dPrefItem*> (parent.internalPointer ());
	else
		item = rootItem;

	ret = item->rowCount ();
	// DBGOUT("data:" << item->data (0).toString () << ", rowCount:" << ret);

	// DBGOUT("leaving"); // to excessive
	return ret;
}

int Xmms2dPrefModel::columnCount ( const QModelIndex & parent ) const {
	// DBGOUT("entered");
	int ret;
	Xmms2dPrefItem *item;

	if(parent.isValid ())
		item = static_cast<Xmms2dPrefItem*>(parent.internalPointer ());
	else
		item = rootItem;
	ret = item->columnCount ();
	// DBGOUT("data:" << item->data (0).toString () << ", columnCount:" << ret);
	// DBGOUT("leaving");
	return ret;
}

QModelIndex Xmms2dPrefModel::index(int row, int column, const QModelIndex & parent) const
{
	//DBGOUT("entered; row:" << row << ", column:" << column); // to excessive
	Xmms2dPrefItem *parentItem;

	if (!parent.isValid())
		parentItem = rootItem;
	else
		parentItem = static_cast<Xmms2dPrefItem*>(parent.internalPointer());

	Xmms2dPrefItem *childItem = parentItem->child(row);

	// DBGOUT("leaving"); // to excessive
	if (childItem) {
		// DBGOUT("row:" << row << ", column:" << column << ", data:" << childItem->data (column).toString ()); // to excessive
		return createIndex(row, column, childItem);
	}
	else {
		// DBGOUT("ERROR: childitem for row:" << row << "is invalid.");
		return QModelIndex();
	}
}


bool Xmms2dPrefModel::insertRow (int row, const QModelIndex &parent/* = QModelIndex ()*/)
{
	// DBGOUT("entered");
	beginInsertRows (parent, row, row);
	bool ret = false;
	Xmms2dPrefItem *parentItem;
	if(!parent.isValid ())
		parentItem = rootItem;
	else
		parentItem = static_cast<Xmms2dPrefItem *>(parent.internalPointer ());

	if(parentItem) {
		ret = parentItem->insertRow (row);
	}
	endInsertRows ();
	// DBGOUT("leaving");
	return ret;
}

QModelIndex Xmms2dPrefModel::findIndex (QVariant data, const QModelIndex & parent/* = QModelIndex()*/) const
{
	// DBGOUT("entered");
	Xmms2dPrefItem *parentItem = NULL, *item = NULL;
	QModelIndex ret;
	int column;

	if(parent.isValid ())
		parentItem = static_cast<Xmms2dPrefItem *> (parent.internalPointer ());
	else
		parentItem = rootItem;

	if (parentItem) {
		column = parentItem->findItemRecursive (data, &item);
		// DBGOUT("column:" << column << ",Item: " << item);
		if (column > -1 && item)
			ret = createIndex(item->row (), column, item);
	 }
	// DBGOUT("leaving");
	return ret;
}

QModelIndex Xmms2dPrefModel::parent(const QModelIndex &index) const
{
	if (!index.isValid())
		return QModelIndex();

	Xmms2dPrefItem *childItem = static_cast<Xmms2dPrefItem *> (index.internalPointer());
	Xmms2dPrefItem *parentItem = childItem->parent ();

	if (parentItem == rootItem)
		return QModelIndex ();

	return createIndex (parentItem->row(), 0, parentItem);
}

bool Xmms2dPrefModel::hasChildren ( const QModelIndex & parent/* = QModelIndex()*/) const
{
	bool ret = false;
	Xmms2dPrefItem *parentItem;

	if(parent.isValid ())
		parentItem = static_cast<Xmms2dPrefItem *> (parent.internalPointer ());
	else
		parentItem = rootItem;

	ret = parentItem->childCount () > 0;
	// DBGOUT("returning:" << ret);
	return ret;
}

void Xmms2dPrefModel::sort ( int column, Qt::SortOrder order/* = Qt::AscendingOrder */)
{
	rootItem->sort (column, order);
	reset ();
}
