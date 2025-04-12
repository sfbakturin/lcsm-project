#ifndef LCSM_VISUAL_ITEMS_ITEM_H
#define LCSM_VISUAL_ITEMS_ITEM_H

#include <GUI/GUIOptions.h>
#include <lcsm/LCSM.h>
#include <lcsm/Model/Component.h>
#include <lcsm/Model/Identifier.h>

#include <QAction>
#include <QGraphicsItem>
#include <QGraphicsSceneHoverEvent>
#include <QGraphicsSceneMouseEvent>
#include <QMenu>
#include <QPointF>
#include <memory>

class CoreScene;

class Item : public QGraphicsItem
{
  public:
	enum class ItemType : uint
	{
		ComponentItemTy,
		VerilogItemTy,
		CircuitItemTy,
		WireItemTy
	};

	enum ItemDirection : uint
	{
		East = 0,
		South = 1,
		West = 2,
		North = 3
	};

  public:
	Item(CoreScene *coreScene, lcsm::Identifier id, GUIOptions *options = nullptr);
	virtual ~Item() noexcept = default;

	virtual ItemType ty() const noexcept = 0;

	lcsm::Identifier id() const noexcept;
	virtual void reid() noexcept = 0;

	GUIOptions *options() noexcept;
	const GUIOptions *options() const noexcept;
	void setOptions(GUIOptions *options) noexcept;

	void setFreeze(bool freeze) noexcept;
	bool freeze() const noexcept;

	void rotateRight();
	void rotateLeft();

	void setAboutToBeConnected(bool aboutToBeConnected);
	bool aboutToBeConnected() const noexcept;

	virtual QPointF relativePositionOfPort(lcsm::portid_t portId) const = 0;
	QPointF absolutePositionOfPort(lcsm::portid_t portId) const;

  protected:
	void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

	void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;

	virtual void connect() = 0;

  protected:
	CoreScene *m_coreScene;
	lcsm::Identifier m_id;
	GUIOptions *m_options;

	std::unique_ptr< QMenu > m_contextMenu;
	QAction *m_connectAction;
	QAction *m_removeAction;
	QAction *m_rotateRightAction;
	QAction *m_rotateLeftAction;

	bool m_freeze;
	bool m_aboutToBeConnected;

	ItemDirection m_direction;
};

#endif /* LCSM_VISUAL_ITEMS_ITEM_H */
