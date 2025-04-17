#ifndef LCMS_VISUAL_ITEMS_POWERITEM_H
#define LCMS_VISUAL_ITEMS_POWERITEM_H

#include <lcsm/Model/std/Power.h>
#include <GUI/GUIOptions.h>
#include <Items/ComponentItem.h>
#include <lcsm/LCSM.h>

#include <QPainter>
#include <QPoint>
#include <QPointF>
#include <QRectF>
#include <QStyleOptionGraphicsItem>
#include <QVariant>
#include <QWidget>

class CoreScene;
class PropertiesList;

class PowerItem : public ComponentItem
{
  public:
	PowerItem(CoreScene *scene, lcsm::model::Power *power, GUIOptions *options = nullptr);
	~PowerItem() noexcept = default;

	virtual QRectF boundingRect() const override;
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

	virtual QPointF relativePositionOfPort(lcsm::portid_t portId) const override final;

	virtual void setProperty(int key, const QVariant &value) override final;
	virtual void setPropertiesList(PropertiesList *propertiesList) override final;

  protected:
	virtual void connect() override final;

	virtual bool rotateActionEnabled() const noexcept override final;
	virtual bool putValueActionEnabled() const noexcept override final;

	virtual lcsm::width_t inputWidth() const noexcept override final;

  private:
	lcsm::model::Power *m_power;

	int m_nameKey;
	int m_facingKey;
	int m_widthKey;
};

#endif /* LCMS_VISUAL_ITEMS_POWERITEM_H */
