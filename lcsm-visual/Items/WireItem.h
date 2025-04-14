#ifndef LCSM_VISUAL_ITEMS_WIREITEM_H
#define LCSM_VISUAL_ITEMS_WIREITEM_H

#include <GUI/GUIOptions.h>
#include <Items/Item.h>
#include <Items/MovableItem.h>
#include <lcsm/LCSM.h>
#include <lcsm/Model/Wire.h>

#include <QPainter>
#include <QPainterPath>
#include <QPointF>
#include <QRectF>
#include <QStyleOptionGraphicsItem>
#include <QVariant>
#include <QWidget>

class CoreScene;
class PropertiesList;

class WireItem : public MovableItem
{
  public:
	WireItem(CoreScene *coreScene, lcsm::model::Wire *wire, GUIOptions *options = nullptr);
	~WireItem() noexcept = default;

	virtual ItemType ty() const noexcept override final;
	virtual void reid() noexcept override final;

	lcsm::model::Wire *wire() noexcept;

	virtual QRectF boundingRect() const override;
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

	virtual QPointF relativePositionOfPort(lcsm::portid_t portId) const override final;

	virtual void setProperty(int key, const QVariant &value) override final;
	virtual void setPropertiesList(PropertiesList *propertiesList) override final;

  protected:
	virtual void connect() override final;

	virtual bool rotateActionEnabled() const noexcept override final;
	virtual bool putValueActionEnabled() const noexcept override final;

  private:
	lcsm::model::Wire *m_wire;

	int m_nameKey;
};

#endif /* LCSM_VISUAL_ITEMS_WIREITEM_H */
