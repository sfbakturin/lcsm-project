#include <View/DesignExplorerList.h>

#include <QAction>
#include <QListView>
#include <QMenu>
#include <QModelIndex>
#include <QObject>
#include <QPoint>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QString>

DesignExplorerList::DesignExplorerList(unsigned option, QWidget *parent) :
	QListView(parent), m_model(new QStandardItemModel(this)), m_openAction(nullptr), m_addToSceneAction(nullptr),
	m_removeAction(nullptr), m_openedIndex(-1)
{
	// View setup.
	setContextMenuPolicy(Qt::CustomContextMenu);
	setDragDropMode(QAbstractItemView::InternalMove);
	setDefaultDropAction(Qt::MoveAction);
	setSelectionMode(QAbstractItemView::SingleSelection);

	// Model.
	setModel(m_model);

	// Set context menu.
	connect(this, &QListView::customContextMenuRequested, this, &DesignExplorerList::onShowContextMenu);
	connect(this, &QListView::doubleClicked, this, &DesignExplorerList::onDoubleClicked);

	// Initialize menu.
	m_contextMenu = new QMenu(this);

	if (option & DesignExplorerList::ListOption::Openable)
	{
		m_openAction = m_contextMenu->addAction(tr("Open"));
	}

	if (option & DesignExplorerList::ListOption::AddeableToCircuit)
	{
		m_addToSceneAction = m_contextMenu->addAction(tr("Add to scene"));
	}

	if (option & DesignExplorerList::ListOption::Removeable)
	{
		m_removeAction = m_contextMenu->addAction(tr("Remove"));
	}
}

void DesignExplorerList::add(const QString &text)
{
	QStandardItem *item = new QStandardItem(text);
	item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled);
	m_model->appendRow(item);
}

static inline void SetEnabled(QAction *action, bool enabled)
{
	if (action)
	{
		action->setEnabled(enabled);
	}
}

void DesignExplorerList::setOpenEnabled(bool enabled)
{
	SetEnabled(m_openAction, enabled);
}

void DesignExplorerList::setAddToSceneEnabled(bool enabled)
{
	SetEnabled(m_addToSceneAction, enabled);
}

void DesignExplorerList::setRemoveEnabled(bool enabled)
{
	SetEnabled(m_removeAction, enabled);
}

void DesignExplorerList::onShowContextMenu(const QPoint &p)
{
	QModelIndex index = indexAt(p);
	if (index.isValid())
	{
		const QString item = index.data().toString();
		QAction *selected = m_contextMenu->exec(viewport()->mapToGlobal(p));
		if (selected == m_openAction)
		{
			emit open(item);
		}
		else if (selected == m_addToSceneAction)
		{
			emit addToScene(item);
		}
		else if (selected == m_removeAction)
		{
			m_model->removeRow(index.row());
			emit remove(item);
		}
	}
}

void DesignExplorerList::onDoubleClicked(const QModelIndex &index)
{
	if (index.isValid())
	{
		const QString item = index.data().toString();
		if (m_openAction != nullptr)
		{
			emit open(item);
		}
		else
		{
			emit addToScene(item);
		}
	}
}
