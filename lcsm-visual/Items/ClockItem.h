#ifndef LCSM_VISUAL_ITEMS_CLOCKITEM_H
#define LCSM_VISUAL_ITEMS_CLOCKITEM_H

#include <GUI/GUIOptions.h>
#include <Items/ComponentItem.h>
#include <lcsm/LCSM.h>
#include <lcsm/Model/std/Clock.h>

#include <QPainter>
#include <QPoint>
#include <QPointF>
#include <QRectF>
#include <QStyleOptionGraphicsItem>
#include <QVariant>
#include <QWidget>

class ClockItem : public ComponentItem
{
  public:
	ClockItem(CoreScene *scene, lcsm::model::Clock *clock, GUIOptions *options = nullptr);
	~ClockItem() noexcept = default;

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
	lcsm::model::Clock *m_clock;

	int m_nameKey;
	int m_facingKey;
	int m_highDurationKey;
	int m_lowDurationKey;
	int m_phaseOffsetKey;
};

#endif /* LCSM_VISUAL_ITEMS_CLOCKITEM_H */
