#ifndef LCSM_VISUAL_ITEMS_PINITEM_H
#define LCSM_VISUAL_ITEMS_PINITEM_H

#include <GUI/GUIOptions.h>
#include <Items/ComponentItem.h>
#include <lcsm/Model/std/Pin.h>

#include <QPainter>
#include <QPoint>
#include <QPointF>
#include <QRectF>
#include <QStyleOptionGraphicsItem>
#include <QVariant>
#include <QWidget>

class CoreScene;

class PinItem : public ComponentItem
{
  public:
	PinItem(CoreScene *scene, lcsm::model::Pin *pin, GUIOptions *options = nullptr);
	~PinItem() noexcept = default;

	virtual QRectF boundingRect() const override;
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

	virtual QPointF relativePositionOfPort(lcsm::portid_t portId) const override final;

  protected:
	virtual void connect() override final;

  private:
	lcsm::model::Pin *m_pin;

  private:
	QPointF relativePositionOfPort() const noexcept;
	QPointF offsetBoundingRect() const noexcept;
};

#endif /* LCSM_VISUAL_ITEMS_PINITEM_H */
