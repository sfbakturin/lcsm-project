#ifndef LCSM_VISUAL_ITEMS_ITEM_H
#define LCSM_VISUAL_ITEMS_ITEM_H

#include <lcsm/Model/Component.h>
#include <lcsm/Model/Identifier.h>

#include <QGraphicsItem>
#include <QPainter>
#include <QRectF>
#include <QStyleOptionGraphicsItem>
#include <QWidget>

class Item : public QGraphicsItem
{
  public:
	Item(lcsm::Identifier id);
	virtual ~Item() noexcept = default;

	lcsm::Identifier id() const noexcept;

	virtual QRectF boundingRect() const override;

	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

  private:
	lcsm::Identifier m_id;
};

#endif /* LCSM_VISUAL_ITEMS_ITEM_H */
