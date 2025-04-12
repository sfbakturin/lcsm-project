#ifndef LCSM_VISUAL_VIEW_DESIGNEXPLORERLIST_H
#define LCSM_VISUAL_VIEW_DESIGNEXPLORERLIST_H

#include <QAction>
#include <QListView>
#include <QMenu>
#include <QModelIndex>
#include <QObject>
#include <QPoint>
#include <QStandardItemModel>
#include <QString>

class DesignExplorerList : public QListView
{
	Q_OBJECT

  public:
	enum ListOption : unsigned
	{
		Openable = 1 << 0,
		AddeableToCircuit = 1 << 1,
		Removeable = 1 << 2
	};

  public:
	explicit DesignExplorerList(unsigned option, QWidget *parent = nullptr);
	~DesignExplorerList() noexcept = default;

	void add(const QString &item);

	void setOpenEnabled(bool enabled);
	void setAddToSceneEnabled(bool enabled);
	void setRemoveEnabled(bool enabled);

  signals:
	void open(const QString &item);
	void addToScene(const QString &item);
	void remove(const QString &item);

  private slots:
	void onShowContextMenu(const QPoint &pos);
	void onDoubleClicked(const QModelIndex &index);

  private:
	QStandardItemModel *m_model;

	QMenu *m_contextMenu;
	QAction *m_openAction;
	QAction *m_addToSceneAction;
	QAction *m_removeAction;

	int m_openedIndex;
};

#endif /* LCSM_VISUAL_VIEW_DESIGNEXPLORERLIST_H */
