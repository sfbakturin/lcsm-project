#ifndef LCSM_VISUAL_ITEMS_WIREITEM_H
#define LCSM_VISUAL_ITEMS_WIREITEM_H

#include <GUI/GUIOptions.h>
#include <Items/ImmovableItem.h>
#include <Items/Item.h>
#include <lcsm/LCSM.h>
#include <lcsm/Model/Wire.h>

#include <QPainter>
#include <QPainterPath>
#include <QPointF>
#include <QRectF>
#include <QStyleOptionGraphicsItem>
#include <QWidget>

class CoreScene;

class WireItem : public ImmovableItem
{
  public:
	WireItem(Item *item1, Item *item2, lcsm::portid_t portId1, lcsm::portid_t portId2, CoreScene *coreScene, lcsm::model::Wire *wire, GUIOptions *options = nullptr);
	~WireItem() noexcept = default;

	virtual ItemType ty() const noexcept override final;
	virtual void reid() noexcept override final;

	lcsm::model::Wire *wire() noexcept;

	virtual QRectF boundingRect() const override;
	virtual QPainterPath shape() const override;
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

	virtual QPointF relativePositionOfPort(lcsm::portid_t portId) const override final;

	Item *item1() noexcept;
	Item *item2() noexcept;

  protected:
	virtual void connect() override final;

  private:
	QPointF relativePositionOfPort() const noexcept;

  private:
	lcsm::model::Wire *m_wire;
	Item *m_item1;
	Item *m_item2;
	lcsm::portid_t m_portId1;
	lcsm::portid_t m_portId2;
};

#endif /* LCSM_VISUAL_ITEMS_WIREITEM_H */
