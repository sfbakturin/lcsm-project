#ifndef LCSM_VISUAL_ITEMS_TRANSISTORITEM_H
#define LCSM_VISUAL_ITEMS_TRANSISTORITEM_H

#include <GUI/GUIOptions.h>
#include <Items/ComponentItem.h>
#include <lcsm/LCSM.h>
#include <lcsm/Model/std/Transistor.h>

#include <QList>
#include <QPainter>
#include <QPoint>
#include <QPointF>
#include <QRectF>
#include <QString>
#include <QStyleOptionGraphicsItem>
#include <QVariant>
#include <QWidget>

class TransistorItem : public ComponentItem
{
  public:
	TransistorItem(CoreScene *scene, lcsm::model::Transistor *transistor, GUIOptions *options = nullptr);
	~TransistorItem() noexcept = default;

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
	lcsm::model::Transistor *m_transistor;

	int m_nameKey;
	int m_facingKey;
	int m_typeKey;

	QList< QString > m_portStrings;
};

#endif /* LCSM_VISUAL_ITEMS_TRANSISTORITEM_H */
