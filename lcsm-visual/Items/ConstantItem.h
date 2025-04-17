#ifndef LCMS_VISUAL_ITEMS_CONSTANTITEM_H
#define LCMS_VISUAL_ITEMS_CONSTANTITEM_H

#include <lcsm/Model/std/Constant.h>
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

class ConstantItem : public ComponentItem
{
  public:
	ConstantItem(CoreScene *scene, lcsm::model::Constant *constant, GUIOptions *options = nullptr);
	~ConstantItem() noexcept = default;

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
	lcsm::model::Constant *m_constant;

	int m_nameKey;
	int m_facingKey;
	int m_widthKey;
	int m_valueKey;
};

#endif /* LCMS_VISUAL_ITEMS_CONSTANTITEM_H */
