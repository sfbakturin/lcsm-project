#include <Core/CoreScene.h>
#include <GUI/GUIOptions.h>
#include <Items/ImmovableItem.h>
#include <Items/Item.h>
#include <Items/WireItem.h>
#include <lcsm/LCSM.h>
#include <lcsm/Model/Wire.h>

#include <QDebug>
#include <QPainter>
#include <QPainterPath>
#include <QPointF>
#include <QRectF>
#include <QStyleOptionGraphicsItem>
#include <QWidget>

WireItem::WireItem(Item *item1, Item *item2, lcsm::portid_t portId1, lcsm::portid_t portId2, CoreScene *coreScene, lcsm::model::Wire *wire, GUIOptions *options) :
	ImmovableItem(coreScene, wire->id(), options), m_wire(wire), m_item1(item1), m_item2(item2), m_portId1(portId1),
	m_portId2(portId2)
{
}

Item::ItemType WireItem::ty() const noexcept
{
	return Item::ItemType::WireItemTy;
}

void WireItem::reid() noexcept
{
	m_id = m_wire->id();
}

lcsm::model::Wire *WireItem::wire() noexcept
{
	return m_wire;
}

QRectF WireItem::boundingRect() const
{
	const qreal gridSize = options()->gridSize();

	const QPointF p1 = m_item1->absolutePositionOfPort(m_portId1);
	const QPointF p2 = m_item2->absolutePositionOfPort(m_portId2);

	if (p1.x() == p2.x())
	{
		const qreal height = qAbs(p1.y() - p2.y());
		return QRectF(-gridSize, 0.0, 2 * gridSize, height).normalized();
	}
	else if (p1.y() == p2.y())
	{
		const qreal width = qAbs(p1.x() - p2.x());
		return QRectF(0.0, -gridSize, width, 2 * gridSize).normalized();
	}
	else
	{
		const qreal width = qAbs(p1.x() - p2.x()) + gridSize;
		const qreal height = qAbs(p1.y() - p2.y()) + gridSize;
		return QRectF(-gridSize, -gridSize, width, height).normalized();
	}
}

static constexpr lcsm::portid_t PORT = lcsm::model::Wire::Port::Wiring;

QPainterPath WireItem::shape() const
{
	const QPointF p1 = mapFromItem(m_item1, m_item1->relativePositionOfPort(m_portId1));
	const QPointF p2 = mapFromItem(m_item2, m_item2->relativePositionOfPort(m_portId2));
	const QPointF pc = relativePositionOfPort(PORT);

	const qreal gridSize = static_cast< qreal >(options()->gridSize());
	const qreal width = static_cast< qreal >(2 * gridSize);
	const int r = static_cast< int >(static_cast< qreal >(width) * 0.2);

	QPainterPath path(p1);
	path.lineTo(pc);
	path.addEllipse(pc, r, r);
	path.lineTo(p2);

	return path;
}

void WireItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	const qreal gridSize = static_cast< qreal >(options()->gridSize());
	const qreal width = static_cast< qreal >(2 * gridSize);
	const int r = static_cast< int >(static_cast< qreal >(width) * 0.2);

	const QPointF p1 = mapFromItem(m_item1, m_item1->relativePositionOfPort(m_portId1));
	const QPointF p2 = mapFromItem(m_item2, m_item2->relativePositionOfPort(m_portId2));
	const QPointF pc = relativePositionOfPort(PORT);
	painter->drawEllipse(pc, r, r);

	QPen pen(Qt::black);
	pen.setWidth(3);
	painter->setPen(pen);

	painter->drawLine(p1, pc);
	painter->drawLine(p2, pc);
}

QPointF WireItem::relativePositionOfPort(lcsm::portid_t portId) const
{
	const lcsm::model::Wire::Port p = static_cast< lcsm::model::Wire::Port >(portId);
	const qreal gs = static_cast< qreal >(options()->gridSize());

	switch (p)
	{
	case lcsm::model::Wire::Port::Wiring:
	{
		const QPointF p1 = mapFromItem(m_item1, m_item1->relativePositionOfPort(m_portId1));
		const QPointF p2 = mapFromItem(m_item2, m_item2->relativePositionOfPort(m_portId2));
		if (p1.x() == p2.x())
		{
			const qreal x = p1.x();
			const qreal y = (p1.y() + p2.y()) / 2.0;
			return QPointF(x, qRound(y / gs) * gs);
		}
		else if (p1.y() == p2.y())
		{
			const qreal x = (p1.x() + p2.x()) / 2.0;
			const qreal y = p1.y();
			return QPointF(qRound(x / gs) * gs, y);
		}
		else if (p1.y() < p2.y())
		{
			return QPointF(p1.x(), p2.y());
		}
		else
		{
			return QPointF(p2.x(), p1.y());
		}
	}
	default:
	{
		break;
	}
	}

	throw std::logic_error("Bad port!");
}

Item *WireItem::item1() noexcept
{
	return m_item1;
}

Item *WireItem::item2() noexcept
{
	return m_item2;
}

void WireItem::connect()
{
	m_coreScene->connection(id(), lcsm::model::Wire::Port::Wiring);
}
