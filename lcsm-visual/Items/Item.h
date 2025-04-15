#ifndef LCSM_VISUAL_ITEMS_ITEM_H
#define LCSM_VISUAL_ITEMS_ITEM_H

#include <GUI/GUIOptions.h>
#include <lcsm/LCSM.h>
#include <lcsm/LCSMState.h>
#include <lcsm/Model/Component.h>
#include <lcsm/Model/Identifier.h>
#include <lcsm/Support/PointerView.hpp>

#include <QAction>
#include <QGraphicsItem>
#include <QGraphicsSceneHoverEvent>
#include <QGraphicsSceneMouseEvent>
#include <QMenu>
#include <QPointF>
#include <memory>

class CoreScene;
class WireLine;
class PropertiesList;

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

	void setSimulate(bool simulate) noexcept;
	bool simulate() const noexcept;

	void rotateRight();
	void rotateLeft();

	void setAboutToBeConnected(bool aboutToBeConnected);
	bool aboutToBeConnected() const noexcept;

	virtual QPointF relativePositionOfPort(lcsm::portid_t portId) const = 0;
	QPointF absolutePositionOfPort(lcsm::portid_t portId) const;

	void addWireLine(WireLine *wireLine);
	void removeWireLine(WireLine *wireLine);
	QList< WireLine * > &wireLines() noexcept;

	void setState(lcsm::LCSMState *state) noexcept;
	void resetState() noexcept;

	virtual void setProperty(int key, const QVariant &value) = 0;
	virtual void setPropertiesList(PropertiesList *propertiesList) = 0;

  protected:
	void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

	void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;

	virtual void connect() = 0;

	void directionChanged();
	void adjust();

	virtual bool rotateActionEnabled() const noexcept = 0;
	virtual bool putValueActionEnabled() const noexcept = 0;

	virtual lcsm::width_t inputWidth() const noexcept = 0;

  protected:
	CoreScene *m_coreScene;
	lcsm::Identifier m_id;
	GUIOptions *m_options;

	std::unique_ptr< QMenu > m_designContextMenu;
	lcsm::support::PointerView< QAction > m_connectAction;
	lcsm::support::PointerView< QAction > m_removeAction;
	lcsm::support::PointerView< QAction > m_rotateRightAction;
	lcsm::support::PointerView< QAction > m_rotateLeftAction;

	std::unique_ptr< QMenu > m_simulateContextMenu;
	lcsm::support::PointerView< QAction > m_putValueAction;
	lcsm::support::PointerView< QAction > m_showValueAction;

	bool m_aboutToBeConnected;
	bool m_simulate;

	ItemDirection m_direction;

	QList< WireLine * > m_wireLines;

	lcsm::support::PointerView< lcsm::LCSMState > m_state;
};

#endif /* LCSM_VISUAL_ITEMS_ITEM_H */
