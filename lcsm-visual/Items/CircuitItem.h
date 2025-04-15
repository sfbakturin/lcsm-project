#ifndef LCSM_VISUAL_ITEMS_CIRCUITITEM_H
#define LCSM_VISUAL_ITEMS_CIRCUITITEM_H

#include <GUI/GUIOptions.h>
#include <Items/MovableItem.h>
#include <lcsm/LCSM.h>
#include <lcsm/LCSMCircuit.h>

#include <QPainter>
#include <QPointF>
#include <QRectF>
#include <QStyleOptionGraphicsItem>
#include <QVariant>
#include <QWidget>

class CoreScene;
class PropertiesList;

class CircuitItem : public MovableItem
{
  public:
	CircuitItem(CoreScene *scene, const lcsm::LCSMCircuitView &view, GUIOptions *options = nullptr);
	~CircuitItem() noexcept = default;

	virtual ItemType ty() const noexcept override final;
	virtual void reid() noexcept override final;

	virtual QRectF boundingRect() const override;
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

	virtual QPointF relativePositionOfPort(lcsm::portid_t portId) const override final;

	virtual void setProperty(int key, const QVariant &value) override final;
	virtual void setPropertiesList(PropertiesList *propertiesList) override final;

	const lcsm::LCSMCircuitView &circuit() const noexcept;

  protected:
	virtual void connect() override final;

	virtual bool rotateActionEnabled() const noexcept override final;
	virtual bool putValueActionEnabled() const noexcept override final;

	virtual lcsm::width_t inputWidth() const noexcept override final;

  private:
	lcsm::LCSMCircuitView m_view;
};

#endif /* LCSM_VISUAL_ITEMS_CIRCUITITEM_H */
