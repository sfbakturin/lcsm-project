#ifndef LCMS_VISUAL_ITEMS_GROUNDITEM_H
#define LCMS_VISUAL_ITEMS_GROUNDITEM_H

#include <lcsm/Model/std/Ground.h>
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

class GroundItem : public ComponentItem
{
  public:
	GroundItem(CoreScene *scene, lcsm::model::Ground *ground, GUIOptions *options = nullptr);
	~GroundItem() noexcept = default;

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
	lcsm::model::Ground *m_ground;

	int m_nameKey;
	int m_facingKey;
	int m_widthKey;
};

#endif /* LCMS_VISUAL_ITEMS_GROUNDITEM_H */
