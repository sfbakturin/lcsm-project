#include <Items/Item.h>
#include <Items/WireLine.h>
#include <lcsm/LCSM.h>

#include <QColor>
#include <QDebug>
#include <QGraphicsItem>
#include <QGraphicsPathItem>
#include <QPainterPath>
#include <QPen>
#include <QPointF>

WireLine::WireLine(Item *item1, Item *item2, lcsm::portid_t port1, lcsm::portid_t port2, QGraphicsItem *parent) :
	QGraphicsPathItem(parent), m_item1(item1), m_item2(item2), m_port1(port1), m_port2(port2)
{
	// Add this line to neighbours.
	item1->addWireLine(this);
	item2->addWireLine(this);

	// Pen setup.
	const QPen pen(QColor(Qt::GlobalColor::gray), 3, Qt::PenStyle::DotLine, Qt::PenCapStyle::RoundCap, Qt::RoundJoin);
	setPen(pen);

	// Z value.
	setZValue(0.0);

	// Initial adjust.
	adjust();
}

void WireLine::adjust()
{
	// Prepare geometrical changes due to movements.
	prepareGeometryChange();

	const QPointF start = mapFromItem(m_item1, m_item1->relativePositionOfPort(m_port1));
	const QPointF end = mapFromItem(m_item2, m_item2->relativePositionOfPort(m_port2));

	QPainterPath path(start);
	path.lineTo(end);

	setPath(path);
}

Item *WireLine::item1() noexcept
{
	return m_item1;
}

Item *WireLine::item2() noexcept
{
	return m_item2;
}

lcsm::portid_t WireLine::port1() const noexcept
{
	return m_port1;
}

lcsm::portid_t WireLine::port2() const noexcept
{
	return m_port2;
}
