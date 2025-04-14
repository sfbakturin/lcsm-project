#ifndef LCSM_VISUAL_ITEMS_VERILOGITEM_H
#define LCSM_VISUAL_ITEMS_VERILOGITEM_H

#include <GUI/GUIOptions.h>
#include <Items/MovableItem.h>
#include <lcsm/Model/Verilog.h>

#include <QPainter>
#include <QPointF>
#include <QRectF>
#include <QStyleOptionGraphicsItem>
#include <QVariant>
#include <QWidget>

class CoreScene;
class PropertiesList;

class VerilogItem : public MovableItem
{
  public:
	VerilogItem(CoreScene *scene, lcsm::model::VerilogModule *verilogModule, GUIOptions *options = nullptr);

	virtual ItemType ty() const noexcept override final;
	virtual void reid() noexcept override final;

	virtual QRectF boundingRect() const override;
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

	virtual QPointF relativePositionOfPort(lcsm::portid_t portId) const override final;

	virtual void setProperty(int key, const QVariant &value) override final;
	virtual void setPropertiesList(PropertiesList *propertiesList) override final;

	lcsm::Component *component() noexcept;

  protected:
	virtual void connect() override final;

	virtual bool rotateActionEnabled() const noexcept override final;
	virtual bool putValueActionEnabled() const noexcept override final;

  private:
	lcsm::model::VerilogModule *m_verilogModule;
};

#endif /* LCSM_VISUAL_ITEMS_VERILOGITEM_H */
