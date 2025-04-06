#include <Items/Item.h>
#include <lcsm/Model/Identifier.h>

#include <QPainter>
#include <QRectF>
#include <QStyleOptionGraphicsItem>
#include <QWidget>

Item::Item(lcsm::Identifier id) : m_id(id) {}

lcsm::Identifier Item::id() const noexcept
{
	return m_id;
}

QRectF Item::boundingRect() const
{
	return QRectF();
}

void Item::paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *) {}
